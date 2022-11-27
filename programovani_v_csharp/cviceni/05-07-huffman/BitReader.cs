using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task5_huffman;

public class BitReader : BinaryReader
{
	private byte buffer = 0;
	private byte mask = 0;
	private readonly byte initialMask = 1;

	public bool LastByte { get; private set; } = false;
	private byte? nextByte = null;

	public BitReader(FileStream fs) : base(fs)
	{

	}

	private bool fillBuffer()
	{
		if( LastByte ) return false;

		if (nextByte == null) // initialization
		{
			try
			{
				nextByte = this.ReadByte();
			}
			catch (EndOfStreamException)
			{
				this.LastByte= true;
				return false;
			}
		}
        this.buffer = nextByte.Value;
		mask = initialMask;

		
        try
		{
            this.nextByte = this.ReadByte();
        }
		catch (EndOfStreamException)
		{
			this.LastByte = true;
		}
		return true;
	}

	public int ReadBit()
	{
		if(mask == 0)
		{
			if (!fillBuffer()) return -1;
		}
		var ret = (mask & buffer) > 0 ? 1 : 0;
		mask <<= 1;
		return ret;
	}


}
