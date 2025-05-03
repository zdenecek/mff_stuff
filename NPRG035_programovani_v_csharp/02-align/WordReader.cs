using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

class WordReader
{


    public static IEnumerable<string?> readWords(TextReader tr)
    {
        bool word = false;
        var currentWord = new StringBuilder();
        Func<char, bool> isWhiteSpace = ch => ch == ' ' || ch == '\n' || ch == '\t';

        bool readNewLine = false;

        while (tr.Peek() >= 0)
        {
            var ch = (char)tr.Read();
            if (!isWhiteSpace(ch))
            {
                word = true;
                currentWord.Append(ch);
            }
            else if (word)
            {
                word = false;
                readNewLine = false;
                yield return currentWord.ToString();
                currentWord.Clear();
            }

            if(ch == '\n')
            {
                if(readNewLine)
                {
                    readNewLine = false;
                    yield return null;
                }
                else 
                    readNewLine = true;
            }
        }

        if (word) yield return currentWord.ToString();
    }
}