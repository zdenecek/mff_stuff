package cz.cuni.mff.d3s.nswi080.graph;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Searcher extends Remote {
    /**
     * Value returned from {@link #getDistance(Node, Node)} when there is no path between the two nodes.
     */
    int DISTANCE_INFINITE = -1;

    /**
     * Computes the distance between the source node and the target node.
     *
     * @param from the source node
     * @param to   the target node
     * @return the distance between the nodes, or {@link #DISTANCE_INFINITE} if there is no path between them
     */
    int getDistance(Node from, Node to) throws RemoteException;

    /**
     * Computes the distance between the source node and the target node. Uses an algorithm that retrieves nodes
     * transitively reachable from the source node up to the specified distance.
     *
     * @param successorDistance the maximum distance of the nodes to be returned
     * @param from              the source node
     * @param to                the target node
     * @return the distance between the nodes, or {@link #DISTANCE_INFINITE} if there is no path between them
     */
    int getDistanceTransitive(int successorDistance, Node from, Node to) throws RemoteException;
}
