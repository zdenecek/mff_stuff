package cz.cuni.mff.d3s.nswi080.testsuite;

import cz.cuni.mff.d3s.nswi080.graph.Graph;
import cz.cuni.mff.d3s.nswi080.graph.Node;
import cz.cuni.mff.d3s.nswi080.graph.Searcher;
import cz.cuni.mff.d3s.nswi080.testsuite.suites.RemoteSearcherSuite;

import java.util.Random;
import java.rmi.RemoteException;
import java.rmi.Naming;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import java.util.Arrays;
import java.util.stream.Collectors;

public class Client {
    private static final String USAGE = "Usage: java Client <test-suite-class-name> <node-count> <edge-counts-comma-separated> [rmi-url] [search-depth]";
    private static final int DEFAULT_NODE_COUNT = 1000;
    private static final String DEFAULT_EDGE_COUNTS = "2000,10000,50000";
    private static final int ATTEMPT_COUNT = 50;
    private static final int SEED = 42;
    private static final String DEFAULT_RMI_URL = "//localhost:1099";

    public static void main(String[] args) throws RemoteException {
        if (args.length < 1 || args.length > 5) {
            System.out.println(USAGE);
            System.exit(1);
        }

        String testSuiteClassName = args[0];
        int nodeCount = args.length > 1 ? Integer.parseInt(args[1]) : DEFAULT_NODE_COUNT;
        List<Integer> edgeCounts = args.length > 2 
            ? Arrays.stream(args[2].split(","))
                   .map(String::trim)
                   .map(Integer::parseInt)
                   .collect(Collectors.toList())
            : Arrays.stream(DEFAULT_EDGE_COUNTS.split(","))
                   .map(String::trim)
                   .map(Integer::parseInt)
                   .collect(Collectors.toList());
        String rmiUrl = args.length > 3 ? args[3] : DEFAULT_RMI_URL;
        int search_depth = args.length > 4 ? Integer.parseInt(args[4]) : 4;

        try {
            // Load the test suite class
            Class<?> testSuiteClass = Class.forName(testSuiteClassName);
            if (!TestSuite.class.isAssignableFrom(testSuiteClass)) {
                System.err.println("Error: Class " + testSuiteClassName + " does not implement TestSuite");
                System.exit(1);
            }

            // Try to create instance with RMI URL constructor first
            TestSuite testSuite;
            try {
                testSuite = (TestSuite) testSuiteClass.getConstructor(String.class).newInstance(rmiUrl);
            } catch (NoSuchMethodException e) {
                // Fall back to default constructor if String constructor not found
                testSuite = (TestSuite) testSuiteClass.getDeclaredConstructor().newInstance();
            }

            // Create searcher
            Searcher searcher = testSuite.createSearcher();

            // Open CSV file for all results
            try (PrintWriter writer = new PrintWriter(System.out)) {
                // Write CSV header
                writer.println("Nodes,Edges,Attempt,Distance,Time,TransitiveTime");

                // Run benchmarks for different edge counts
                for (int edgeCount : edgeCounts) {
                    System.err.printf("Running benchmark with %d nodes and %d edges...%n", nodeCount, edgeCount);
                    // Create new random generator for each graph to ensure consistency
                    Random random = new Random(SEED);
                    Graph graph = testSuite.createGraph(nodeCount, edgeCount, random);
                    
                    // Run benchmark and write to CSV
                    runBenchmark(graph, searcher, nodeCount, edgeCount, ATTEMPT_COUNT, random, writer, search_depth);
                }
            }

        } catch (ClassNotFoundException e) {
            System.err.println("Error: Test suite class not found: " + testSuiteClassName);
            System.exit(1);
        } catch (Exception e) {
            System.err.println("Error running test suite: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }

    /**
     * Run a single benchmark with given parameters and write results to the CSV file.
     */
    private static void runBenchmark(Graph graph, Searcher searcher, int nodeCount, int edgeCount, int attemptCount, Random random, 
    PrintWriter writer, int search_depth) 
    throws RemoteException {
        for (int i = 0; i < attemptCount; i++) {
            // Select two random nodes
            int idxFrom = random.nextInt(graph.getNodeCount());
            int idxTo = random.nextInt(graph.getNodeCount());

            // Calculate distance, measure operation time
            long startTimeNs = System.nanoTime();
            int distance = searcher.getDistance(graph.getNode(idxFrom), graph.getNode(idxTo));
            long durationNs = System.nanoTime() - startTimeNs;

            // Calculate transitive distance, measure operation time
            long startTimeTransitiveNs = System.nanoTime();
            int distanceTransitive = searcher.getDistanceTransitive(search_depth, graph.getNode(idxFrom), graph.getNode(idxTo));
            long durationTransitiveNs = System.nanoTime() - startTimeTransitiveNs;

            if (distance != distanceTransitive) {
                System.err.printf("Standard and transitive algorithms inconsistent (%d != %d)%n", distance, distanceTransitive);
            }

            // Write results to CSV
            writer.printf("%d,%d,%d,%d,%d,%d%n",
                nodeCount, edgeCount, i, distance, durationNs / 1000, durationTransitiveNs / 1000);
        }
        writer.flush(); // Ensure data is written after each benchmark
    }
}
