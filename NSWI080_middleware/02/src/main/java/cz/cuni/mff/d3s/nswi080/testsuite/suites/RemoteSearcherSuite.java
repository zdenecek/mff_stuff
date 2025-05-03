package cz.cuni.mff.d3s.nswi080.testsuite.suites;

import cz.cuni.mff.d3s.nswi080.graph.Graph;
import cz.cuni.mff.d3s.nswi080.graph.Searcher;
import cz.cuni.mff.d3s.nswi080.graph.SearcherImpl;
import cz.cuni.mff.d3s.nswi080.testsuite.TestSuite;

import java.util.Random;
import java.rmi.RemoteException;
import java.rmi.Naming;

public class RemoteSearcherSuite implements TestSuite {
    private final String rmiUrl;

    public RemoteSearcherSuite(String rmiUrl) {
        this.rmiUrl = rmiUrl;
    }

     @Override
    public Graph createGraph(int nodeCount, int edgeCount, Random random) {
        var graph = new Graph(nodeCount);
        try {
            graph.addRandomEdges(edgeCount, random);
        } catch (Exception e) {
            throw new RuntimeException("Failed to create graph", e);
        }
        return graph;
    }

    @Override
    public Searcher createSearcher() {
        try {
            return (Searcher) Naming.lookup(this.rmiUrl +"/RemoteSearcher");
        } catch (Exception e) {
            throw new RuntimeException("Failed to lookup remote searcher", e);
        }
    }
}
