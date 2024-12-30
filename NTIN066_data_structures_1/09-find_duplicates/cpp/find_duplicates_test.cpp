#include <cmath>
#include <functional>
#include <iterator>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <cstdint>

using namespace std;

// If the condition is not true, report an error and halt.
#define EXPECT(condition, message) do { if (!(condition)) expect_failed(message); } while (0)

void expect_failed(const string& message);

template < typename Impl >
class IteratorHelper : iterator<input_iterator_tag, typename Impl::T> {
  public:
    IteratorHelper() {}

    template < typename ... Args >
    IteratorHelper(Args... args) : impl(args...) { finished = !impl.next(); }

    IteratorHelper& operator++() {
        finished = !impl.next();
        return *this;
    }

    IteratorHelper operator++(int) {
        IteratorHelper tmp(*this);
        operator++();
        return tmp;
    }

    bool operator==(const IteratorHelper& other) const { return other.finished && finished; }
    bool operator!=(const IteratorHelper& other) const { return !(*this == other); }
    auto operator*() -> typename Impl::T { return impl.get(); }

    private:
    bool finished = true;
    Impl impl;
};


class DataGenerator {
  public:
    struct Gen {
      uint64_t state;
      uint64_t mul;
      uint64_t mod;

      uint64_t next() {
        uint64_t ret = state;
        state = (state * mul) % mod;
        return ret;
      }
    };

    struct IteratorImpl {
        DataGenerator* dg = nullptr;
        bool only_dups;
        Gen rng, fw_gen, bw_gen;
        int fw_steps = 0;
        int bw_steps = 0;
        uint64_t val;
        string ret;

        using T = string;

        IteratorImpl() {}

        IteratorImpl(DataGenerator *dg, bool only_dups) : dg(dg), only_dups(only_dups) {
            rng = { (dg->seed * 311) % dg->prime, 78403, dg->prime };
            fw_gen = { dg->seed, dg->step, dg->prime };
            bw_gen = { dg->rev_seed, dg->rev_step, dg->prime };
        }

        bool next() {
            repeat:
            if (fw_steps >= dg->length) return false;

            if (rng.next() < dg->prime * (dg->repeat_prob / (dg->repeat_prob + 1))) {
                while (rng.next() < dg->prime * (1 - dg->repeat_prob)) {
                    bw_gen.next();
                    bw_steps++;
                }

                if (only_dups && bw_steps >= dg->length) return false;

                bw_steps++;
                val = bw_gen.next();
                return true;
            } else {
                fw_steps++;
                if (!only_dups) {
                  val = fw_gen.next();
                  return true;
                }
                goto repeat;
            }
        }

        string get() {
            constexpr char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";
            constexpr uint64_t p = (1 << 21) - 19;
            static_assert(sizeof(alphabet) == 65); // +1 due to '\0' at the end

            ret = string(dg->str_len, ' ');
            uint64_t state = val;
            int i = 0;

            while (i < dg->str_len) {
                for (int j = 0; j < 5 && i < dg->str_len; j++)
                    ret[i++] = alphabet[(state >> (6*j)) & 0x3F];
                state = state * p + 11;
            }

            return ret;
        }
    };

    using Iterator = IteratorHelper<IteratorImpl>;

    Iterator begin() { return Iterator(this, false); }
    Iterator end() { return Iterator(); }

    DataGenerator(int _seed, int _length, double _repeat_prob, int _str_len) {
        prime = (1ULL << 30) - 101;

        seed = _seed + 101 + _length;
        for (int i = 0; i < 100; i++) seed = (seed * 54321) % prime;

        repeat_prob = _repeat_prob;
        length = _length;

        step = 23987;

        uint64_t x = pow_mod(step, length - 1, prime);
        rev_seed = (x * seed) % prime;
        rev_step = mult_inverse(step, prime);

        str_len = _str_len;
    };

  private:
    string alphabet;
    uint64_t seed, rev_seed, step, rev_step, prime;
    int length, str_len;
    double repeat_prob;

    Iterator dups() { return Iterator(this, true); }

    uint64_t pow_mod(uint64_t x, uint64_t n, uint64_t mod) {
        if (n == 0) return 1;
        if (n == 1) return x % mod;

        uint64_t rec = pow_mod(x, n / 2, mod);
        rec = (rec * rec) % mod;

        if (n % 2 == 1) return (rec * x) % mod;
        return rec;
    }

    uint64_t mult_inverse(uint64_t x, uint64_t mod) {
        // works only for prime mod
        return pow_mod(x, mod - 2, mod);
    }

    friend void test_duplicates(int, int, double, int);
};

#include "find_duplicates.h"

#ifdef __linux__
#include <sys/time.h>
#include <sys/resource.h>
#endif

void test_duplicates(int seed, int length, double repeat_prob, int str_len) {
#ifdef __linux__
    rlimit data_limit;
    data_limit.rlim_cur = data_limit.rlim_max = 64 << 20;
    setrlimit(RLIMIT_DATA, &data_limit);
#endif

    DataGenerator generator(seed, length, repeat_prob, str_len);
    auto results = find_duplicates(generator);

    vector<string> correct;
    for (auto it = generator.dups(); it != generator.end(); ++it)
        correct.push_back(*it);

    EXPECT(results.size() == correct.size(),
           "Wrong number of generated duplicates, got " + to_string(results.size()) +
           " and expected " + to_string(correct.size()));

    sort(correct.begin(), correct.end());
    sort(results.begin(), results.end());

    for (int i = 0; i < int(results.size()); i++)
        EXPECT(results[i] == correct[i],
               "Wrong generated duplicate, got " + results[i] + " and expected " + correct[i]);
}

vector<pair<string, function<void()>>> tests = {
    {"10k", [] { test_duplicates(43, 10*1000, 0.01, 13); }},
    {"100k", [] { test_duplicates(43, 100*1000, 0.01, 20); }},
    {"1M", [] { test_duplicates(43, 1000*1000, 0.001, 40); }},
    {"10M", [] { test_duplicates(43, 10*1000*1000, 0.0001, 160); }},
    {"16M", [] { test_duplicates(43, 16*1000*1000, 0.0001, 360); }},
};
