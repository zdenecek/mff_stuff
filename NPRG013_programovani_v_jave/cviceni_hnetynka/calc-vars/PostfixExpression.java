package com.mypackage.calcVars;

import java.util.ArrayList;
import java.util.Map;
import java.util.Stack;

class PostfixExpression {
    public ArrayList<String> tokens;

    public PostfixExpression(ArrayList<String> expression) {
        this.tokens = expression;
    }

    public static int getOperatorPriority(String operator) {
        return switch (operator) {
            case "+", "-" -> 1;
            case "*", "/" -> 2;
            default -> -1;
        };
    }

    public static PostfixExpression FromInfix(ArrayList<String> expression) {
        ArrayList<String> postfixExpression = new ArrayList<>();
        Stack<String> stack = new Stack<>();
        for (String token : expression) {

            if (Tokenizer.isOperator(token)) {
                while (!stack.isEmpty() && getOperatorPriority(stack.peek()) >= getOperatorPriority(token)) {
                    postfixExpression.add(stack.pop());
                }
                stack.push(token);
            } else if (token.equals(")")) {
                String s = stack.pop();
                while (!s.equals("(")) {
                    postfixExpression.add(s);
                    s = stack.pop();
                }
            } else if (token.equals("(")) {
                stack.push(token);
            } else { // number
                postfixExpression.add(token);
            }
        }
        int size = stack.size();
        for (int i = 0; i < size; i++) {
            postfixExpression.add(stack.pop());
        }

        return new PostfixExpression(postfixExpression);
    }

    public void evaluate( Map<String, Double> context) {

        var success = _evaluate( context);

        if (!success) {
            System.out.println("ERROR");
            context.put("last", 0d);
        }
    }

    private boolean _evaluate( Map<String, Double> context) {
        Stack<Double> numberStack = new Stack<>();
        String assignVariable = "";
        int i = 0;


        if (this.tokens.size() >= 2 && this.tokens.get(1).equals("=")) {
            assignVariable = this.tokens.get(0);
            if (!Tokenizer.isVariable(assignVariable))
                return false;
            i = 2;
        }
        for (; i < this.tokens.size(); i++) {
            String token = this.tokens.get(i);
            if (Tokenizer.isNumber(token)) {
                numberStack.push(Double.parseDouble(token));
            } else if (Tokenizer.isVariable(token)) {
                numberStack.push(context.containsKey(token) ? context.get(token) : (double) 0);
            } else if (Tokenizer.isOperator(token)) {
                if (numberStack.isEmpty()) return false;
                double num;
                num = numberStack.pop();
                if (numberStack.isEmpty()) { // unary minus
                    if (!token.equals("-")) return false;

                    numberStack.push(-num);
                    continue;
                }
                double num2 = numberStack.pop();
                switch (token) {
                    case "+" -> numberStack.push(num2 + num);
                    case "-" -> numberStack.push(num2 - num);
                    case "*" -> numberStack.push(num2 * num);
                    case "/" -> {
                        if (num == 0) {
                            return false;
                        }
                        numberStack.push(num2 / num);
                    }
                }
            } else {
                return false;
            }
        }

        if (numberStack.size() > 1)
            return false;

        double result = numberStack.pop();
        System.out.printf("%.5f%n", result);

        if (!assignVariable.isEmpty())
            context.put(assignVariable, result);

        context.put("last", result);
        return true;

    }
}
