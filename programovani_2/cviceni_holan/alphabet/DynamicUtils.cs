using System;
using System.Collections.Generic;
using System.Linq;

namespace cs
{

    public class DynamicUtils
    {
        Dictionary<char, List<Coordinates>> keyboardDict;
        string textToWrite;

        public void ShortestPathOnAKeyboard()
        {
            parseInput();
            //parseInput(3,3, "ABCBFECDFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", "ABCDEFA");
            var res = ShortestPathDynamically(textToWrite);
            Console.WriteLine(res);
        }

        private int ShortestPathDynamically(string text)
        {
            var partialSolutions = new List<(Coordinates /* ending tile */, int /* steps so far*/)>();
            partialSolutions.Add((Coordinates.Start, 0)); // so far 0 steps and ending on start
            
            foreach(char c in text)
            {
                partialSolutions = findSolutionsGivenSolutions(partialSolutions, c);
            }

            return partialSolutions.Min(x => x.Item2);
        }

        private List<(Coordinates, int)> findSolutionsGivenSolutions(List<(Coordinates, int)> solutions, char toWrite)
        {
            // if the char is not on the keyboard, we don't have to move
            if (!keyboardDict.ContainsKey(toWrite)) return solutions;

            var newSolutions = new List<(Coordinates, int)>();
            foreach(var newEnd in keyboardDict[toWrite])
            {
                int? currMin = null;
                foreach((var end, int steps) in solutions)
                {
                    var distance = Coordinates.distace(newEnd, end) + steps;
                    if (currMin == null || currMin > distance) currMin = distance;
                }
                newSolutions.Add((newEnd, currMin + 1 ?? throw new Exception()));
            }
            return newSolutions;
        }

        private void parseInput()   
        {
            parseInput(int.Parse(Console.ReadLine()), int.Parse(Console.ReadLine()),
                Console.ReadLine(), Console.ReadLine());
        }

        private void parseInput(int width, int height, string keyboard, string text = null)
        {
            keyboardDict = new Dictionary<char, List<Coordinates>>();
            textToWrite = text;

            for(int i = 0; i < height; i++)
            {
                int a = i * width;
                
                for(int j = 0; j < width; j++)
                {
                    int index = a + j;
                    if (index >= keyboard.Length) return;
                    if (!keyboardDict.ContainsKey(keyboard[index])) keyboardDict.Add(keyboard[index], new List<Coordinates>());
                    keyboardDict[keyboard[index]].Add(new Coordinates(i, j));
                }
            }
        }
    }
    
    struct Coordinates
    {
        public int row, col;

        public static Coordinates Start = new Coordinates(0,0);


        public static int distace(Coordinates a, Coordinates b)
        {
            return Math.Abs(a.col - b.col) + Math.Abs(a.row - b.row);
        }

        public Coordinates(int row, int col)
        {
            this.row = row;
            this.col = col;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(row, col);
        }

        public override string ToString()
        {
            return $"({row},{col})";
        }
    }
}