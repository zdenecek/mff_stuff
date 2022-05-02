
import sys

def main():
    num = int(sys.argv[1])
    print_multiples(num)

def print_multiples(num):
    for m in range(1,11):
        print(f"{str(m).rjust(2, ' ')} * {num} = {str(num*m).rjust(2, ' ')}")

if __name__ == "__main__":
    main()