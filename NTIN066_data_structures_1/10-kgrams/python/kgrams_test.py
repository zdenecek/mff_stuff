#!/usr/bin/python

import sys
from kgrams import SuffixArray

def test_generic(text, k, expected_count):
    sa = SuffixArray(text)
    count = sa.num_kgrams(k)
    assert count == expected_count, \
        "Expected %i %i-grams, found %i." % (expected_count, k, count)

def test_explicit(k, expected_count):
    test_generic("annbansbananas", k, expected_count)

def test_random(n, k, expected_count):
    b = bytearray(n)
    state = n
    for i in range(n):
        state = (state*2654289733 + 7) % (1 << 32)
        x = (state >> 28) % 16
        next = "aaaaaaaaaaaabbbc"[x]
        b[i] = ord(next)

    test_generic(b.decode(), k, expected_count)

def test_trivial(n, k, expected_count):
    s = "".join( "b" if i == n // 2 else "a" for i in range (n) )
    test_generic(s, k, expected_count)

tests = [
    ("basic-1",     lambda: test_explicit(1, 4)),
    ("basic-2",     lambda: test_explicit(2, 8)),
    ("basic-3",     lambda: test_explicit(3, 10)),
    ("basic-4",     lambda: test_explicit(4, 11)),
    ("basic-14",    lambda: test_explicit(14, 1)),

    ("short-5",     lambda: test_random(1000, 5, 107)),
    ("short-33",    lambda: test_random(1000, 33, 968)),
    ("short-500",   lambda: test_random(1000, 500, 501)),

    ("long-5",      lambda: test_random(100000, 5, 230)),
    ("long-33",     lambda: test_random(100000, 33, 99767)),
    ("long-5000",   lambda: test_random(100000, 5000, 95001)),

    ("triv-1",      lambda: test_trivial(1000000, 1, 2)),
    ("triv-5",      lambda: test_trivial(1000000, 5, 6)),
    ("triv-3333",   lambda: test_trivial(1000000, 3333, 3334)),
    ("triv-500000", lambda: test_trivial(1000000, 500000, 500001)),
]

if __name__ == "__main__":
    for required_test in sys.argv[1:] or [name for name, _ in tests]:
        for name, test in tests:
            if name == required_test:
                print("Running test {}".format(name), file=sys.stderr)
                test()
                break
        else:
            raise ValueError("Unknown test {}".format(name))

