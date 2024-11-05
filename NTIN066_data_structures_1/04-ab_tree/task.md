You are given a representation of _(a, b)-tree_ with a `find` operation,
and a representation of an _(a, b)-tree node_.

Your goal is to implement an `insert` operation, which inserts the given
key in the tree (or does nothing if the key is already present). Preferably, 
you should also implement `split_node` method and use it properly in 
your `insert` implementation.

The implementation uses the variant of (a,b)-trees from lecture notes by [Martin Mares,
Chapter 3](http://mj.ucw.cz/vyuka/dsnotes/03-abtree.pdf) where the actual values are
stored also in the internal nodes of the tree and not only in leaves.

You should submit the `ab_tree.*` file (but not `ab_tree_test.*` files).

Source code templates can be found in [git](https://gitlab.kam.mff.cuni.cz/datovky/assignments/-/tree/master).
