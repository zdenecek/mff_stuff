package com.mypackage.calcVars;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

public class Calculator {

    final static char ASGN = '=';
    final static char LPAR = '(';
    final static char RPAR = ')';
    private static final Map<String, Double> variables = new HashMap<>();

   static {
        variables.put("last", 0d);
    }

    public static void main(String[] args) throws IOException {

        var input = new BufferedReader(new InputStreamReader(System.in));
        var tokenizer = new Tokenizer();

        String line;
        line = input.readLine();
        while (line != null) {
            if (line.isBlank()) {
                line = input.readLine();
                continue;
            }
            var tokens = tokenizer.tokenize(line);
            var expression = PostfixExpression.FromInfix(tokens);
            expression.evaluate(variables);
            line = input.readLine();
        }
    }


}