package com.mypackage.task4;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.Scanner;

public class WordReader {

    private BufferedReader r;
    private boolean wordState = false;
    private boolean paragraphEnded = false;
    private boolean readNewLine = false;
    private StringBuilder currentWord = new StringBuilder();


    WordReader(BufferedReader r) {
        this.r = r;
    }

    public Token getNextToken() throws IOException {

        int c;
        while ((c = r.read()) != -1) {
            char ch = (char) c;

            if (ch == '\n') {
                if (readNewLine) paragraphEnded = true;
                readNewLine = true;
            }


            if (!Character.isWhitespace(ch)) {
                readNewLine = false;
                wordState = true;
                currentWord.append(ch);
                if (paragraphEnded) {
                    paragraphEnded = false;
                    return new EndOfParagraphToken();
                }
            } else if (wordState) {
                wordState = false;
                var word = currentWord.toString();
                currentWord = new StringBuilder();
                return new WordToken(word);
            }
        }

        if (wordState) {
            wordState = false;
            return new WordToken(currentWord.toString());
        }

        return new EndOfStreamToken();

    }
}