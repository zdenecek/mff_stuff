package com.mypackage;

import java.util.stream.IntStream;
import java.util.stream.Stream;

public class Task01 {
    public static void main(String[] args) {
        //noinspection RedundantStringFormatCall
        IntStream.of(Integer.parseInt(args[0])).forEach(a -> Stream.of(a == 10 ? "%2d * %d = %3d" : "%2d * %d = %2d").forEach(format -> IntStream.range(1, 11).forEach(i -> System.out.println(String.format(format, i, a, a * i)))));
    }
}