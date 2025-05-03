package cz.cuni.mff.d3s.nswi080.testsuite.suites;

import cz.cuni.mff.d3s.nswi080.graph.Graph;
import cz.cuni.mff.d3s.nswi080.graph.Searcher;
import cz.cuni.mff.d3s.nswi080.graph.SearcherImpl;
import cz.cuni.mff.d3s.nswi080.testsuite.TestSuite;

import java.util.Random;

public class LocalTestSuite implements TestSuite {


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
        return new SearcherImpl();
    }
}
