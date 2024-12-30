#!/usr/bin/env python3
import gc
import itertools
import sys
import string

from find_duplicates import find_duplicates

class DataGenerator():
    def __init__(self, seed, length, repeat_prob, str_len):
        self.prime = 2**30 - 101

        self.seed = seed + 101 + length
        for _ in range(100): self.seed = (self.seed * 54321) % self.prime

        self.repeat_prob = float(repeat_prob)
        self.length = length

        self.step = 23987

        x = self._pow_mod(self.step, self.length - 1, self.prime)
        self.rev_seed = (x * self.seed) % self.prime
        self.rev_step = self._mult_inverse(self.step, self.prime)

        self.str_len = str_len

    def _generator(self, only_dups=False):
        def gen(seed, step):
            state = seed
            while True:
                yield state
                state = (state * step) % self.prime

        rng = gen((self.seed * 311) % self.prime, 78403)
        fw_gen = gen(self.seed, self.step)
        bw_gen = gen(self.rev_seed, self.rev_step)

        fw_steps = 0
        bw_steps = 0
        while fw_steps < self.length:
            if next(rng) < self.prime * (self.repeat_prob / (self.repeat_prob + 1)):
                while next(rng) < self.prime * (1 - self.repeat_prob):
                    next(bw_gen)
                    bw_steps += 1

                if only_dups and bw_steps >= self.length: return

                bw_steps += 1
                yield self._make_string(next(bw_gen))
            else:
                fw_steps += 1
                if not only_dups:
                    yield self._make_string(next(fw_gen))

    def _make_string(self, x):
        alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-"
        assert(len(alphabet) == 64)
        long_strings = [
            "hn7fHKPgyw6GiGu3dRx8NpDPIK1eB2",
            "YPBhODY2UU7KTntxAI9YbK4JNPCPJj",
            "5qh0uhJW3ZheD65ZnNThGeeB6ds7pI",
            "wW8jgWM7cEkEmNWOsyEmOQezHGOGnf",
            "JAL6lzo1W3viaHhBrAPC992YIBdQHS",
            "Y7OtykNRwyNaZvHsLtFBYoVSJac9xM",
            "xIHUKmJFH663fuzs37PXSC8AwL9inq",
        ]
        p = 2**21 - 19

        ret = []
        state = x
        i = 0

        for j in range(0, 30, 6):
            if i >= self.str_len: break
            ret.append(alphabet[(state >> j) & 0x3F])
            i += 1

        state = state * p + 11;

        while i < self.str_len:
            ret.append(long_strings[state % len(long_strings)])
            state = state * p + 11;
            i += len(ret[-1])

        while i < self.str_len:
            for j in range(0, 30, 6):
                if i >= self.str_len: break
                ret.append(alphabet[(state >> j) & 0x3F])
                i += 1
            state = state * p + 11;

        return "".join(ret)

    def __iter__(self):
        return self._generator()

    def _pow_mod(self, x, n, mod):
        if n == 0: return 1
        if n == 1: return x % mod
        rec = self._pow_mod(x, n // 2, mod)
        rec = (rec * rec) % mod
        if n % 2 == 1:
            return (rec * x) % mod
        else:
            return rec

    def _mult_inverse(self, x, mod):
        # works only for prime mod
        return self._pow_mod(x, mod - 2, mod)

def test_duplicates(seed, length, repeat_prob, str_len):
    generator = DataGenerator(seed, length, repeat_prob, str_len)
    results = find_duplicates(generator)
    gc.collect()

    correct = list(generator._generator(only_dups=True))
    assert len(results) == len(correct), \
        "Wrong number of generated duplicates, got %i and expected %i" % (len(results), len(correct))
    assert sorted(results) == sorted(correct), \
        "The generates list of duplicates is not correct, got {} and expected {}".format(results, correct)

tests = [
    ("10k", lambda: test_duplicates(42, 10**4, 0.01, 14)),
    ("100k", lambda: test_duplicates(10, 10**5, 0.01, 20)),
    ("1M", lambda: test_duplicates(10, 10**6, 0.001, 340)),
    ("10M", lambda: True),
    ("16M", lambda: True),
]

if __name__ == "__main__":
    try:
        import resource
        resource.setrlimit(resource.RLIMIT_DATA, (12<<20, 12<<20))
    except:
        pass

    for required_test in sys.argv[1:] or [name for name, _ in tests]:
        for name, test in tests:
            if name == required_test:
                print("Running test {}".format(name), file=sys.stderr)
                test()
                break
        else:
            raise ValueError("Unknown test {}".format(name))
