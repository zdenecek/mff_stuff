package cz.cuni.mff.d3s.nswi080.graph;

import java.util.Random;
import java.rmi.Remote;
import java.rmi.RemoteException;
/**
 * A directed graph.
 */
public class Graph implements Remote {
    /**
     * Nodes in the graph.
     */
    private final Node[] nodes;

    /**
     * Creates an empty graph with the specified number of nodes.
     *
     * @param nodeCount number of nodes
     */
    public Graph(int nodeCount) {
        nodes = new Node[nodeCount];
        for (int i = 0; i < nodeCount; i++) {
            nodes[i] = new NodeImpl();
        }
    }

    /**
     * Creates a graph with the specified nodes.
     *
     * @param nodes nodes in the graph
     */
    public Graph(Node[] nodes) {
        this.nodes = nodes;
    }

    /**
     * Gets the number of nodes in the graph.
     */
    public int getNodeCount() {
        return nodes.length;
    }

    /**
     * Gets the node at the specified index.
     */
    public Node getNode(int index) {
        return nodes[index];
    }

    /**
     * Adds random edges to the graph.
     *
     * @param edgeCount number of edges
     * @param random    random number generator
     */
    public void addRandomEdges(int edgeCount, Random random) throws RemoteException {
        for (int i = 0; i < edgeCount; i++) {
            int from = random.nextInt(nodes.length);
            int to = random.nextInt(nodes.length);
            nodes[from].addDirectSuccessor(nodes[to]);
        }
    }
}
