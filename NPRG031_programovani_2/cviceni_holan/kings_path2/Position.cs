using System;

namespace cs
{
    public struct Position
    {
        public int row, col;

        public static Position Start = new Position(0, 0);

        public Position(int row, int col)
        {
            this.row = row;
            this.col = col;
        }

        public static int distance(Position a, Position b) => Math.Abs(a.col - b.col) + Math.Abs(a.row - b.row);


        public bool inBounds(int minRow, int maxRow, int minCol, int maxCol) =>
            row >= minRow && row <= maxRow && col >= minCol && col <= maxCol;

        public override string ToString() => $"({row}, {col})";

        public override bool Equals(object obj)
        {
            return obj is Position position &&
                   row == position.row &&
                   col == position.col;
        }

        public override int GetHashCode() => HashCode.Combine(row, col);
        public static bool operator ==(Position a, Position b) => a.row == b.row && a.col == b.col;
        public static bool operator !=(Position a, Position b) => a.row != b.row || a.col != b.col;
        public static Position operator +(Position a, Position b) => new Position(a.row + b.row, a.col + b.col);

    }
}
