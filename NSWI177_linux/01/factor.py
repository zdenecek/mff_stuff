#!/usr/bin/env python3


def getInputFromFile():
    file = open('input.txt', 'r')
    data = file.read()
    file.close()
    return int(data)


def primeFactors(number):

    if number < 1:
        raise ValueError()
        
    if number == 1:
        return [1]

    divisor = 2

    factors = []

    while number != 1:
        while number % divisor == 0:
            factors.append(divisor)
            number /= divisor
        divisor += 1
    
    return factors

def main(number = None):

    try:
        number = number or getInputFromFile()
        factors = primeFactors(number)
    except:
        print('-')
        return

    print(*factors, sep = "\n")
    

    
if __name__ == '__main__':
    main()
