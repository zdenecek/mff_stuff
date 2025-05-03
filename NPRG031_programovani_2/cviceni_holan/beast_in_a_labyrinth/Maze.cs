using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cs
{

    public static class Suite
    {
        public static void Main(string[] args)
        {
            int cols = int.Parse(Console.ReadLine());
            int rows = int.Parse(Console.ReadLine());
            int steps = 20;
            var maze = MazeFactory.parse(rows, cols);

            for(int i = 0; i < steps; i++)
            {
                maze.doStep();
                Console.WriteLine(MazePrinter.StringifyMaze(maze));
            }
        }
    }

    public enum Tile
    {
        Obstacle,
        Free
    }

    public  static class MazeFactory
    {
        public const char free = '.', obstacle = 'X';

        public static Maze parse(int rows, int cols)
        {
            var m = new Maze(rows, cols);
            Action<int,int,Position> setBiest = (r, c, pos) => {
                m.DasBiest = new Position(r, c);
                m.BiestRotation = pos;
            };
            for(int r = 0; r < rows; r++)
            {
                var line = Console.ReadLine();
                for(int c = 0; c < cols; c++)
                {
                    switch(line[c]) {
                        case obstacle:
                            m.set(r, c, Tile.Obstacle);
                            break;
                        case '^':
                            setBiest(r, c, Position.North);
                            break;
                        case '>':
                            setBiest(r, c, Position.East);
                            break;
                        case 'v':
                            setBiest(r, c, Position.South);
                            break;
                        case '<':
                            setBiest(r, c, Position.West);
                            break;
                        default:
                            break;
                    }
                }
            }
            return m;
        }

    }

    public class Maze
    {
        private Tile[,] tiles;

        public Tile at(int row, int col) => tiles[row, col];
        public void set(int row, int col, Tile value) => tiles[row, col] = value;

        public Position DasBiest { get; set; }
        public Position BiestRotation { get; set; }

        private bool BiestMovedLastStep = false;

        public int Rows { get; private set; }
        public int Cols { get; private set; }
        public Maze(int rows, int cols)
        {
            Cols = cols;
            Rows = rows;
            tiles = new Tile[rows, cols];
            for(int i = 0; i < rows; i++)
            {
                for(var j = 0; j < cols; j++)
                {
                    tiles[i, j] = Tile.Free;
                }
            }
        }
        private bool isFree(Position position) => position.inBounds(0, this.Rows, 0, this.Cols) && this.at(position.row, position.col) == Tile.Free;

        public void doStep()
        {

            var forward = DasBiest + BiestRotation;
            var right = DasBiest + BiestRotation.rotateRight();

            if (isFree(right) && BiestMovedLastStep) {
                BiestRotation = BiestRotation.rotateRight();
                BiestMovedLastStep = false;
            } else if (isFree(forward)) {
                DasBiest += BiestRotation;
                BiestMovedLastStep = true;
            } else {
                BiestRotation = BiestRotation.rotateLeft();
                BiestMovedLastStep = false;
            }

        }

    }

    public static class MazePrinter {

        public static char free = '.', obstacle = 'X';
        public static string StringifyMaze(Maze maze)
        {
            var sb = new StringBuilder();

            for (int row = 0; row < maze.Rows; row++)
            {
                for (int col = 0; col < maze.Cols; col++)
                {
                    if (new Position(row, col) == maze.DasBiest)
                        sb.Append(rotationToString(maze.BiestRotation));
                    else
                        sb.Append(maze.at(row, col) == Tile.Free ? free : obstacle );
                }
                sb.Append('\n');
            }

            return sb.ToString();
        }

        public static char rotationToString(Position rotation)
        {
            if (rotation == Position.North) return '^';
            if (rotation == Position.East) return '>';
            if (rotation == Position.South) return 'v';
            if (rotation == Position.West) return '<';

            return '?';
        }

    }
}
