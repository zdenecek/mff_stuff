class SuffixArray:
    def __init__(self, text):
        self.text = text
        # S is the suffix array (a permutation which sorts suffixes)
        # R is the ranking array (the inverse of S)
        self.R, self.S = self._build_suffix_array(text)

    def __getitem__(self, index):
        return self.S[index]

    def __len__(self):
        return len(self.S)

    def _build_suffix_array(self, text):
        """
        Construct the suffix array and ranking array for the given string
        using the doubling algorithm.
        """

        n = len(text)
        R = [None] * (n+1)
        S = [None] * (n+1)

        R = self._sort_and_rank(S, lambda a: ord(
            text[a]) if a < len(text) else -1)

        k = 1
        while k < n:
            R = self._sort_and_rank(S, lambda a: (
                R[a], (R[a+k] if a+k < n else -1)))
            k *= 2

        return (tuple(R), tuple(S))

    # An auxiliary function used in the doubling algorithm.
    def _sort_and_rank(self, S, key):
        for i in range(len(S)):
            S[i] = i
        S.sort(key=key)

        R = [None] * len(S)
        for i, s in enumerate(S):
            prev_s = S[i-1]
            if i == 0 or key(prev_s) != key(s):
                R[s] = i
            else:
                R[s] = R[prev_s]
        return R

    def num_kgrams(self, k):
        """Return the number of distinct k-grams in the string."""
        
        S_without_empty_suffix = self.S[1:]
        R_without_empty_suffix = list(map(lambda x: x- 1, self.R[:-1]))
        
        assert len(S_without_empty_suffix) == len(R_without_empty_suffix) == len(self.text) 

        lcp = build_lcp(self.text, S_without_empty_suffix, R_without_empty_suffix)
        count = 0
        # spocitame vsechny k-gramy
        # pro LCP >= k nemame novy k-gram
        # pro LCP < k mame novy k-gram, pokud je novy suffix alespon k delky
        l = len(self.text)
        for i, suffix_index in enumerate(S_without_empty_suffix):
            if l - suffix_index >= k:
                if lcp[i] < k:
                    count += 1
        return count
            


def build_lcp(s, sa, ra):
    n = len(s)
    lcp= [0] * (n-1)
    k = 0
    for i in range(n):
        if ra[i] == n - 1:
            k = 0
            continue
        j = sa[ra[i] + 1]
        while i + k < n and j + k < n and s[i + k] == s[j + k]:
            k += 1
        lcp[ra[i]] = k
        if k > 0:
            k -= 1
    l = [0]
    l.extend(lcp)
    return l
