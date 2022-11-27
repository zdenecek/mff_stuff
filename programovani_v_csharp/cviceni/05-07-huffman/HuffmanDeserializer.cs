using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection.PortableExecutable;
using System.Text;
using System.Threading.Tasks;

namespace task5_huffman
{
    public class HuffmanDeserializer
    {
        BitReader bitReader;
        private readonly byte[] header = new byte[] { 0x7B, 0x68, 0x75, 0x7C, 0x6D, 0x7D, 0x66, 0x66 };
        private readonly byte[] separator = new byte[] { 0, 0, 0, 0, 0,0,0,0 };
        public HuffmanDeserializer(BitReader where)
        {
            this.bitReader = where;
        }

        public void DeserializeData(BinaryWriter whereToWrite)
        {
            if(!ReadHeader()) throw new InvalidDataException("Bad file header");
            var tree = ReadTree();
            if (!ReadSeparator()) throw new InvalidDataException("Bad separator");

            WriteData(tree, whereToWrite);
        }

        private bool ReadHeader() => ReadByteSequence(this.header);
        private bool ReadSeparator() => ReadByteSequence(this.separator);


        private bool ReadByteSequence(byte[] seq)
        {
            try
            {
                foreach (var b in seq)
                {
                    if (b != bitReader.ReadByte())

                        return false;

                }
            }
            catch (EndOfStreamException) { 
            
                return false;
            }
            return true;
        }

        private HuffmanTree ReadTree() {

            try
            {
                var root = parseNode();
                return new HuffmanTree(root);
            }
            catch (EndOfStreamException)
            {
                throw new InvalidDataException("Bad tree");
            }
        }

        /*       private readonly ulong typeMask = 1ul << 63;
               private readonly ulong weightMask = (1ul << 63) - (1ul << 8) ; // ((1ul << 63) - 1 ) - ( (1ul << 8) -1)
               private readonly ulong charMask = (1ul << 8) - 1; // 255 
        */

        private const ulong typeMask = 1;
        private const ulong weightMask = (1ul << 56) - 1 -1; // ((1ul << 63) - 1 ) - ( (1ul << 8) -1)
        private const int weightShift = 1; // 255 

        private const ulong charMask = (1ul << 63) - (1ul << 56) + (1ul << 63); // 255 
        private const int charShift = 56; // 255 

        private Node parseNode()
        {
            var nodeData = bitReader.ReadUInt64();
            var typeBit = (nodeData & typeMask) == 0 ?  0 : 1;


            if(typeBit == 1) // leafnode
                {
                var data = (byte)((nodeData & charMask) >> charShift);
                ulong weight = ((nodeData & weightMask) >> weightShift);


                return new LeafNode() { count = weight, key = data };
                }
            else // inner node
                return new InnerNode() { left = parseNode(), right= parseNode() };

        }



        private Dictionary<(int, ulong),LeafNode> createCodes(HuffmanTree tree)
        {
            Dictionary<(int, ulong), LeafNode> dict = new();
            void workNode(Node node, ulong seq, int len)
            {
                if (node is LeafNode leaf)
                {
                    dict.Add((len, seq), leaf   );
                }
                else
                {
                    len++;
                    workNode((node as InnerNode).left, (seq << 1), len);

                    workNode((node as InnerNode).right, ((seq << 1) + 1), len);

                }
            }

            workNode(tree.Root, 0, 0);
            return dict;
        }

        const bool i_hate_my_life = true;
        private void WriteData(HuffmanTree tree, BinaryWriter where)
        {

            var counter = new Dictionary<byte, ulong>();
            var codes = createCodes(tree);



            while (i_hate_my_life)
            {
                int len = 0;
                ulong seq = 0;
                int bit;    
                do
                {
                    if ((bit = bitReader.ReadBit()) == -1) break;
                    len++;
                    seq <<= 1;
                    seq += (ulong) bit;


                }
                while (!codes.ContainsKey((len, seq)));



                if(bit == -1)
                {
                    if (seq == 0) return;
                    else throw new InvalidDataException($"Data ends with invalid encoding sequence {seq}");
                }
 

                LeafNode c = codes[(len, seq)];
                if (!counter.ContainsKey(c.key)) counter[c.key] = 0;

                if(bitReader.LastByte)
                {
                    if ((counter[c.key] & 0b1111) == (c.Weight & 0b1111)) continue;
                }

                counter[c.key]++;

                where.Write(c.key);
            }

        }


    }
}
