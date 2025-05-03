package cz.cuni.mff.d3s.nswi080.testsuite;

import cz.cuni.mff.d3s.nswi080.graph.Graph;
import cz.cuni.mff.d3s.nswi080.graph.Searcher;

import java.util.Random;
import java.rmi.RemoteException;

public interface TestSuite {
    
    abstract Graph createGraph(int nodeCount, int edgeCount, Random random) throws RemoteException;

    abstract Searcher createSearcher() throws RemoteException; 

}
