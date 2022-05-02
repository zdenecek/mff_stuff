import numpy
import sys

def sieve(num):

    prime = numpy.full((num + 1), True)
    n = 2
    while n*n <= num:
        if(prime[n]):
            for x in range(n + n, num + 1, n):
                prime[x] = False
        n += 1
    return prime.sum() - 2

def main():
    num = int(sys.stdin.readline())
    print(sieve(num))


if __name__ == "__main__":
    main()
