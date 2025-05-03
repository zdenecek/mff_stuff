package cz.cuni.mff.d3s.nswi080.testsuite.suites;

import cz.cuni.mff.d3s.nswi080.graph.Graph;
import cz.cuni.mff.d3s.nswi080.graph.Searcher;
import cz.cuni.mff.d3s.nswi080.graph.SearcherImpl;
import cz.cuni.mff.d3s.nswi080.testsuite.TestSuite;
import cz.cuni.mff.d3s.nswi080.graph.NodeFactory;
import cz.cuni.mff.d3s.nswi080.graph.Node;

import java.util.Random;
import java.rmi.RemoteException;
import java.rmi.Naming;

public class BothRemoteSuite implements TestSuite {
    private final String rmiUrl;

    public BothRemoteSuite(String rmiUrl) {
        this.rmiUrl = rmiUrl;
    }

    @Override
    public Graph createGraph(int nodeCount, int edgeCount, Random random) {
         try {
        NodeFactory nodeFactory = (NodeFactory) Naming.lookup(this.rmiUrl + "/NodeFactory");
        Node[] nodes = new Node[nodeCount];
        for (int i = 0; i < nodeCount; i++) {
            nodes[i] = nodeFactory.createNode();
        }
        Graph graph = new Graph(nodes);
        graph.addRandomEdges(edgeCount, random);
        return graph;
        } catch (Exception e) {
            throw new RuntimeException("Failed to create graph", e);
        }
    }

    @Override
    public Searcher createSearcher() {
        try {
            return (Searcher) Naming.lookup(this.rmiUrl + "/RemoteSearcher");
        } catch (Exception e) {
            throw new RuntimeException("Failed to lookup remote searcher", e);
        }
    }
}
