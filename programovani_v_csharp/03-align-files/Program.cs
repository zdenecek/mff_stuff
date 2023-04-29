using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection.PortableExecutable;

namespace task3;

public class Program
{
    public static void Main(string[] args)
    {
        bool highlightLines; int lineLength; string outputPath; IEnumerable<string> inputPaths;
        try
        {
            ParseArgs(args, out highlightLines, out lineLength, out outputPath, out inputPaths);
        }
        catch (ArgumentException)
        {
            Console.WriteLine("Argument Error");
            return;
        }

        try
        {
            using (var writer = new StreamWriter(outputPath))
            using (var fw = new MyFileReader(inputPaths, path => new WordReader(path)))
            {
                var jw = JustifiedWriterFactory.MakeJustifiedWriter(writer, lineLength, highlightLines);
                jw.WriteJustified(fw);
            }
        }
        catch (Exception e) when (e is FileNotFoundException || e is UnauthorizedAccessException || e is FormatException)
        {
            Console.WriteLine("File Error");
        }
    }


    static void ParseArgs(IEnumerable<string> args, out bool highlightLines, out int lineLength, out string outputPath, out IEnumerable<string> inputPaths)
    {
        if (args.Count() < 3) throw new ArgumentException();

        var res = int.TryParse(args.Last(), out lineLength);
        if (!res || lineLength <= 0) throw new ArgumentException();
        args = args.SkipLast(1);

        outputPath = args.Last();
        args = args.SkipLast(1);

        if (args.First() == "--highlight-spaces")
        {
            highlightLines = true;
            args = args.Skip(1);
        }
        else highlightLines = false;

        if (args.Count() == 0) throw new ArgumentException();
        inputPaths = args;

    }
}