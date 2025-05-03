package cz.cuni.mff.d3s.nswi080.graph;

import java.rmi.RemoteException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.io.Serializable;
public class NodeImpl implements Node, Serializable {
    private final Set<Node> successors = new HashSet<>();

    @Override
    public Set<Node> getDirectSuccessors() throws RemoteException {
        return successors;
    }

    @Override
    public Map<Node, Integer> getTransitiveSuccessors(int distance) throws RemoteException {
        if (distance <= 0) {
            throw new IllegalArgumentException("Argument distance must be positive");
        }
        Map<Node, Integer> nodeToDistance = new HashMap<>();
        Set<Node> currentLayer = new HashSet<>();
        // Initial node at zero-distance
        currentLayer.add(this);
        // Closure  for each level of i-distant nodes
        for (int i = 0; i < distance; ++i) {
            Set<Node> nextLayer = new HashSet<>();
            // Use nodes which are in the current level
            for (Node node : currentLayer) {
                if (!nodeToDistance.containsKey(node)) {
                    nodeToDistance.put(node, i);
                    nextLayer.addAll(node.getDirectSuccessors());
                }
            }
            // Move to the next layer
            currentLayer = nextLayer;
        }
        // Handle the last layer
        for (Node node : currentLayer) {
            if (!nodeToDistance.containsKey(node)) {
                nodeToDistance.put(node, distance);
            }
        }
        return nodeToDistance;
    }

    @Override
    public void addDirectSuccessor(Node successor) throws RemoteException {
        successors.add(successor);
    }
}
