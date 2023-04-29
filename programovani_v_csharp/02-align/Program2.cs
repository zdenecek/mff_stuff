
using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace task1;
class Program2
{

    static void Main(string[] args)
    {
        if (args.Length != 3)
        {
            Console.WriteLine("Argument Error");
            return;
        }

        int lineLength;
        var res = int.TryParse(args[2], out lineLength);

        if (!res || lineLength <= 0)
        {
            Console.WriteLine("Argument Error");
            return;
        }

        try
        {
            using (StreamReader sr = new StreamReader(args[0]))
            using (StreamWriter sw = new StreamWriter(args[1]))
            {
                Align(sr, sw, lineLength);
            }
        }
        catch (Exception)
        {
            Console.WriteLine("File Error");
        }
    }


    static void Align(TextReader r, TextWriter w, int lineLength)
    {
        var words = new List<string>();
        int currentWordLength = 0;

        var lw = new LineWriter(w, lineLength);

        bool paragraphStarted = false;
        bool shouldEndParagraph = false;

        foreach (string? word in WordReader.readWords(r))
        {

            if (word == null)
            {
                if (!paragraphStarted) continue;
                paragraphStarted = false;
                lw.WriteLine(words, false);
                shouldEndParagraph = true;
                words.Clear();
                currentWordLength = 0;
                continue;
            }

            paragraphStarted = true;

            if (currentWordLength + word.Length > lineLength)
            {
                if (words.Count != 0)
                {
                    if(shouldEndParagraph)
                    {
                        lw.EndParagraph();
                        shouldEndParagraph = false;
                    }
                    lw.WriteLine(words);
                    words.Clear();
                    currentWordLength = 0;
                }
            }

            currentWordLength += word.Length + 1;
            words.Add(word);
        }


        if (words.Count() > 0)
            lw.WriteLine(words, false);

    }

}