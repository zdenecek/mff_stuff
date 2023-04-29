using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace task3;

public class JustifiedWriter
{
    private TextWriter tw;
    private int lineLength;
    private char spaceChar;
    private string? appendage;
    public JustifiedWriter(TextWriter tw, int lineLength, char spaceChar = ' ', string? appendage = null)
	{
        this.tw = tw;
        this.lineLength = lineLength;
        this.spaceChar = spaceChar;
        this.appendage = appendage;
    }


    /**
     * Writes the text justified to the current line lenght.
     */
    public void WriteJustified(TokenReader reader)
    {

        var words = new List<string>();
        int wordsCummulativeLength = 0;

        foreach (Token token in reader.Read())
        {

            if (token is ParagraphBreakToken)
            {
                WriteLine(words, false);
                EndParagraph();
                words.Clear();
                wordsCummulativeLength = 0;
                continue;
            }
            else if (token is WordToken)
            {
                var word = ((WordToken)token).Word;
                if (wordsCummulativeLength + word.Length > lineLength)
                {
                    if (words.Count != 0)
                    {
                        WriteLine(words);
                        words.Clear();
                        wordsCummulativeLength = 0;
                    }
                }

                wordsCummulativeLength += word.Length + 1;
                words.Add(word);
            }
            // ignore file break
        }


        if (words.Count() > 0)
            WriteLine(words, false);

    }

    public void WriteLine(List<string> words, bool justify = true) {
        if (justify && words.Count > 1)
        {
            writeLineJustified(words);
        }
        else
            writeLineNotJustified(words);
    }   

    private void writeLineJustified(List<string> words)
    {
        int spaces = lineLength - words.Select(word => word.Length).Sum();
        int spaceRegions = (words.Count - 1);
        int spacesPerBlock = spaces / spaceRegions;
        int extraSpaces = spaces % spaceRegions;
        string space = string.Concat(Enumerable.Repeat(spaceChar, spacesPerBlock));
        string extraSpace = space + spaceChar;

        foreach (var word in words)
        {
            tw.Write(word);
            if (spaceRegions > 0)
            {
                spaceRegions--;
                if(extraSpaces > 0)
                {
                    extraSpaces--;
                    tw.Write(extraSpace);
                }
                else
                    tw.Write(space);
            }
        }

        writeNewLine();

    }

    private void writeLineNotJustified(List<string> words)
    {
        tw.Write(String.Join(spaceChar, words));
        writeNewLine();
    }

    private void writeNewLine()
    {
        if (appendage is not null) tw.Write(appendage);
        tw.Write('\n');
    }
    public void EndParagraph() => writeNewLine();
}
