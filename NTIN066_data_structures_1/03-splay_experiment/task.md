## Goal

The goal of this assignment is to evaluate your implementation of Splay trees
experimentally and to compare it with a "naive" implementation which splays
using single rotations only.

You are given a test program (`splay_experiment`) which calls your
implementation from the previous assignment to perform the following
experiments:

- _Sequential test:_ Insert _n_ elements sequentially and then repeatedly
  find them all in sequential order.
- _Random test:_ Insert _n_ elements in random order and then find _5n_
  random elements.
- _Subset test:_ Insert a sequence of _n_ elements, which contains arithmetic
  progressions interspersed with random elements. Then repeatedly access
  a small subset of these elements in random order. Try this with subsets of
  different cardinalities.

The program tries each experiment with different values of _n_. In each try,
it prints the average number of rotations per splay operation.

You should perform these experiments and write a report, which contains the following
plots of the measured data. Each plot should show the dependence of the average
number of rotations on the set size _n_.

- The sequential test: one curve for the standard implementation, one for the naive one.
- The random test: one curve for the standard implementation, one for the naive one.
- The subset test: three curves for the standard implementation with different sizes
  of the subset, three for the naive implementation with the same sizes.

The report should discuss the experimental results and try to explain the observed
behavior using theory from the lectures. (If you want, you can carry out further
experiments to gain better understanding of the data structure and include these
in the report. This is strictly optional.)

You should submit a PDF file with the report (and no source code).
You will get 1 temporary point upon submission if the file is syntantically correct;
proper points will be assigned later.

## Test program

The test program is given three arguments:
- The name of the test (`sequential`, `random`, `subset`).
- The random seed: you should use the last 2 digits of your student ID (you can find
  it in the Study Information System – just click on the Personal data icon). Please
  include the random seed in your report.
- The implementation to test (`std` or `naive`).

The output of the program contains one line per experiment, which consists of:
- For the sequential and random test: the set size and the average number of rotations.
- For the subset test: the subset size, the set size, and the average number of rotations
  per find. The initial insertions of the full set are not counted.

## Your implementation

Please use your implementation from the previous exercise. Methods `splay()`
and `rotate()` will be augmented by the test program. If you are performing
a double rotation directly instead of composing it from single rotations, you
need to adjust the `BenchmarkingTree` class accordingly.

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
