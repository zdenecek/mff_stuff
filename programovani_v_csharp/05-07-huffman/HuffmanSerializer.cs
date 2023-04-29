using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task5_huffman
{
    public class HuffmanSerializer
    {
        BitWriter bitWriter;
        public HuffmanSerializer(BitWriter where)
        {
            this.bitWriter = where;
        }

        public void SerializeTree(HuffmanTree tree, BinaryReader data)
        {
            WriteHeader();
            WriteTree(tree);
            WriteSeparator();
            WriteData(tree, data);
        }

        private void WriteHeader() => bitWriter.Write(new byte[] { 0x7B, 0x68, 0x75, 0x7C, 0x6D, 0x7D, 0x66, 0x66 });
        private void WriteSeparator()
        {
            for(int i = 0; i < 8; i++) bitWriter.WriteBits(0);
        }

        private void WriteNodeContents(Node node) {

            void writeWeight()
            {
                ulong number = node.Weight;
                ulong mask = 1;
                for (int i = 0; i < 55; i++)
                {
                    bitWriter.WriteBit(number & mask);
                    mask <<= 1;
                }
            }

            if(node is LeafNode) {
                bitWriter.WriteBit(1);
                writeWeight(); 
                bitWriter.WriteBits((node as LeafNode).key);
            }
            else if (node is InnerNode)
            {
                bitWriter.WriteBit(0);
                writeWeight();
                bitWriter.WriteBits(0);
            }
        }

        private void WriteNode(Node node)
        {
            WriteNodeContents(node);
            if (node is InnerNode)
            {
                WriteNode((node as InnerNode).left);
                WriteNode((node as InnerNode).right);
            }
        }

        private void WriteTree(HuffmanTree tree) => WriteNode(tree.Root);


        private Dictionary<byte,List<bool>> createCodes(HuffmanTree tree)
        {
            Dictionary<byte, List<bool>> dict = new();
            void workNode(Node node, List<bool> path)
            {
                if (node is LeafNode)
                {
                    dict.Add((node as LeafNode).key, new List<bool>(path));
                }
                else
                {
                    path.Add(false);
                    workNode((node as InnerNode).left, path);
                    path.RemoveAt(path.Count - 1);
                    path.Add(true);
                    workNode((node as InnerNode).right, path);
                    path.RemoveAt(path.Count - 1);

                }
            }

            workNode(tree.Root, new());
            return dict;
        }
        private void WriteData(HuffmanTree tree, BinaryReader data)
        {
            var codes = createCodes(tree);

            try
            {
                for (; ; )
                {
                    var key = data.ReadByte();
                    var code = codes[key];
                    code.ForEach(bit => this.bitWriter.WriteBit(bit));
                }

            } catch (EndOfStreamException)
            {
                // do nothing, end of file, flush will happen elsewhere
            }
        }


    }
}
