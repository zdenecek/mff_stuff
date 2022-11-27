using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task3;

public abstract record Token
{
}


public record FileBreakToken : Token
{

}

public record ParagraphBreakToken : Token
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


