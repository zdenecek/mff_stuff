## Goal

The goal of this assignment is to evaluate your implementation of cache-oblivious
matrix transposition experimentally and to compare it with the trivial algorithm
which transposes by definition.

You are given a test program (`matrix_experiment_sim`) which evaluates your
implementation from the previous assignment on different simulated caches and
matrices of different sizes. For each experiment, the average number of cache
misses per item is reported (the diagonal items which do not move are not
counted). The program also evaluates performance of the trivial transposition algorithm.
The simulated cache is fully associative and uses LRU replacement strategy.

You should run these experiments and write a report, which contains one plot of
the measured data for each cache type, showing dependency of the average number of
misses per item on the matrix size. There should be two curves in each plot: one for your
algorithm, another for the trivial one.

The report should discuss the experimental results and try to explain the observed
behavior (including any strange anomalies) using theory from the lectures.
If you want, you can carry out further experiments to gain better understanding
of the algorithm and include these in the report.

You should submit a PDF file with the report (and no source code).
You will get 1 temporary point upon submission if the file is syntactically correct;
proper points will be assigned later.

## Test program

The test program is given two arguments:
- Cache type:
    - `m1024-b16` – cache of 1024 items organized in 16-item blocks
    - `m8192-b64` – cache of 8192 items organized in 64-item blocks
    - `m65536-b256` – cache of 65536 items organized on 256-item blocks
    - `m65536-b4096` – cache of 65536 items organized in 4096-item blocks
- The implementation to test (`smart` or `naive`).

The output of the program contains one line per experiment, which consists of
the matrix size and the average number of cache misses per item.

*Warning:* The Python tests are slow, even though they use only a subset of the
matrix sizes. They can take about one hour to complete.
If your machine has multiple processors or cores, you can try `make -j`
to run the tests in parallel.

## Optional: Tests on real hardware (for 5 extra points)

You can also test your transposition algorithm on real hardware
using the `matrix_experiment_real` program. The matrix is stored in row-major
order, each item takes 4 bytes.

The program takes one parameter, the implementation to test: `smart` or `naive`.
Its output contains one line per experiment, which consists of the matrix size
and the average time per item in nanoseconds.

However, the program is available only for C++, because general slowness of
Python completely hides all cache effects.

Again, your report should show a plot of the measured data and discuss the observed
effects. You should also include the configuration of caches in your machine.
(If you are using Linux, you can try the `machinfo` script from
[this repository](https://gitlab.kam.mff.cuni.cz/mj/aim.git).)

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
- **Is the graph distorted by compression artifacts?** (No, you shouldn't use JPEG for plots!)

In your discussion, please distinguish the following kinds of claims.
It should be always clear which is which:
- Experimental results (i.e., the raw data you obtained from the experiments)
- Theoretical facts (i.e., claims we have proved mathematically)
- Your hypotheses (e.g., when you claim that the graph looks like something is true,
  but you are not able to prove rigorously that it always holds)

Source code templates can be found in [git](https://gitlab.kam.mff.cuni.cz/datovky/assignments/-/tree/master).
