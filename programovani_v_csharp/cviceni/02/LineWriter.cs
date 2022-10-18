using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class LineWriter
{
    private TextWriter tw;
    private int lineLength;

    public LineWriter(TextWriter tw, int lineLength)
	{
        this.tw = tw;
        this.lineLength = lineLength;
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
        int spaces = this.lineLength - words.Select(word => word.Length).Sum();

        int spaceRegions = (words.Count - 1);
        int spacesPerBlock = spaces / spaceRegions;
        int extraSpaces = spaces % spaceRegions;
        string space = string.Concat(Enumerable.Repeat(" ", spacesPerBlock));
        string extraSpace = string.Concat(Enumerable.Repeat(" ", spacesPerBlock + 1));

        foreach (var word in words)
        {
            tw.Write(word);
            if (spaceRegions > 0)
            {
                if(extraSpaces > 0)
                {
                    extraSpaces--;
                    tw.Write(extraSpace);
                }
                else
                {
                    tw.Write(space);
                }
                spaceRegions--;
            }
        }

        writeNewLine();

    }

    private void writeLineNotJustified(List<string> words)
    {
        tw.Write(String.Join(' ', words));
        writeNewLine();
    }

    private void writeNewLine() => tw.Write('\n');
    public void EndParagraph() => writeNewLine();
}
