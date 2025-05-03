using System;
using System.Collections.Generic;
using System.Linq;

namespace cs
{

    public class Chess
    {
        const bool obstacle = true;

        public static void Main(string[] args)
        {
            run();
        }

        public static void run()
        {
            Func<int> read = new InputUtils(Console.In).ReadNumber;

            // Init board with walls around
            int dimension = 8;
            
            bool[,] boardWithObstacles = new bool[dimension, dimension];


            Position? start = null;
            Position? end = null;

            for (int row = 0; row < dimension; row++)
            {
                var line = Console.ReadLine();
                for (int col = 0; col < dimension; col++)
                {
                    switch(line[col])
                    {
                        case 'x':
                            boardWithObstacles[row, col] = obstacle;
                            break;
                        case 'v':
                            start = new Position(row, col);
                            break;
                        case 'c':
                            end = new Position(row, col);
                            break;
                        default:
                            break;
                    }
                }
            }

            var result = search(boardWithObstacles, start ?? throw new Exception("No start"), end ?? throw new Exception("No end"));

            Console.WriteLine(result);
        }

        static IEnumerable<Position> getNewPositions(Position p, Func<Position, bool> obstacleAt)
        {
            Position[] vectors = { new Position(0, 1),
                            new Position(0, -1),
                            new Position(1, 0),
                            new Position(-1, 0)}; 

            var positions = new List<Position>();

            foreach(var vector in vectors)
            {
                var newPos = p + vector;
                while(newPos.inBounds(0,7,0,7) && !obstacleAt(newPos))
                {
                    positions.Add(newPos);
                    newPos += vector;
                }
            }

            return positions;
        }

        static int search(bool[,] boardWithObstacles, Position start, Position end)
        {
            Func<Position, bool> obstacleAt = pos => boardWithObstacles[pos.row, pos.col] == obstacle;

            var memory = new HashSet<Position>();
            var que = new Queue<Tuple<Position, int>>();
            
            Position[,] history = new Position[boardWithObstacles.GetLength(0), boardWithObstacles.GetLength(1)];

            que.Enqueue(Tuple.Create(start, 0));

    


            while (que.Count > 0)
            {

                var currentNode = que.Dequeue();
                var currentPosition = currentNode.Item1;
                var depth = currentNode.Item2;

                if (currentPosition == end) {
                    return depth;
                }

                var newPositions = getNewPositions(currentPosition, obstacleAt);

                foreach (var newPosition in newPositions)
                {
                   

                    if (memory.Contains(newPosition)) continue;
                    memory.Add(newPosition);
                    que.Enqueue(Tuple.Create(newPosition, depth + 1));
                }
            }

            return -1;
        }

    }

}