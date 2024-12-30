from matrix_transpose import Matrix
import numpy

# Description of memory blocks is stored in an array blocks[block_index][B_xxx].
# If the block is cached, B_CACHED is 1 and B_LRU_NEXT and B_LRU_PREV point to
# neighboring blocks in the cyclic LRU list. Otherwise, B_CACHED is 0 and the block
# is not in the LRU.
B_CACHED = 0
B_LRU_NEXT = 1
B_LRU_PREV = 2

class CachedMatrix(Matrix):
    """A matrix stored in simulated cache"""

    def __init__(self, N, M, B, debug_level=0):
        assert N>0, "CachedMatrix must be non-empty."
        assert B>0, "Blocks must be non-empty."
        assert M%B == 0, "Cache size must be divisible by block size."
        assert M >= 2*B, "Cache must have at least 2 blocks."

        Matrix.__init__(self, N)

        NN = N*N
        self.items = numpy.zeros(shape=NN, dtype=numpy.int32, order="C")

        self.B = B
        self.M = M
        self.debug_level = debug_level
        self.mem_blocks = (NN+B-1) // B
        self.cache_blocks = M//B
        self.cache_used = 0

        # Initialize the LRU list. There is a virtual block right after the last real block,
        # which serves as a head of the cyclic LRU list.
        self.blocks = numpy.zeros(shape=(self.mem_blocks+1, 3), dtype=numpy.int32, order="C")
        self.lru_head = self.mem_blocks
        self.blocks[self.lru_head, B_LRU_NEXT] = self.lru_head
        self.blocks[self.lru_head, B_LRU_PREV] = self.lru_head

        self.reset_stats()

        if debug_level > 0:
            print("\tMemory: {} blocks of {} items, {} cached".format(self.mem_blocks, self.B, self.cache_blocks))

    def _pos(self, i, j):
        """Convert position in matrix to linear address in simulated memory."""

        return i*self.N + j

    def read(self, i, j):
        """Read value at position (i,j), used only in testing code."""

        assert i >= 0 and i < self.N and j >= 0 and j < self.N, "Read out of range: ({},{})".format(i, j)
        addr = self._pos(i, j)
        self._access(addr)
        return self.items[addr]

    def write(self, i, j, value):
        """Write value at position (i,j), used only in testing code."""

        assert i >= 0 and i < self.N and j >= 0 and j < self.N, "Write out of range: ({},{})".format(i, j)
        addr = self._pos(i, j)
        self._access(addr)
        self.items[addr] = value

    def swap(self, i1, j1, i2, j2):
        """Swap items (i1,j1) and (i2,j2)."""

        assert i1 >= 0 and i1 < self.N and j1 >= 0 and j1 < self.N and \
               i2 >= 0 and i2 < self.N and j2 >= 0 and j2 < self.N, \
               "Swap out of range: ({},{}) with ({},{})".format(i1, j1, i2, j2)
        if self.debug_level > 1:
            print("\tSwap ({},{}) ({},{})".format(i1, j1, i2, j2))
        addr1 = self._pos(i1, j1)
        addr2 = self._pos(i2, j2)
        self._access(addr1)
        self._access(addr2)
        items = self.items
        items[addr1], items[addr2] = items[addr2], items[addr1]

    def reset_stats(self):
        """Reset statistic counters."""

        self.stat_cache_misses = 0
        self.stat_accesses = 0

    def _access(self, addr):
        """Bring the given address to the cache."""

        blocks = self.blocks
        i = addr // self.B      # Which block to bring
        if blocks[i, B_CACHED] > 0:
            self._lru_remove(i)
        else:
            if self.cache_used < self.cache_blocks:
                # We still have room in the cache.
                self.cache_used += 1
                if self.debug_level > 1:
                    print("\t\tLoading block {}".format(i))
            else:
                # We need to evict the least-recently used block to make space.
                replace = blocks[self.lru_head, B_LRU_PREV]
                self._lru_remove(replace)
                assert blocks[replace, B_CACHED] > 0, "Internal error: Buggy LRU list"
                blocks[replace, B_CACHED] = 0
                if self.debug_level > 1:
                    print("\t\tLoading block {}, replacing {}".format(i, replace))
            blocks[i, B_CACHED] = 1
            self.stat_cache_misses += 1
        self._lru_add_after(i, self.lru_head)
        self.stat_accesses += 1

    def _lru_remove(self, i):
        """Remove block from the LRU list."""

        blocks = self.blocks
        prev, next = blocks[i, B_LRU_PREV], blocks[i, B_LRU_NEXT]
        blocks[prev, B_LRU_NEXT] = next
        blocks[next, B_LRU_PREV] = prev

    def _lru_add_after(self, i, after):
        """Add block at the given position in the LRU list."""

        blocks = self.blocks
        next = blocks[after, B_LRU_NEXT]
        blocks[after, B_LRU_NEXT] = i
        blocks[next, B_LRU_PREV] = i
        blocks[i, B_LRU_NEXT] = next
        blocks[i, B_LRU_PREV] = after

class TestMatrix(CachedMatrix):
    """A cached matrix extended by methods for testing."""

    # Constructor is inherited

    def fill_matrix(self):
        """Fill matrix with a testing pattern."""

        if self.debug_level > 1:
            print("\tInitializing")
        N = self.N
        for i in range(N):
            for j in range(N):
                self.write(i, j, i*N + j)

    def check_result(self):
        """Check that the pattern corresponds to the properly transposed matrix."""

        if self.debug_level > 1:
            print("\tChecking")
        N = self.N
        for i in range(N):
            for j in range(N):
                want = j*N + i
                have = self.read(i, j)
                have_i = have // N
                have_j = have % N
                assert have == want, "Mismatch at position ({},{}): expected element from ({},{}), found element from ({},{})".format(i, j, j, i, have_i, have_j)

    def naive_transpose(self):
        """Transpose the matrix naively."""

        for i in range(self.N):
            for j in range(i):
                self.swap(i, j, j, i)
