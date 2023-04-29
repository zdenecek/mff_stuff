package com.mypackage.task3;

import java.util.Arrays;

public class Program {

    public static void main(String[] args) {
        var tree = new BinaryTree<Integer>();
        try {
            Arrays.stream(args).map(Integer::parseInt).forEach(tree::add);
            tree.iterator().forEachRemaining(System.out::println);
        } catch (Exception e) {
            System.out.println("INPUT ERROR");
        }
    }
}
