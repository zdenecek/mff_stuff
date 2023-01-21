using System;
// using System.Collections.Generic;
// using System.Text;

namespace cs
{
    class Program
    {
        static void Main(string[] args)
        {
            var input = new InputUtils(Console.In);

            //var factors = new List<int>();
            int num = input.ReadNumber();
            //int originalNum = num;
            int divisor = 2;

            Console.Write($"{num}=");
            bool start = true;

            while(num >= 2) {
                while(num % divisor == 0) {
                    num /= divisor;
                    //factors.Add(divisor);
                    Console.Write( (start ? "" : "*") + $"{divisor}");
                    start = false;
                }
                divisor++;
            }
            Console.Write("\n");

            //Console.WriteLine($"{originalNum}=" + String.Join('*',factors));

        }

    }
}
