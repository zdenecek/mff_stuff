#!/usr/bin/env python3
import sys
import random

from cuckoo_hash import CuckooTable
from hash_functions import TabulationHash, FixedHash

def inspect_table(cuckoo, hashes, n, table_size, step):
    table = cuckoo.get_table()
    assert len(table) == table_size, "The size of table is given and it is expected not to be changed."
    for i in range(n):
        k = step*i
        h0 = hashes[0].hash(k)
        h1 = hashes[1].hash(k)
        assert table[h0] == k or table[h1] == k, "Item should be stored on one of two positions given by hash functions."
        assert h0 == h1 or table[h0] != k or table[h1] != k, "Item should be stored only on one position."

    for t in range(table_size):
        k = table[t]
        if k is not None:
            assert k % step == 0 and k < step * n, "Only inserted items should be stored."
            assert hashes[0].hash(k) == t or hashes[1].hash(k) == t, "Item should be stored on one of two positions given by hash functions."

def simple_test(n, table_size_percentage):
    random.seed(42)
    num_buckets = n*table_size_percentage//100
    hashes = [TabulationHash(num_buckets), TabulationHash(num_buckets)]
    table = CuckooTable(num_buckets, hashes)

    # Insert an arithmetic progression
    for i in range(n):
        table.insert(37*i)

    # Verify contents of the table
    for i in range(n):
        assert table.lookup(37*i), "Item not present in table, but it should be."
        assert not table.lookup(37*i+1), "Item present in table, even though it should not be."

    inspect_table(table, hashes, n, num_buckets, 37)

def multiple_test(min_n, max_n, step_n, table_size_percentage):
    for n in range(min_n, max_n, step_n):
        print("\tn={}".format(n))
        simple_test(n, table_size_percentage)

def fixed_test():
    table_size = FixedHash.table_size
    hashes = [FixedHash(0), FixedHash(1) ] 
    cuckoo = CuckooTable(table_size, hashes)
    for k in range(FixedHash.keys):
        cuckoo.insert(k)
    inspect_table(cuckoo, hashes, FixedHash.keys, table_size, 1)

# A list of all tests
tests = [
    ("small",       lambda: simple_test(100, 400)),
    ("middle",      lambda: simple_test(31415, 300)),
    ("big",         lambda: simple_test(1000000, 300)),
    ("tight",       lambda: multiple_test(20000, 40000, 500, 205)),
    ("fixed",       fixed_test)
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
