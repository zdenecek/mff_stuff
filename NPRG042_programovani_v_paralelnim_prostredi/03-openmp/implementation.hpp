#ifndef LEVENSHTEIN_IMPLEMENTATION_HPP
#define LEVENSHTEIN_IMPLEMENTATION_HPP

#include <vector>
#include <numeric>
#include <cassert>
#include <algorithm>
#ifdef _OPENMP
// runs also serially if no OpenMP is available
// for compilation on my macbook, where I was just checking compilation and correctness
#include <omp.h>
#endif

template<typename C = char, typename DIST = std::size_t, bool DEBUG = false, std::size_t BLOCK_SIZE = 1024>
class EditDistanceGeneric : public IEditDistance<C, DIST, DEBUG>
{
private:
    std::vector<DIST> vals_rows;
    std::vector<DIST> vals_columns;
    std::vector<DIST> vals_diagonal;

    DIST len1;  
    DIST len2;  

public:
	/*
	 * \brief Perform the initialization of the functor (e.g., allocate memory buffers).
	 * \param len1, len2 Lengths of first and second string respectively.
	 */
    virtual void init(DIST l1, DIST l2)
    {
        this->len1 = l1;
        this->len2 = l2;

        vals_rows.resize(len1 + 1);
        vals_columns.resize(len2 + 1);
        std::size_t num_block_cols = len1 / BLOCK_SIZE;
        vals_diagonal.resize(num_block_cols + 1);

        std::iota(vals_rows.begin(), vals_rows.end(), 0);
        std::iota(vals_columns.begin(), vals_columns.end(), 0);
        vals_diagonal[0] = 0;
    }
	/*
	 * \brief Compute the distance between two strings.
	 * \param str1, str2 Strings to be compared.
	 * \result The computed edit distance.
	 */
    virtual DIST compute(const std::vector<C> &str1, const std::vector<C> &str2)
    {
        assert(str1.size() == len1 && str2.size() == len2); // sanity check when debugging..
        assert(len1 % BLOCK_SIZE == 0 && len2 % BLOCK_SIZE == 0);

        std::size_t cols = len1 / BLOCK_SIZE;  
        std::size_t rows = len2 / BLOCK_SIZE;  

		// process the submatrices by antidiagonal
		/* Example:
		 *   0  1  2  3  4
		 * 0 1  2  3  4  5
		 * 1 2  3  4  5  6
		 * 2 3  4  5  6  7
		 * 3 4  5  6  7  8
		 * 4 5  6  7  8  9
		 * 5 6  7  8  9  10
		*/

        for (std::size_t anti_diag = 1; anti_diag < rows + cols; ++anti_diag)
        {
            std::size_t start_col = (anti_diag < rows) ? 0 : anti_diag - rows;
            std::size_t end_col = std::min(anti_diag, cols);

#pragma omp parallel for schedule(static)
            for (std::size_t col_block = start_col; col_block < end_col; ++col_block)
            {
                std::size_t row_block = anti_diag - col_block - 1;
                compute_submatrix(row_block, col_block, str1, str2);
            }

            if (anti_diag <= cols)
            {
                vals_diagonal[anti_diag] = vals_rows[anti_diag * BLOCK_SIZE];
            }
        }

        return vals_rows[len1];
    }

private:
	/*
	 * compute the submatrix for a given block row and block column
	 */
    void compute_submatrix(std::size_t block_row, std::size_t block_col, 
                           const std::vector<C> &horiz_str, const std::vector<C> &vert_str)
    {
        std::size_t start_row = block_row * BLOCK_SIZE + 1;
        std::size_t start_col = block_col * BLOCK_SIZE + 1;

        DIST diag_val = vals_diagonal[block_col];

#pragma omp simd
        for (std::size_t i = start_row; i < start_row + BLOCK_SIZE; ++i)
        {
            DIST last_upper = vals_columns[i];  
            for (std::size_t j = start_col; j < start_col + BLOCK_SIZE; ++j)
            {
                DIST cost = (horiz_str[j - 1] == vert_str[i - 1]) ? 0 : 1;
                DIST min_adjacent = std::min(vals_rows[j], vals_columns[i]) + 1;
                DIST diagonal_plus_cost = diag_val + cost;
                diag_val = vals_rows[j];
                vals_rows[j] = vals_columns[i] = std::min(min_adjacent, diagonal_plus_cost);
            }
            diag_val = last_upper;
        }
        vals_diagonal[block_col] = diag_val;
    }
};

template<typename C = char, typename DIST = std::size_t, bool DEBUG = false>
class EditDistance : public EditDistanceGeneric<C, DIST, DEBUG, (DEBUG ? 64 : 1024)>
{};

#endif
