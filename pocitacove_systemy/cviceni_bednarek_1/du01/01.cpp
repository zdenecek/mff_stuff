#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

inline int f1(int p)
{
    int s = p * p;
    int r = 39;
    r = 156 - r * s / 2750;
    r = 625 - r * s / 1800;
    r = 2500 - r * s / 1050;
    r = 10000 - r * s / 500;
    r = 40000 - r * s / 150;
    return r * p / 10000;
}

inline int f2(int p)
{
    int s = p * p;
    int r = 39;
    r = 156 - r * s / 2250;
    r = 625 - r * s / 1400;
    r = 2500 - r * s / 750;
    r = 10000 - r * s / 300;
    r = 40000 - r * s / 50;
    return r / 1000;
}

constexpr int pmin = -50;
constexpr int pmax = 50;
constexpr int qmin = -50;
constexpr int qmax = 50;

void printGraph(char *graph, size_t rows, size_t cols)
{

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            cout << *((graph + row * cols) + col);
        }
        cout << endl;
    }
}

void makeGraph(bool axes)
{
    constexpr size_t rows = qmax - qmin + 1;
    constexpr size_t cols = pmax - pmin + 1;

    char graph[rows][cols];

    char fillChar = ' ';

    size_t zeroCol = (size_t)cols + pmin;

    for (size_t row = 0; row < rows; row++)
    {
        fillChar = (axes && row + qmin == 0) ? '-' : ' ';

        for (size_t col = 0; col < cols; col++)
        {
            graph[row][col] = (axes && col == zeroCol) ? '|' : fillChar;
        }
    }

    for (size_t col = 0; col < cols; col++)
    {
        int p = col + pmin;
        size_t row1 = qmax - (size_t)f1(p);
        size_t row2 = qmax - (size_t)f2(p);

        if (row1 >= 0 && row1 < rows)
        {
            graph[row1][col] = '*';
        }
        if (row2 >= 0 && row2 < rows)
        {
            graph[row2][col] = (row1 == row2) ? '#' : 'o';
        }
    }

    printGraph((char *)graph, rows, cols);
}

int main()
{
    //cout << "start-" << endl;

    makeGraph(false);

    //cout << "end-" << endl;

    return 0;
}
