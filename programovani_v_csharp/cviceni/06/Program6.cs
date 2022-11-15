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

        Frequencies frequencies;
        HuffmanTree tree;


        try
        {
            using (var fr = File.Open(args[0], FileMode.Open, FileAccess.Read))
            using (var fw = File.Open(args[0] + ".huff", FileMode.Create, FileAccess.Write))
            using (var r = new BinaryReader(fr))
            using (var b = new BitWriter(fw))
            {
                frequencies = CreateFrequencies(r);
                tree = new HuffmanTree(frequencies);
                fr.Seek(0, SeekOrigin.Begin); // Reset filestream to start
                var serializer = new HuffmanSerializer(b);
                serializer.SerializeTree(tree, r);
                b.Close();
            }
        }
        catch (Exception e)
        when (e is FileNotFoundException or UnauthorizedAccessException or FormatException)
        {
            Console.WriteLine("File Error");
            return;
        }

        
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


