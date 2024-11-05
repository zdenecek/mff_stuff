## Goal

The goal of this assignment is to evaluate your implementation of (a,b)-trees
experimentally and compare performance of (2,3) and (2,4)-trees.

You are given a test program (`ab_experiment`) which is used to evaluate your
implementation of the previous assignment. The test program auguments your implementation
by implementing a `remove` method and it performs the following experiments:

- _Insert test:_ Insert _n_ elements in random order.
- _Min test:_ Insert _n_ elements sequentially and then _n_ times repeat: remove the minimal
  element in the tree and then insert it back.
- _Random test:_ Insert _n_ elements sequentially and then _n_ times repeat: remove random
  element from the tree and then insert random element into the tree. Removed element is
  always present in the tree and inserted element is always *not* present in the tree.


The program tries each experiment with different values of _n_. In each try,
it prints the average number of _structural changes_ per operation. Structural change is
either a node split (in insert) or merging of two nodes (in delete).

You should perform these experiments and write a report, which contains the following
plots of the measured data. Each plot should show the dependence of the average
number of structural changes on the set size _n_.

- The insert test: one curve for (2,3) tree, one for (2,4) tree.
- The min test: one curve for (2,3) tree, one for (2,4) tree.
- The random test: one curve for (2,3) tree, one for (2,4) tree.

The report should discuss the experimental results and try to explain the observed
behavior using theory from the lectures. (If you want, you can carry out further
experiments to gain better understanding of the data structure and include these
in the report. This is strictly optional.)

You should submit a PDF file with the report (and no source code).
You will get 1 temporary point upon submission if the file is syntantically correct;
proper points will be assigned later.

## Test program

The test program is given three arguments:
- The name of the test (`insert`, `min`, `random`).
- The random seed: you should use the last 2 digits of your student ID (you can find
  it in the Study Information System – just click on the Personal data icon). Please
  include the random seed in your report.
- The type of the tree to test (`2-3` or `2-4`).

The output of the program contains one line per experiment, which consists of _n_ and the
average number of structural changes.

## Your implementation

Please use your implementation from the previous exercise. Methods `split_node(...)`
and `insert()` will be augmented by the test program. If you are performing
a node splits directly instead of using `split_node(...)` method, you
need to adjust the `BenchmarkingABTree` class accordingly.

## Hints

The following tools can be useful for producing nice plots:
- [pandas](https://pandas.pydata.org/)
- [matplotlib](https://matplotlib.org/)
- [gnuplot](http://www.gnuplot.info/)

A quick checklist for plots:
- Is there a caption explaining what is plotted?
- Are the axes clearly labelled? Do they have value ranges and units?
- Have you mentioned that this axis has logarithmic scale? (Logarithmic graphs
  are more fitting in some cases, but you should tell.)
- Is it clear which curve means what?
- Is it clear what are the measured points and what is an interpolated
  curve between them?
- Are there any overlaps? (E.g., the most interesting part of the curve
  hidden underneath a label?)

In your discussion, please distinguish the following kinds of claims.
It should be always clear which is which:
- Experimental results (i.e., the raw data you obtained from the experiments)
- Theoretical facts (i.e., claims we have proved mathematically)
- Your hypotheses (e.g., when you claim that the graph looks like something is true,
  but you are not able to prove rigorously that it always holds)


Source code templates can be found in [git](https://gitlab.kam.mff.cuni.cz/datovky/assignments/-/tree/master).
