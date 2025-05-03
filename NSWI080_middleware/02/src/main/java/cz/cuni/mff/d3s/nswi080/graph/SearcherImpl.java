package cz.cuni.mff.d3s.nswi080.graph;

import java.rmi.RemoteException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

public class SearcherImpl implements Searcher {
    /**
     * {@inheritDoc}
     *
     * @implNote Starting from the source node, then a set of visited nodes
     * is always extended by immediate successors of all visited nodes,
     * until the target node is visited or no node is left.
     */
    @Override
    public int getDistance(Node from, Node to) throws RemoteException {
        // visited keeps the nodes visited in past steps.
        Set<Node> visited = new HashSet<>();
        // boundary keeps the nodes visited in current step.
        Set<Node> boundary = new HashSet<>();
        int distance = 0;
        // We start from the source node.
        boundary.add(from);
        // Traverse the graph until finding the target node.
        while (!boundary.contains(to)) {
            // Not having anything to visit means the target node cannot be reached.
            if (boundary.isEmpty()) {
                return Searcher.DISTANCE_INFINITE;
            }
            Set<Node> traversing = new HashSet<>();
            // Nodes visited in current step become nodes visited in past steps.
            visited.addAll(boundary);
            // Collect a set of immediate successors of nodes visited in current step.
            for (Node node : boundary) {
                traversing.addAll(node.getDirectSuccessors());
            }
            // Out of immediate successors, consider only those not yet visited.
            traversing.removeIf(visited::contains);
            // Make these nodes the new nodes to be visited in current step.
            boundary = traversing;
            distance++;
        }
        return distance;
    }

    /**
     * {@inheritDoc}
     *
     * @implNote Starting from the source node, a set of visited nodes
     * is always extended by transitive successors of all visited
     * nodes, until the target node is visited or no node is left.
     */
    @Override
    public int getDistanceTransitive(int successorDistance, Node from, Node to) throws RemoteException {
        if (from.equals(to)) {
            return 0;
        }
        // visited keeps the nodes visited in past steps.
        Set<Node> visited = new HashSet<>();
        // boundary keeps the nodes visited in current step.
        Map<Node, Integer> boundary = new HashMap<>();
        // We start from the source node.
        boundary.put(from, 0);
        // Traverse the graph until finding the target node or having an empty boundary.
        while (!boundary.isEmpty()) {
            Map<Node, Integer> traversing = new HashMap<>();
            // Collect transitive successors of nodes visited in current step
            for (Entry<Node, Integer> currentTuple : boundary.entrySet()) {
                Node currentNode = currentTuple.getKey();
                int currentDistance = currentTuple.getValue();
                if (visited.contains(currentNode)) {
                    continue;
                }
                Map<Node, Integer> partialGraph = currentNode.getTransitiveSuccessors(successorDistance);
                // Store the distance of each transitive successor
                for (Entry<Node, Integer> searchedTuple : partialGraph.entrySet()) {
                    final Node searchedNode = searchedTuple.getKey();
                    final int newDistance = currentDistance + searchedTuple.getValue();

                    Integer oldDistance = traversing.get(searchedNode);
                    if (oldDistance == null || newDistance < oldDistance)
                        traversing.put(searchedNode, newDistance);
                }
                // Nodes visited in current step become nodes visited in past steps
                visited.add(currentNode);
            }
            // Check if the distance to the destination has been computed
            Integer distance = traversing.get(to);
            if (distance != null) {
                return distance;
            }
            boundary = traversing;
        }
        // Not having anything to visit means the target node cannot be reached.
        return Searcher.DISTANCE_INFINITE;
    }
}
