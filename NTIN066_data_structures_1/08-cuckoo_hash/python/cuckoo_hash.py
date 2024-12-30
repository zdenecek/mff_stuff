import math

class CuckooTable:
    """Hash table with Cuckoo hashing.

    We have two hash functions, which map 32-bit keys to buckets of a common
    hash table. Unused buckets contain None.
    """

    def __init__(self, num_buckets, hashes):
        """Initialize the table with the given number of buckets.
        The number of buckets is expected to stay constant."""
        self.num_buckets = num_buckets
        self.table = [None] * num_buckets
        self.hashes = hashes

    def get_table(self):
        return self.table

    def lookup(self, key):
        """Check if the table contains the given key. Returns True or False."""
        b0 = self.hashes[0].hash(key) % self.num_buckets
        b1 = self.hashes[1].hash(key) % self.num_buckets
        return self.table[b0] == key or self.table[b1] == key

    def insert(self, key):
        """Insert a new key to the table. Assumes that the key is not present yet."""
        b0 = self.hashes[0].hash(key) % self.num_buckets
        b1 = self.hashes[1].hash(key) % self.num_buckets

        if self.table[b0] is None:
            self.table[b0] = key
            return
        if self.table[b1] is None:
            self.table[b1] = key
            return

        current_key = key
        current_bucket = b0
        max_moves = int(math.ceil(6 * math.log2(self.num_buckets)))  

        for _ in range(max_moves):
            displaced_key = self.table[current_bucket]
            self.table[current_bucket] = current_key

            if current_bucket == self.hashes[0].hash(displaced_key) % self.num_buckets:
                new_bucket = self.hashes[1].hash(displaced_key) % self.num_buckets
            else:
                new_bucket = self.hashes[0].hash(displaced_key) % self.num_buckets

            if self.table[new_bucket] is None:
                self.table[new_bucket] = displaced_key
                return

            current_key = displaced_key
            current_bucket = new_bucket

        self.rehash(current_key)

    def rehash(self, key):
        """Relocate all items using new hash functions and insert a given key."""
        for i in range(2):
            self.hashes[i].regenerate()

        keys = [k for k in self.table if k is not None]
        keys.append(key)

        self.table = [None] * self.num_buckets

        for k in keys:
            self.insert(k)
