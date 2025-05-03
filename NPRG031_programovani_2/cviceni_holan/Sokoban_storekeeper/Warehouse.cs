using System;
using System.Linq;
using System.Collections.Generic;
using System.Collections;



public class Program
{
    static void Main(string[] args)
    {
        run(10);
    }

    static void run(int dimension)
    {
   
        bool[,] board = new bool[dimension, dimension];
        Position? box = null, man = null, goal = null;

        for (int i = 0; i < dimension; i++)
        {
            var line = Console.ReadLine();
            for (var j = 0; j < dimension; j++)
            {
                switch(line[j])
                {
                    case 'X': board[i, j] = Warehouse.Obstacle;
                        break;
                    case 'S':
                        man = new Position(i,j);
                        break;
                    case 'B':
                        box = new Position(i, j);
                        break;
                    case 'C':
                        goal = new Position(i,j);
                        break;
                    case '.':
                        break;
                    default:
                        throw new Exception("Invalid input");
                }
            }
        }
        var wh = new Warehouse(board);
        var res = wh.search(goal ?? throw new Exception("No goal"), 
            man ?? throw new Exception("No warehouseman"), 
            box ?? throw new Exception("No box"));

        Console.WriteLine(res);
    }
}

public class Warehouse
{
    public static readonly bool Obstacle = true;

    bool[,] boardWithObstacles;
    IEnumerable<Position> vectors;

    bool obstacleAt(Position pos) => boardWithObstacles[pos.row, pos.col] == Obstacle;

    public Warehouse(bool[,] boardWithObstacles)
    {
        this.boardWithObstacles = boardWithObstacles;

        var vectors_ = new List<Position>(4);
        vectors_.Add(new Position(0, 1));
        vectors_.Add(new Position(1, 0));
        vectors_.Add(new Position(0, -1));
        vectors_.Add(new Position(-1, 0));

        vectors = vectors_;
    }

    IEnumerable<WarehouseState> generateNextStates(WarehouseState state)
    {
        foreach(var vector in vectors)
        {
            var newMan = state.warehousemanPosition + vector;
            var newBox = state.boxPosition;
            if (!newMan.inBounds(0, 9, 0, 9)) continue;
            if (obstacleAt(newMan)) continue;
            if (newMan == state.boxPosition)
            {
                newBox = state.boxPosition + vector;
                if (!newBox.inBounds(0, 9, 0, 9)) continue;
                if (obstacleAt(newBox)) continue;

            }
            yield return new WarehouseState(newMan, newBox);
        }
    }

    public int search(Position goal, Position man, Position box)
    {
        if (box == goal) return 0;

        var memory = new HashSet<WarehouseState>();
        var que = new Queue<Tuple<WarehouseState, int>>();

        que.Enqueue(Tuple.Create(new WarehouseState(man, box), 0));

        while (que.Count > 0)
        {

            var currentNode = que.Dequeue();
            (var currentState, var depth) = currentNode;


            foreach (var newState in generateNextStates(currentState))
            {
                if (memory.Contains(newState)) continue;
                if (newState.boxPosition == goal) return depth + 1;
                memory.Add(newState);
                que.Enqueue(Tuple.Create(newState, depth + 1));
            }
        }

        return -1;
    }

}



public struct Position
{
    public int row, col;

    public Position(int row, int col)
    {
        this.row = row;
        this.col = col;
    }

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


struct WarehouseState
{
    public Position warehousemanPosition, boxPosition;

    public WarehouseState(Position warehousemanPosition, Position boxPosition)
    {
        this.warehousemanPosition = warehousemanPosition;
        this.boxPosition = boxPosition;
    }

    public override bool Equals(object obj)
    {
        return obj is WarehouseState state &&
                this.boxPosition == ((WarehouseState) obj).boxPosition &&
                this.warehousemanPosition == ((WarehouseState)obj).warehousemanPosition;
    }

    public override int GetHashCode() => HashCode.Combine(warehousemanPosition, boxPosition);

    public override string ToString() => $"Box{boxPosition} Man{warehousemanPosition})";

}

