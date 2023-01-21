using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace cs
{
    class Program
    {
        const bool obstacle = true;
        static void Main(string[] args) {


            Func<int> read = new InputUtils(Console.In).ReadNumber;

            // Init board with walls around
            bool[,] boardWithObstacles = new bool[8 + 2, 8 + 2];
            for (int row = 0; row < 10; row++)
            {
                for (int col = 0; col < 10; col++)
                {
                    boardWithObstacles[row, col] = (row == 0 || col == 0 || row == 9 || col == 9) ? obstacle : !obstacle;
                }
            }

            // Read obstacles
            var obstacleCount = read();
            for (int i = 0; i < obstacleCount; i++)
            {
                boardWithObstacles[read(), read()] = obstacle;
            }

            var start = new Position(read(), read());
            var end = new Position(read(), read());

            var result = search(boardWithObstacles, start, end);

            Console.WriteLine(result);
        }

        static int search(bool[,] boardWithObstacles, Position start, Position end)
        {
            if (end == start) return 0;

            Func<Position, bool> obstacleAt = pos => boardWithObstacles[pos.row, pos.col] == obstacle;

            var memory = new HashSet<Position>();
            var que = new Queue<Tuple<Position, int>>();

            que.Enqueue(Tuple.Create(start, 0));

            var vectors = from row in (new[] { -1, 0, 1 })
                          from col in (new[] { -1, 0, 1 })
                          where row != 0 || col != 0
                          select new Position(row, col);

            while (que.Count > 0)
            {

                var currentNode = que.Dequeue();
                var currentPosition = currentNode.Item1;
                var depth = currentNode.Item2;

                // Console.WriteLine($"Exploring {currentPosition} at depth {depth}");

                foreach(var vector in vectors)
                {
                    var newPosition = currentPosition + vector;

                    if (obstacleAt(newPosition)) continue;
                    if (memory.Contains(newPosition)) continue;
                    if (newPosition == end) return depth + 1;
                    memory.Add(newPosition);
                    que.Enqueue(Tuple.Create(newPosition, depth + 1));
                }
            }

            return -1;
        }

        struct Position
        {
            public int row, col;

            public Position(int row, int col)
            {
                this.row = row;
                this.col = col;
            }

            public override string ToString() => $"({row}, {col})";
            public static bool operator ==(Position a, Position b) => a.row == b.row && a.col == b.col;
            public static bool operator !=(Position a, Position b) => a.row != b.row || a.col != b.col;
            public static Position operator +(Position a, Position b) => new Position(a.row + b.row , a.col + b.col);

        }
    }
}
