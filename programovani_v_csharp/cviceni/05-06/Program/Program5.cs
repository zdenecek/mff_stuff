using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Text;

namespace task5_huffman;

using Frequencies = List<(byte, ulong)>;

public class Program5
{
    public static void Main5(string[] args)
    {
        if (args.Count() != 1)
        {
            Console.WriteLine("Argument Error");
            return;
        }

        Frequencies frequencies;

        try
        {
            using (var f = File.Open(args[0], FileMode.Open, FileAccess.Read))
            using (var r = new BinaryReader(f))
            {
                frequencies = CreateFrequencies(r);
            }
        }
        catch (Exception e)
        when (e is FileNotFoundException or UnauthorizedAccessException or FormatException)
        {
            Console.WriteLine("File Error");
            return;
        }

        var tree = new HuffmanTree(frequencies);
        Console.WriteLine(tree);
    }

    public static Frequencies CreateFrequencies(BinaryReader b)
    {
        ulong[] frequencies = new ulong[byte.MaxValue + 1];
        byte current_byte;

        for(; ; )
        {
            try {
                current_byte =  b.ReadByte();
            } catch (EndOfStreamException)
            {
                break;
            }
            frequencies[current_byte]++;
        }

        var list = new Frequencies();
        for(int i = 0; i <= byte.MaxValue; ++i)
        {
            list.Add(((byte)i, frequencies[i]));
        }

        return list;
    }

}


