using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task5_huffman;

using Frequencies = List<(byte, ulong)>;

public class HuffmanTree
{
    public Node Root { get; private set; }

    public HuffmanTree(Node root)
    {
        Root = root;
    }

    public HuffmanTree(Frequencies frequencies)
    {
        IEnumerable<Node> a = frequencies
            .Where(e => e.Item2 > 0)
            .Select(e => new LeafNode { key = e.Item1, count = e.Item2 })
            .OrderBy(e => e.count);

        IEnumerable<InnerNode> b = new List<InnerNode>();

        Func<Node> selectMin = () =>
        {
            if (b.Count() != 0 && (a.Count() == 0 || b.Min(e => e.weight) < a.First().Weight))
            {
                var m = b.Min(e => e.weight);
                var r1 = b.Where(b => b.Weight == m).First();
                b = b.Skip(1);
                return r1;
            }
            var r = a.First();
            a = a.Skip(1);
            return r;
        };

        int c = a.Count() - 1;

        for (int i = 0; i < c; i++)
        {
            var left = selectMin();
            var right = selectMin();
            b = b.Append(new InnerNode { left = left, right = right, weight = left.Weight + right.Weight });
        }

        Root = b.First();
    }

    public override string ToString()
    {
        return Root.ToString();
    }

}

public abstract record Node
{
    public abstract ulong Weight { get; }
}
public record LeafNode : Node
{
    public byte key;
    public ulong count;

    public override ulong Weight => count;
    public override string ToString()
    {
        return $"*{key}:{count}";
    }

}

public record InnerNode : Node
{
    public ulong weight;
    public Node left, right;

    public override ulong Weight => weight;

    public override string ToString()
    {
        return $"{weight} {left} {right}";
    }
}


