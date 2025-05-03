
class Solver:

    def __init__(self) -> None:
        self.cache = {}

    def solve(self, remaining_rounds, opponents, prize_money):

        if remaining_rounds == 0:
            return (0, [])

        if (remaining_rounds, opponents) in self.cache:
            return self.cache[(remaining_rounds, opponents)]

        if remaining_rounds == 1:
            # if only one turn is remaining, linear function maximum is at the rightmost point
            eliminated = opponents
            current_maximum = prize_money * eliminated // opponents
            current_history = [opponents]

        else:
            current_maximum = 0
            current_history = []

            up_for_grabs = opponents - remaining_rounds + 1
            for eliminated in range(up_for_grabs + 1):

                next_rounds_maximum_prize, next_rounds_history = self.solve(remaining_rounds - 1, opponents - eliminated, prize_money)
                possible_prize = prize_money * eliminated // opponents + next_rounds_maximum_prize

                if possible_prize > current_maximum:
                    current_maximum = possible_prize
                    current_history = [eliminated] + next_rounds_history

        result = current_maximum, current_history

        self.cache[(remaining_rounds, opponents)] = result

        return result

def main(test_input = None):

    rounds = test_input or int(input())
    
    solver = Solver()
    amount, eliminations = solver.solve(rounds, 100, 100000)
    
    print(amount)
    print(*eliminations)
    
main()
