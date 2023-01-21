using System;
using System.Collections.Generic;
using System.Linq;

namespace cs
{

    public class Chess
    {
        const bool obstacle = false;

        public static void Main(string[] args)
        {
            run();
        }

        public static void run()
        {
            Func<int> read = new InputUtils(Console.In).ReadNumber;

            // Init board with walls around
            int dimension = 8 + 2;
            
            bool[,] boardWithObstacles = new bool[dimension, dimension];

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


            if (result == null) Console.WriteLine("-1");
            else
            {
                foreach(var node in result)
                {
                    Console.WriteLine($"{node.row} {node.col}");
                }
            }
        }

        static List<Position> search(bool[,] boardWithObstacles, Position start, Position end)
        {
            Func<Position, bool> obstacleAt = pos => boardWithObstacles[pos.row, pos.col] == obstacle;

            var memory = new HashSet<Position>();
            var que = new Queue<Tuple<Position, int>>();
            
            Position[,] history = new Position[boardWithObstacles.GetLength(0), boardWithObstacles.GetLength(1)];

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

                if (currentPosition == end) { 
                    var res = new List<Position>();
                    while (currentPosition != start)
                    {
                        res.Add(currentPosition);
                        currentPosition = history[currentPosition.row, currentPosition.col];
                    }
                    res.Add(start);
                    res.Reverse();
                    return res;
                }

                foreach (var vector in vectors)
                {
                    var newPosition = currentPosition + vector;

                    if (obstacleAt(newPosition)) continue;
                    if (memory.Contains(newPosition)) continue;
                    history[newPosition.row, newPosition.col] = currentPosition;
                    memory.Add(newPosition);
                    que.Enqueue(Tuple.Create(newPosition, depth + 1));
                }
            }

            return null;
        }

    }

}