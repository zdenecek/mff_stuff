using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace task3;


public class WordReader : TokenReader
{
    private TextReader tr;
    private bool responsibleForClosing = false;
    public WordReader(string path)
    {
        tr = new StreamReader(path);
        responsibleForClosing = true;
    }

    public WordReader(TextReader reader)
    {
        tr = reader;
    }

    public void Dispose()
    {
      if(responsibleForClosing) tr.Dispose();
    }

    public IEnumerable<Token> Read()
    {
        bool wordState = false;
        var currentWord = new StringBuilder();
        Func<char, bool> isNotWhiteSpace = ch => !(ch == ' ' || ch == '\n' || ch == '\t');
        bool paragraphEnded = false;
        bool readNewLine = false;

        while (tr.Peek() >= 0)
        {
            var ch = (char)tr.Read();
            if (isNotWhiteSpace(ch))
            {
                readNewLine = false;
                wordState = true;
                currentWord.Append(ch);
                if (paragraphEnded)
                {
                    yield return new ParagraphBreakToken();
                    paragraphEnded = false;
                }
            }
            else if (wordState)
            {
                wordState = false;
                yield return new WordToken(currentWord.ToString());
                currentWord.Clear();
            }
            if (ch == '\n')
            {
                if (readNewLine) paragraphEnded = true;
                readNewLine = true;
            }
        }

        if (wordState) yield  return new WordToken(currentWord.ToString());
    }

}