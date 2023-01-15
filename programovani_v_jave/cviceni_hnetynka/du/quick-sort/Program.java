package com.mypackage.task5;

import java.util.Arrays;

public class Program {

    public static void main(String[] args) {


        int[] ar;
        try{
            ar = Arrays.stream(args).mapToInt(a -> Integer.parseInt(a)).toArray();
        } catch (Exception e){
            System.out.println("Input error");
            return;
        }
        Sorter.QuickSort(ar);
        Arrays.stream(ar).forEach(a -> System.out.println(a));
    }
}
