package com.mypackage.task4;

abstract class Token {
}

class WordToken extends Token {
    private String word;

    public WordToken(String word) {
        this.word = word;
    }

    public String getWord() {
        return word;
    }
}

class EndOfStreamToken extends Token {
}

class EndOfParagraphToken extends Token {
}


