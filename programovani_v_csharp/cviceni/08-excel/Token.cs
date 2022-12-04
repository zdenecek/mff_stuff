using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task3a;

public abstract record Token
{
}


public record EndOfLineToken : Token
{

}

public record WordToken : Token
{
    public readonly string Word;

    public WordToken(string value)
    {
        Word = value;
    }
}


