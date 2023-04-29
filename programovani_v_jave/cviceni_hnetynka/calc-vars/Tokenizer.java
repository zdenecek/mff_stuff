package com.mypackage.calcVars;

import java.util.ArrayList;
import java.util.regex.Pattern;

class Tokenizer {

    public static boolean isVariable(String token) {
        return Pattern.matches("^[a-zA-Z]*$", token);
    }

    public static boolean isNumber(String token) {
        try {
            Double.parseDouble(token);
            return true;
        } catch (NumberFormatException ex) {
            return false;
        }
    }

    public static boolean isOperator(String token) {
        return token.equals("+") || token.equals("-") ||
                token.equals("*") || token.equals("/");
    }

    public ArrayList<String> tokenize(String line) {
        StringBuilder partials = new StringBuilder();
        ArrayList<String> tokens = new ArrayList<>();
        for (int i = 0; i < line.length(); i++) {
            char c = line.charAt(i);
            if (Character.isDigit(c)) {
                while ((i < line.length()) && ((Character.isDigit(line.charAt(i))) || (line.charAt(i) == 'e') | (line.charAt(i) == '.'))) {
                    partials.append(line.charAt(i));

                    if (line.charAt(i) == 'e') {
                        if (line.charAt(i + 1) == '-') {
                            partials.append('-');
                            i++;
                        } else if (line.charAt(i + 1) == '+') {
                            partials.append('+');
                            i++;
                        }
                    }
                    i++;
                }
                i--;
                tokens.add(partials.toString());
                partials.setLength(0);

            } else if (isOperator(Character.toString(c)) || (line.charAt(i) == Calculator.ASGN) || (line.charAt(i) == Calculator.LPAR) || (line.charAt(i) == Calculator.RPAR)) {
                tokens.add(Character.toString(c));
            } else if (Character.isLetter(c)) {

                while ((i < line.length()) && (Character.isLetter(line.charAt(i)))) {
                    partials.append(line.charAt(i));
                    i++;
                }
                i--;
                tokens.add(partials.toString());
                partials.setLength(0);
            }
        }

        ArrayList<String> realTokens = new ArrayList<>();
        for (int i = 0; i < tokens.size(); i++) {
            String symbol = tokens.get(i);
            if ((symbol.equals("-")) && (((i > 0) && (!isNumber(tokens.get(i - 1))) && (!isVariable(tokens.get(i - 1))) && (!tokens.get(i - 1).equals(")"))) || ((i == 0) && ((tokens.size() > 1) &&
                    ((isNumber(tokens.get(i + 1))) || isVariable(tokens.get(i + 1))))))) {

                realTokens.add(symbol + tokens.get(i + 1));
                i++;

            } else realTokens.add(symbol);
        }

        return realTokens;
    }

}
