package cz.cuni.mff.d3s.nswi080.testsuite;

import cz.cuni.mff.d3s.nswi080.graph.Node;
import cz.cuni.mff.d3s.nswi080.graph.NodeImpl;
import cz.cuni.mff.d3s.nswi080.graph.NodeFactory;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
public class NodeFactoryImpl  implements NodeFactory {
    public NodeFactoryImpl() throws RemoteException {
        super();
    }

    @Override
    public Node createNode() throws RemoteException {
        var node = (Node) UnicastRemoteObject.exportObject( new NodeImpl(), 0);
        return node;
    }
}