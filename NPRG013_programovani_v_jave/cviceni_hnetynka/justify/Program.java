package com.mypackage.task4;

import java.io.*;
import java.util.Scanner;

public class Program {


    public static void main(String[] args)
    {
        var in = new BufferedReader(new InputStreamReader(System.in));
        int lineLength;
        try {
            lineLength = Integer.parseInt(in.readLine());
            if (lineLength <= 0) throw new IllegalArgumentException();

        } catch (Exception e) {
            System.out.println("Error");
            return;
        }


        var justifiedWriter = new JustifiedWriter( System.out, lineLength);
        try {
            justifiedWriter.WriteJustified(in);
            in.close();
        } catch (IOException e) {
            System.out.println("Error");
        }
    }
}
