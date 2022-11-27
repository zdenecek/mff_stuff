using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Text;

namespace task5_huffman;

using Frequencies = List<(byte, ulong)>;

public class Program
{
    public static void Main(string[] args)
    {
        if (args.Count() != 1)
        {
            Console.WriteLine("Argument Error");
            return;
        }

        var infile = args[0];

        if(!infile.EndsWith(".huff")  || infile == ".huff") {

            Console.WriteLine("Argument Error");
            return;
        }

        var outfile = infile.Substring(0, infile.Length - 5);

        try
        {
            using (var fr = File.Open(infile, FileMode.Open, FileAccess.Read))
            using (var fw = File.Create(outfile))
            using (var r = new BitReader(fr))
            using (var b = new BinaryWriter(fw))
            {

                var deserialize = new HuffmanDeserializer(r);

                deserialize.DeserializeData(b);

              
            }
        }
        catch (Exception e)
        when (e is UnauthorizedAccessException or FormatException or InvalidDataException or Exception or FileNotFoundException)
        {
            Console.WriteLine("File Error");
            return;
        }

        
    }

}


