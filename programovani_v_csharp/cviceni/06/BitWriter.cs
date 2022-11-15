using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task5_huffman;

public class BitWriter : BinaryWriter, IDisposable
{
	private byte buffer = 0;
	// bit write order 8765 4321
	private byte mask = 1;

	public bool written = false;

	public BitWriter(FileStream fs) : base(fs)
	{

	}

	public void WriteBits(byte value)
	{
		byte mask = 1;
		while(mask != 0)
		{
			WriteBit(value & mask);
			mask <<= 1;
		}
	}

	public void WriteBit(int value) => WriteBit(value != 0);
	public void WriteBit(ulong value) => WriteBit(value != 0);

    public void WriteBit(bool value)
	{
		written = true;
		if(value) 
			buffer |= mask;

		mask <<= 1;

		if (mask == 0) FlushBuffer();
	} 
	
	private void FlushBuffer()
	{
		if (!written) return;
		base.Write(buffer);
        buffer = 0;
        mask = 1;
		written = false;
    }

    public override void Close()
	{
        FlushBuffer();

		base.Close();
	}

}
