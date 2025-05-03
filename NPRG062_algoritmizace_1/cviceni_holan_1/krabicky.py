
from typing import Counter


class Solver:

    def solve(self, operations, nums):
        self.cache = {}
        self.reachable = set()

        
        # counter není nutný, jen pro zajímavost počítám kolikrát by se funkce volala nebýt cache
        self.counter = 0

        # history není potřeba, jen jsem chtěl vidět, jak jsem spočítal nějaká čísla
        # když Vojta posílal řešení s více nespočtenými čísly
        self.history = {}        
        
        self.counter = self.__go_through(operations, nums)
        return self.reachable

    def __go_through(self, operations, nums):



        if sum(operations.values()) == 0:
            return 1

        key = (operations['+'],operations['*'], tuple(nums))
        if key in self.cache:
            return self.cache[key]
        
        counter = 1


        for num1 in nums:
            for operation in operations:
                if operations[operation] != 0:
                    new_operations = dict(operations)
                    new_operations[operation] -= 1
                    
                    for num2 in nums:
                        calculated = operate[operation](num1, num2)
                        self.reachable.add(calculated)

                        if calculated in nums:
                            continue

                        new_nums = set(nums)
                        new_nums.add(operate[operation](num1, num2))

                        # navíc 
                        self.history[calculated] = \
                             f"({self.history.get(num1, num1)}{operation}{self.history.get(num2, num2)})"
                        # dulezite je rekurzivni volani, prirazeni do counteru je navic
                        counter += self.__go_through(new_operations, new_nums)

        # cache by mohlo byt jenom set, ale chtel jsem si pamatovat, kolikrat se ta funkce zavola pro to 
        # konkretni zadani
        self.cache[key] = counter
        return counter

        
               
operate = {
    "+": lambda x, y: x + y,
    "*": lambda x, y: x * y,
}

def main():

    import time

    start = time.time()

    operations = {
        "+": 2,
        "*": 2
    }

    nums = {2, 4, 5, 7}

    solver = Solver()
    solution = solver.solve(operations, nums)
    
    unreachable = set()

    for x in range(200, 301):
        if x in solver.history:
            print(x, " = ", solver.history[x])
        else:
            unreachable.add(x)

    print(f"unreachable: ", *sorted(unreachable)) # toto je jediny hlavni print
    print(f"how many times function would be called: {solver.counter}")
    print(f"time: {time.time() - start} seconds")
    # print(*solver.cache, sep="\n")


main()