# Grading

- Local measurements: OK; 1.5 points
- Remote searcher: OK; 1.5 points
- Remote nodes: OK; 1.5 points
- Remote nodes and searcher: 1 point
    - Re answer: there is no such thing as a "RMI Loopback Optimization". There are regular calls over TCP that happen to not leave the machine.
- Passing by value/reference: 1.5 points
    - There should be no network effect with the remote searcher variant because the nodes are anyway serialized at once.
- Network impact: OK; 2 points
    - What about the transitive variant...? You should try it with remote nodes, which could work better than non-transitive remote nodes.
    - In the both remote variant/two machines, the network latency should not affect each node operation - the nodes and searcher are both on the server.

Total **9** points.

# Hash

The repository hash used in this grading is 1840445456ff2285f4f2487cfeae1e1a3cd7bead.
