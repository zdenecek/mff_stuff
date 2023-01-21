using System;
using System.Linq;

namespace cs
{
    class Program
    {
        static void Main(string[] args) {

            var cupCount = 3;
            var input = from str in Console.ReadLine().Split() select int.Parse(str);
            //var input = from str in  "8 4 3 4 2 3".Split(' ') select int.Parse(str);
            var cups = input
                .Take(cupCount)
                .Zip(input.TakeLast(cupCount), (capacity, initial) => new WaterCup(capacity, initial));

            var results = WaterUtils.bfs(cups);

            Console.WriteLine(string.Join(' ', results.OrderBy(x => x.Key).Select(x => $"{x.Key}:{x.Value}")));
        }

        
    }
}
