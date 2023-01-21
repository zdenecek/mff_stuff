
class Solver:

    def __init__(self, prize_money) -> None:
        self.cache = {}
        self.prize_money = prize_money

    def solve(self, remaining_rounds, opponents):
        
        key = (remaining_rounds, opponents)
        
        if key in self.cache:
            return self.cache[key]
                
        result = self.__solve(*key)

        self.cache[key] = result

        return result

    def __solve(self, remaining_rounds, opponents):
        
        if remaining_rounds == 0:
            return 0, []

        if remaining_rounds == 1:
            # if only one turn is remaining, linear function maximum is at the rightmost point
            return self.prize_money, [opponents]

        current_maximum = 0
        current_history = []

        # we have to leave at least one opponent for each remaining turn
        up_for_grabs = opponents - remaining_rounds + 1

        for eliminated in range(1, up_for_grabs + 1):

            next_rounds_maximum, next_rounds_history = self.solve(remaining_rounds - 1, opponents - eliminated)
            possible_prize = self.prize_money * eliminated // opponents + next_rounds_maximum

            if possible_prize > current_maximum:
                current_maximum = possible_prize
                current_history = [eliminated] + next_rounds_history
        
        return current_maximum, current_history
            
        

def main(test_input = None):

    rounds = test_input or int(input())
    
    solver = Solver(100000)
    amount, eliminations = solver.solve(rounds, 100)
    
    print(amount)
    print(*eliminations)

    # print(*solver.cache.items(), sep="\n")

main()