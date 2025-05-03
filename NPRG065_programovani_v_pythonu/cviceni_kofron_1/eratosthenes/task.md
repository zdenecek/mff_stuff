Implement a program for determining the number of primes in the set `2..n` where `n` is written in a file given as a program argument provided on standard input.

Use the algorithm of [Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes). A naïve implementation would not be fast enough, there is a time limit set in Recodex.

The program prints out just the number of primes.

If you run the program for the number `1500000` (e.g., `primes.py < file_with_1500000`), it has to print out `114155`.