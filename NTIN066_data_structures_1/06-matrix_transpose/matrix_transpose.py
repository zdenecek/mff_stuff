class Matrix:
    def __init__(self, N):
        self.N = N

    def swap(self, i1, j1, i2, j2):
        """Swap elements (i1, j1) and (i2, j2)."""
        raise NotImplementedError

    def transpose(self):
        """Public method to transpose the matrix."""
        self.transpose_recursive(0, 0, self.N, self.N)

    def transpose_recursive(self, start_row, start_col, end_row, end_col):
        """
        Recursive helper for cache-oblivious transpose.
        Transposes a submatrix defined by [start_row:end_row, start_col:end_col].
        """
        if end_row - start_row <= 1 or end_col - start_col <= 1:
            return

        mid_row = (start_row + end_row) // 2
        mid_col = (start_col + end_col) // 2

        self.transpose_recursive(start_row, start_col, mid_row, mid_col)

        self.transpose_recursive(mid_row, mid_col, end_row, end_col)

        for i in range(start_row, mid_row):
            for j in range(mid_col, end_col):
                self.swap(i, j, j, i)

        self.transpose_recursive(start_row, mid_col, mid_row, end_col)

        self.transpose_recursive(mid_row, start_col, end_row, mid_col)
