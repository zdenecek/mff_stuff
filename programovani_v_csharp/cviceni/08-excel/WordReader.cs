using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace task3a;


public class WordReader 
{
    private TextReader tr;

    public WordReader(TextReader reader)
    {
        tr = reader;
    }

    public void Dispose()
    {
        ;
    }

    public IEnumerable<Token> Read()
    {
        bool wordState = false;
        var currentWord = new StringBuilder();
        Func<char, bool> isNotWhiteSpace = ch => !(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');

        while (tr.Peek() >= 0)
        {
            var ch = (char)tr.Read();
            if (isNotWhiteSpace(ch))
            {
                wordState = true;
                currentWord.Append(ch);
            }
            else if (wordState)
            {
                wordState = false;
                yield return new WordToken(currentWord.ToString());
                currentWord.Clear();
            }

            if (ch == '\n')
            {
                yield return new EndOfLineToken();
            }
        }

        if (wordState) yield  return new WordToken(currentWord.ToString());
    }

}