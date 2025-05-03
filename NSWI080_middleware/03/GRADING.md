# Grading

- Offers: OK; 2.5 points
    - - In Java, you typically wouldn't synchronize on an "outside" object like a map, which has methods of its own: either synchronize on `this` or create a private object just for synchronization. `synchronize (this)` would be sufficient.
- Account balance: OK; 2.5 points
    - Note there is no need to synchronize in the bank. There is just one session - one thread.
- Basic sale protocol: OK; 2.5 points
- Robust sale protocol: OK; 2.5 points

Total **10** points.

# Hash

The repository hash used in this grading is 2813c6f5269289e5e2846dd7e5725b23ad076a82.
