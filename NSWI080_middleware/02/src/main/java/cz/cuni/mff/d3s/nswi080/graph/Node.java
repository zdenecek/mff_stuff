package cz.cuni.mff.d3s.nswi080.graph;

import java.util.Map;
import java.util.Set;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.io.Serializable;

/**
 * A node in a directed graph.
 */
public interface Node extends Remote, Serializable {
    /**
     * Gets the set of direct successors of this node. A node V is a direct
     * successor if there is an edge from this node to V.
     */
    Set<Node> getDirectSuccessors() throws RemoteException;

    /**
     * Gets the set of transitively reachable nodes from this node, up to the specified distance.
     *
     * @param distance the maximum distance of the nodes to be returned
     * @return a map of nodes to their distances from this node
     */
    Map<Node, Integer> getTransitiveSuccessors(int distance) throws RemoteException;

    /**
     * Adds a direct successor to this node.
     *
     * @param successor the direct successor to be added
     */
    void addDirectSuccessor(Node successor) throws RemoteException;
}
