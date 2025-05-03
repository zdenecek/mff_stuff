using System;
using System.Linq;
using System.Collections.Generic;

namespace cs {

    public class MyMath {

        public static void PrintFactors(int num) {
            
            int divisor = 2;
            Console.Write($"{num}=");
            bool start = true;

            while(num >= 2) {
                while(num % divisor == 0) {
                    num /= divisor;
                    Console.Write( (start ? "" : "*") + $"{divisor}");
                    start = false;
                }
                divisor++;
            }
            Console.Write("\n");
        }

        public static List<int> GetFactors(int num)
        {

            int originalNum = num;
            int divisor = 2;
            List<int> factors = new List<int>();

            while (num >= 2)
            {
                while (num % divisor == 0)
                {
                    num /= divisor;
                    factors.Add(divisor);
                }
                divisor++;
            }

            return factors;
        }

        public static IEnumerable<IEnumerable<int>> Permutations(int num, int order)
        {
            return Permutations<int>(Enumerable.Range(1, num), order);
        }

        public static IEnumerable<IEnumerable<T>> Permutations<T>(IEnumerable<T> _from, int order)
        {
            if(order == 0) yield return Enumerable.Empty<T>();
            else
            {
                foreach(T element in _from)
                {
                    foreach(var p in Permutations<T>(from a in _from  where !a.Equals(element) select a, order - 1))
                    {
                        yield return p.Prepend(element);
                    }
                }
            }

        }
    }
}
