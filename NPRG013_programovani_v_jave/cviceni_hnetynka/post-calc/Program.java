package com.mypackage.task6;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Stack;

public class Program {

    public static void main(String[] args) {

        var inp = new InputStreamReader(System.in);
        var r = new BufferedReader(inp);


        r.lines().forEach(s -> parseExpr(s));

        try {
            r.close();
            inp.close();
        } catch (IOException e) {

        }

    }

    public static void parseExpr(String e) {

        if(e.length() == 0) return;

        var stack = new Stack<Integer>();

        boolean error = false;
        for (String token : e.split("\s+")) {
            if(token.length() == 0) continue;
             if(token.equals("+")){
                 if(stack.size() < 2){
                     error = true;
                     break;
                 }
                var r = stack.pop()+stack.pop();
                stack.push(r);
            }
            else if(token.equals("-")){
                 if(stack.size() < 2){
                     error = true;
                     break;
                 }
                 var x = stack.pop();
                 var y = stack.pop();

                 var r = y-x;
                stack.push(r);
            }
            else if(token.equals("*")){
                 if(stack.size() < 2){
                     error = true;
                     break;
                 }
                var r = stack.pop()*stack.pop();
                stack.push(r);
            }
            else if(token.equals("/")){
                 if(stack.size() < 2){
                     error = true;
                     break;
                 }
                var x = stack.pop();
                var y = stack.pop();
                if(x == 0) {
                    System.out.println("Zero division");
                    return;
                }
                var r = y/x;
                stack.push(r);
            }
            else {
                try {
                    stack.push(Integer.parseInt(token));
                } catch (Exception ex) {
                        error = true;
                        break;
                }
             }
        }
        if(stack.size() != 1 || error){
            System.out.println("Malformed expression");
            return;
        }
        int a = stack.pop();
        System.out.println(a);
    }
}
