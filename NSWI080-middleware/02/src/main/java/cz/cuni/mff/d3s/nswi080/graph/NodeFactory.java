package cz.cuni.mff.d3s.nswi080.graph;


import java.rmi.Remote;
import java.rmi.RemoteException;

public interface NodeFactory extends Remote {
   /**
    * Creates a new node.
    * @return a new node
    * @throws RemoteException if a remote error occurs
    */
   Node createNode() throws RemoteException;
}
