package com.mypackage.task4;

import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

public class JustifiedWriter {

    private PrintStream tw;
    private int lineLength;

    public JustifiedWriter(PrintStream tw, int lineLength)
    {
        this.tw = tw;
        this.lineLength = lineLength;
    }


    /**
     * Writes the text justified to the current line lenght.
     */
    public void WriteJustified(BufferedReader r) throws IOException {
        var words = new ArrayList<String>();
        int wordsCummulativeLength = 0;

        WordReader wr = new WordReader(r);
        Token t = wr.getNextToken();

        while (true)
        {
            if(t  instanceof  EndOfStreamToken) break;
            else if (t instanceof  EndOfParagraphToken)
            {
                WriteLine(words, false);
                EndParagraph();
                words.clear();
                wordsCummulativeLength = 0;
            }
            else if (t instanceof  WordToken) {
                var word = ((WordToken) t).getWord();

                if (wordsCummulativeLength + word.length() > lineLength)
                {
                    if (words.size() != 0)
                    {
                        WriteLine(words, true);
                        words.clear();
                        wordsCummulativeLength = 0;
                    }
                }

                wordsCummulativeLength += word.length() + 1;
                words.add(word);

            }

            t = wr.getNextToken();
        }


        if (words.size() > 0)
            WriteLine(words, false);

    }

    public void WriteLine(ArrayList<String> words, boolean justify) {
        if (justify && words.size() > 1)
        {
            writeLineJustified(words);
        }
        else
            writeLineNotJustified(words);
    }

    private void writeLineJustified(ArrayList<String> words)
    {

        int spaces = this.lineLength - words.stream().mapToInt( word -> word.length()).sum();
        int spaceRegions = (words.size() - 1);
        int spacesPerBlock = spaces / spaceRegions;
        int extraSpaces = spaces % spaceRegions;
        String space = new String(new char[spacesPerBlock]).replace("\0", " ");
        String extraSpace = space + ' ';

        for (var word : words)
        {
            tw.print(word);
            if (spaceRegions > 0)
            {
                spaceRegions--;
                if(extraSpaces > 0)
                {
                    extraSpaces--;
                    tw.print(extraSpace);
                }
                else
                    tw.print(space);
            }
        }

        writeNewLine();

    }

    private void writeLineNotJustified(ArrayList<String> words)
    {
        tw.print(String.join(" ", words));
        writeNewLine();
    }

    private void writeNewLine()  {
        tw.print('\n');
    }
    public void EndParagraph() { writeNewLine(); }



}
