import random

class TabulationHash:
    """Hash function for hashing by tabulation.

    The 32-bit key is split to four 8-bit parts. Each part indexes
    a separate table of 256 randomly generated values. Obtained values
    are XORed together.
    """

    def __init__(self, num_buckets):
        self.tables = [None] * 4
        self.num_buckets = num_buckets
        self.regenerate()

    def regenerate(self):
        for i in range(4):
            self.tables[i] = [random.randint(0, 0xffffffff) for _ in range(256)]

    def hash(self, key):
        h0 = key & 0xff
        h1 = (key >> 8) & 0xff
        h2 = (key >> 16) & 0xff
        h3 = (key >> 24) & 0xff
        t = self.tables
        return (t[0][h0] ^ t[1][h1] ^ t[2][h2] ^ t[3][h3]) % self.num_buckets
    

class FixedHash:
    keys = 5
    max_regenerations = 6
    table_size = 16

    hashes = [
        [ # Two items hashed into the same bucket by both functions
            [ 1, 7, 3, 7, 10 ],
            [ 2, 7, 4, 7, 11 ]
        ],
        [ # Three items stored in two positions
            [ 1, 7, 3, 8, 7 ],
            [ 2, 8, 4, 7, 8 ]
        ],
        [ # Four items stored in three positions
            [ 1, 7, 7, 8, 9 ],
            [ 2, 8, 9, 7, 8 ]
        ],
        [ # Five should be possible to store in five positions, but the cuckoo's insert operation may not find the proper locations
            [ 1, 2, 3, 4, 5 ],
            [ 2, 3, 4, 5, 1 ]
        ],
        [ # Five should be possible to store in six positions, the timeout in the insert may not be sufficient
            [ 1, 2, 3, 4, 5 ],
            [ 2, 3, 4, 5, 6 ]
        ],
        [ # This should be easy
            [ 8, 7, 7, 8, 12 ],
            [ 11, 6, 7, 9, 0 ]
        ]
    ]

    def __init__(self, id):
        self.id = id
        self.regenerations = 0

    def regenerate(self):
        self.regenerations += 1
        assert self.regenerations < self.max_regenerations, "Too many rehashes"

    def hash(self, key):
        assert 0 <= key and key < self.keys, "Invalid key"
        return self.hashes[self.regenerations][self.id][key]
