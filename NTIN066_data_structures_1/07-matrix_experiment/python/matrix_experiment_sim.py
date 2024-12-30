#!/usr/bin/env python3
import sys

from matrix_tests import TestMatrix

def simulated_test(M, B, naive):
    for e in range(10, 25):
        N = int(2 ** (e/2))
        print("    ", N, M, B, file=sys.stderr)
        m = TestMatrix(N, M, B, 0)
        m.fill_matrix()
        m.reset_stats()
        if naive:
            m.naive_transpose()
        else:
            m.transpose()
        misses_per_item = m.stat_cache_misses / (N*(N-1))
        print(N, misses_per_item, flush=True)
        m.check_result()

tests = {
#                                                M     B
    "m1024-b16":    lambda n: simulated_test( 1024,   16, n),
    "m8192-b64":    lambda n: simulated_test( 8192,   64, n),
    "m65536-b256":  lambda n: simulated_test(65536,  256, n),
    "m65536-b4096": lambda n: simulated_test(65536, 4096, n),
}

if len(sys.argv) == 3:
    test = sys.argv[1]
    if sys.argv[2] == "smart":
        naive = False
    elif sys.argv[2] == "naive":
        naive = True
    else:
        raise ValueError("Last argument must be either 'smart' or 'naive'")
    if test in tests:
        tests[test](naive)
    else:
        raise ValueError("Unknown test {}".format(test))
else:
    raise ValueError("Usage: {} <test> (smart|naive)".format(sys.argv[0]))
