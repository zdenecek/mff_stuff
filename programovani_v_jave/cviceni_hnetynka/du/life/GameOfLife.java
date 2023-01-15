package com.mypackage.life;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class GameOfLife {

    // matrix to store the current state of the cells
    private char[][] current;
    // matrix to store the next state of the cells
    private char[][] next;
    // number of steps
    private int steps;
    // number of threads
    private int numThreads;
    // width and height of the matrix
    private int n;

    ExecutorService executor;


    public GameOfLife(int n, int steps, int numThreads) {
        this.n = n;
        this.steps = steps;
        this.numThreads = numThreads;
        current = new char[n][n];
        next = new char[n][n];
        executor = Executors.newFixedThreadPool(numThreads);
    }

    // method to read the input from the standard input
    public void readInput(BufferedReader scanner) throws IOException {

        for (int i = 0; i < n; i++) {
            current[i] = scanner.readLine().toCharArray();
        }
    }

    // method to compute the next state of a cell based on its current state and the states of its neighbors
    private char computeNextState(int i, int j) {
        int liveNeighbors = 0;

        int right = j + 1 == n ? 0 : j + 1;
        int left = j == 0 ?  n -1: j - 1;
        int top = i == 0 ? n -1: i - 1;
        int bottom = i + 1 == n ? 0 : i + 1;

        // check left neighbor
        if (current[i][left] == 'X') {
            liveNeighbors++;
        }
        // check right neighbor
        if ( current[i][right] == 'X') {
            liveNeighbors++;
        }
        // check top neighbor
        if ( current[top][j] == 'X') {
            liveNeighbors++;
        }
        // check bottom neighbor
        if ( current[bottom][j] == 'X') {
            liveNeighbors++;
        }
        // check top-left neighbor
        if ( current[top][left] == 'X') {
            liveNeighbors++;
        }
        // check top-right neighbor
        if ( current[bottom][left] == 'X') {
            liveNeighbors++;
        }
        // check bottom-left neighbor
        if ( current[top][right] == 'X') {
            liveNeighbors++;
        }
        // check bottom-right neighbor
        if (current[bottom][right] == 'X') {
            liveNeighbors++;
        }
        // apply the rules of the Game of Life to compute the next state of the cell
        if (current[i][j] == 'X') {
            if (liveNeighbors < 2 || liveNeighbors > 3) {
                return '_';
            }
            return 'X';
        }

        if (liveNeighbors == 3) {
            return 'X';
        } else {
        return '_';
        }
    }

    // method to compute the next state of the cells in parallel using executors
    public void computeNextState() {
        CountDownLatch latch = new CountDownLatch(n);
        for (int i = 0; i < n; i++) {
            int finalI = i;
            executor.execute(() -> {
                for (int j = 0; j < n; j++) {


                    next[finalI][j] = computeNextState(finalI, j);
                }

                latch.countDown();
            });


        }
        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void end(){
        this.executor.shutdown();
        this.printState();
    }

    // method to update the current state with the next state and reset the next state
    public void updateState() {

        char[] temp;

        for (int i = 0; i < n; i++) {

            temp = current[i];
            current[i] = next[i];
            next[i] = temp;
        }
    }

    // method to print the current state of the cells
    public void printState() {
        for (int i = 0; i < n; i++) {
            System.out.println(current[i]);
        }
    }

    public static void main(String[] args) throws IOException {
        var scanner = new BufferedReader(new InputStreamReader(System.in));

        var first = scanner.readLine().split(" ");

        int n = Integer.parseInt(first[0]);
        int steps =  Integer.parseInt(first[1]);
        int numThreads = Runtime.getRuntime().availableProcessors();
        GameOfLife game = new GameOfLife(n, steps, numThreads);
        game.readInput(scanner);

        for (int i = 0; i < steps; i++) {

            game.computeNextState();
            game.updateState();
        }
        game.end();
    }
}



