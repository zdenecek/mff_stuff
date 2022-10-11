
using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace task1;
class Program
{

    static void Main(string[] args)
    {
        if (args.Length != 1)
        {
            Console.WriteLine("Argument Error");
            return;
        }

        try
        {
            using (StreamReader sr = new StreamReader(args[0]))
            {
                // Console.WriteLine(CountWords(sr));

                var list = CreateFrequencies(sr);
                foreach( (var key, var value) in list)
                {
                    Console.WriteLine($"{key}: {value}");
                }
            }
        }
        catch (Exception)
        {
            Console.WriteLine("File Error");
        }


    }

    static int CountWords(TextReader s)
    {
        int count = 0;

        bool word = false;

        Func<char, bool> isWhiteSpace = ch => ch == ' ' || ch == '\n' || ch == '\t';

        while (s.Peek() >= 0)
        {
            if (!isWhiteSpace((char)s.Read())) word = true;
            else if (word)
            {
                word = false;
                count++;
            }
        }

        if (word) count++;

        return count;
    }


    static SortedDictionary<string, int> CreateFrequencies(TextReader s) {
        
        var dict = new SortedDictionary<string, int>();
        Action<string> add = s => { 
            if(dict.ContainsKey(s)) dict[s]++;
            else dict[s] = 1;
        };

        bool word = false;
        var currentWord = new StringBuilder();
        Func<char, bool> isWhiteSpace = ch => ch == ' ' || ch == '\n' || ch == '\t';

        while (s.Peek() >= 0)
        {
            var ch = (char)s.Read();
            if (!isWhiteSpace(ch))
            {
                word = true;
                currentWord.Append(ch);
            }
            else if (word)
            {
                word = false;
                add(currentWord.ToString());
                currentWord.Clear();
            }
        }

        if(word) add(currentWord.ToString());

        return dict;

    }

}