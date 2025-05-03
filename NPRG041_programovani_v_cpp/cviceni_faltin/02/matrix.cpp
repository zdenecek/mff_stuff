#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <numeric>

using namespace std;

class IntMatrix
{
    int height;
    int width;
    vector<vector<int>> data;

public:
    IntMatrix() : IntMatrix(10, 10) {}
    IntMatrix(int width, int height)
    {
        data.resize(width, vector<int>(height));
        this->width = width;
        this->height = height;
    }
    void set(int x, int y, int value)
    {
        data.at(x).at(y) = value;
    }
    int get(int x, int y) const
    {
        return data.at(x).at(y);
    }
    void set_width(int width)
    {
        data.resize(width, vector<int>(height));
        this->width = width;
    }
    void set_height(int height)
    {
        for (int r = 0; r < width; r++)
        {
            data.at(r).resize(height);
        }
        this->height = height;
    }
    int get_width()
    {
        return this->width;
    }
    int get_height()
    {
        return this->height;
    }

    vector<int> get_vector(int axis, int index) const
    {
        if (axis == 1)
        {
            vector<int> row(data.at(index));
            return row;
        }

        vector<int> col;
        col.resize(width);
        for (int r = 0; r < width; r++)
        {
            col.at(r) = data.at(r).at(index);
        }
        return col;
    }

    void clear()
    {
        this->fill_with_value(0);
    }
    void fill_with_value(int value)
    {
        for (int r = 0; r < width; r++)
        {
            std::fill(data.at(r).begin(), data.at(r).end(), value);
        }
    }

    int num_zeros() const
    {
        return count([](int number) -> bool
                     { return number == 0; });
    }

    int num_negative() const
    {
        return count([](int number) -> bool
                     { return number < 0; });
    }

    int num_positive() const
    {
        return count([](int number) -> bool
                     { return number > 0; });
    }

    int count(const std::function<int(bool)> &acc) const
    {

        int count = 0;
        for (auto row : data)
        {
            for (auto i : row)
            {
                if (acc(i))
                    count++;
            }
        }
        return count;
    }
};

class Int3DMatrix
{
    int length;
    vector<IntMatrix> data;

public:
    Int3DMatrix() : Int3DMatrix(10, 10, 10) {}
    Int3DMatrix(int width, int height, int length)
    {
        this->length = length;
        data.resize(length, IntMatrix(width, length));
        set_width(width);
        set_height(height);
    }
    void set(int x, int y, int z, int value)
    {
        data.at(z).set(x, y, value);
    }
    int get(int x, int y, int z) const
    {
        return data.at(z).get(x, y);
    }
    void set_width(int width)
    {
        for (int z = 0; z < length; z++)
        {
            data.at(z).set_width(width);
        }
    }
    void set_height(int height)
    {
        for (int z = 0; z < length; z++)
        {
            data.at(z).set_height(height);
        }
    }
    void set_length(int length)
    {
        this->length = length;
        data.resize(length, IntMatrix(get_width(), get_height()));
    }
    int get_width()
    {
        return data.at(0).get_width();
    }
    int get_height()
    {
        return data.at(0).get_height();
    }

    vector<int> get_vector(int axis, int index1, int index2) const
    {
        if (axis == 0 || axis == 1)
        {
            return data.at(index2).get_vector(axis, index1);
        }

        vector<int> vec;
        vec.resize(length);
        for (int z = 0; z < length; z++)
        {
            vec.at(z) = data.at(z).get(index1, index2);
        }
        return vec;
    }

    void clear()
    {
        this->fill_with_value(0);
    }
    void fill_with_value(int value)
    {
        for (int z = 0; z < length; z++)
        {
            data.at(z).fill_with_value(value);
        }
    }

    int num_zeros() const
    {
        return count([](int number) -> bool
                     { return number == 0; });
    }

    int num_negative() const
    {
        return count([](int number) -> bool
                     { return number < 0; });
    }

    int num_positive() const
    {
        return count([](int number) -> bool
                     { return number > 0; });
    }

    int count(const std::function<int(bool)> &acc) const
    {

        int count = 0;
        for (auto slice : data)
        {
            count += slice.count(acc);
        }
        return count;
    }
};

void _main(vector<string> args)
{
    Int3DMatrix a(1, 2, 3);

    cout << a.num_zeros() << endl;
}


int main(int argc, char **argv)
{
    vector<string> args(argv, argv + argc);
    _main(args);
    return 0;
}
