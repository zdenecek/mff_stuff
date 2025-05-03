package com.mypackage.task7;


import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Program {

    public static void main(String[] args) {

        if(args.length == 0) {
            System.out.println("Error");
            return;
        }

        int total = 0;
        int uid = 0;
        int nologin = 0;
        int empty = 0;

        try(BufferedReader br = new BufferedReader(new FileReader(args[0]))) {

            String line = br.readLine();
            while (line != null) {
                total++;
                String[] items = line.split(":");
                if (items[2].equals("0")) uid++;
                if (items[6].equals("/sbin/nologin")) nologin++;
                if (items[4].isEmpty()) empty++;

                line = br.readLine();
            }
        }
        catch(Exception e)
        {
            System.out.println("Error");
            return;
        }

        System.out.println(String.format("Users: %d",  total));
        System.out.println(String.format("Users with UID 0: %d", uid));
        System.out.println(String.format("Users with /sbin/nologin shell: %d", nologin));
        System.out.println(String.format("Users with empty comment field: %d", empty));
    }
}
