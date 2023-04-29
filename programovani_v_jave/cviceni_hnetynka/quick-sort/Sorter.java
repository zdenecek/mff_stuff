package com.mypackage.task5;

import java.util.Random;

public class Sorter {

    private static Random r = new Random(42);

    public static void QuickSort(int[] array) {
        qsort(array, 0, array.length - 1);
    }

    private static void qsort(int[] array, int start, int end) {
        if (start >= end) return; // done
        int pivot = r.nextInt(start, end);

        int index = makePivotedArray(array, pivot, start, end);
        qsort(array, start, index - 1);
        qsort(array, index + 1, end);
    }

    private static int makePivotedArray(int[] array , int pivot, int start, int end) {
        int piv = array[pivot];
        array[pivot] = array[end];
        array[end] = piv;

        int firstGreater = start;
        for (int i = start; i < end; ++i) {
            if (array[i] <= piv) {
                if(i != firstGreater - 1) {
                int c = array[firstGreater];
                array[firstGreater] = array[i];
                array[i] = c;
            }
                firstGreater++;
        }
        }
        array[end] = array[firstGreater];;
        array[firstGreater] = piv;
        return firstGreater;
    }

}
