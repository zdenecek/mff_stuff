package cz.cuni.mff.d3s.nswi080.testsuite;

import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

import cz.cuni.mff.d3s.nswi080.graph.NodeFactory;
import cz.cuni.mff.d3s.nswi080.graph.Searcher;
import cz.cuni.mff.d3s.nswi080.graph.SearcherImpl;
import cz.cuni.mff.d3s.nswi080.testsuite.NodeFactoryImpl;

public class Server {
    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(1099);
            
            Searcher searcher = new SearcherImpl();
            Searcher remoteSearcher = (Searcher) UnicastRemoteObject.exportObject(searcher, 0);
            Naming.rebind("rmi://localhost/RemoteSearcher", remoteSearcher);

            NodeFactory nodeFactory = new NodeFactoryImpl();
            NodeFactory remoteNodeFactory = (NodeFactory) UnicastRemoteObject.exportObject(nodeFactory, 0);

            Naming.rebind("rmi://localhost/NodeFactory", remoteNodeFactory);

            System.out.println("Server started successfully ;)");
            System.out.println("Press Ctlr-C to exit.");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
