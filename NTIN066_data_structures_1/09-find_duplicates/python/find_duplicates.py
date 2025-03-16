#!/usr/bin/env python3
class BloomFilter:
    def __init__(self, size, hash_count, salt, array, offset):
        self.size = size
        self.hash_count = hash_count
        self.salt = salt
        self.array = array
        self.offset = offset

    def _hashes(self, item):
        h1 = hash(item) & 0xffffffff
        h2 = hash(self.salt + item) & 0xffffffff
        for i in range(self.hash_count):
            yield self.offset + ((h1 + i * h2) % self.size)

    def add(self, item):
        for pos in self._hashes(item):
            self.array[pos >> 3] |= (1 << (pos & 7))

    def contains(self, item):
        for pos in self._hashes(item):
            if not (self.array[pos >> 3] & (1 << (pos & 7))):
                return False
        return True


def find_duplicates(data_generator):
    T = 10**6
    min_bits = 1 << 10
    total = 0
    for _ in data_generator:
        total += 1
    num_buckets = max(1, total // T)
    dups = set()
    for bucket in range(num_buckets):
        exp = total / num_buckets
        bits = max(min_bits, int(9 * exp))
        k = 5
        tot_bits = 4 * bits
        tot_bytes = (tot_bits + 7) // 8
        arr = bytearray(tot_bytes)

        bf1 = BloomFilter(bits, k, "bf1", arr, 0)
        bf2 = BloomFilter(bits, k, "bf2", arr, bits)
        bf3 = BloomFilter(bits, k, "bf3", arr, 2 * bits)
        bf4 = BloomFilter(bits, k, "bf4", arr, 3 * bits)
        bucket_cand = set()
        for line in data_generator:
            if hash(line) % num_buckets != bucket:
                continue
            if bf1.contains(line) and bf2.contains(line) and bf3.contains(line) and bf4.contains(line):
                bucket_cand.add(line)
            else:
                bf1.add(line)
                bf2.add(line)
                bf3.add(line)
                bf4.add(line)
        cnts = {}
        for line in data_generator:
            if hash(line) % num_buckets != bucket:
                continue
            if line in bucket_cand:
                cnts[line] = cnts.get(line, 0) + 1
        for line, cnt in cnts.items():
            if cnt > 1:
                dups.add(line)
    return list(dups)
