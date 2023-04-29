
HOLE = "X"

class TwoDimensionalArray:
    
    def cords_to_rc(self, num):
        return (num // self.cols, num % self.cols)

    def rc_to_coords(self, r, c):
        return c + r*self.cols

    def __init__(self, rows, cols, initial = None) -> None:
        self.cols = cols
        self.rows = rows
        self.grid = initial or [None * (rows * cols)]

    def get(self, row, col):

        c = self.rc_to_coords(row, col)
        return self.grid[c]

    def set(self, row, col, val):
        c = self.rc_to_coords(row, col)
        self.grid[c] = val

    def index_of(self, val):
        return self.cords_to_rc(self.grid.index(val))
        


class Grid(TwoDimensionalArray):

    def __init__(self, rows, cols, initial_position = None) -> None:
        super().__init__(rows, cols, initial_position)   

    def clone(self):
        return Grid(self.rows, self.cols, list(self.grid))

    def get_possible_moves(self):
        moves = []
        h = self.get_hole()
        for dir in (-1, 1):
            if 0 <= h[0] + dir < self.rows:
                moves.append((h[0] + dir, h[1]))
        for dir in (-1, 1):
            if 0 <= h[1] + dir < self.cols:
                moves.append((h[0], h[1]  + dir))

        return moves

    def do_move(self, move):

        h = self.get_hole()
        self.set(*h, self.get(*move))
        self.set(*move, HOLE)


    def get_state(self):
        return " ".join(map(str,self.grid))

    def get_hole(self):
        return self.index_of(HOLE)

    def __str__(self) -> str:
        s = []
        for r in range(self.rows):
            s.append("|".join(self.grid[self.rc_to_coords(r, 0): self.rc_to_coords(r, self.cols)]))

        return "\n".join(s)




class Que:

    def __init__(self, root = None) -> None:
        self.root = None
        self.last = None
        if root is not None:
            self.append(root)

    def pop(self):
        x = self.root.obj
        self.root = self.root.next
        return x

    def append(self, obj):
        x = Node(obj) 
        if self.root is None:
            self.root = x
        else:
            self.last.next = x
        self.last = x

    def is_empty(self):
        return self.root is None

class Node:
    def __init__(self, obj = None, next = None) -> None:
        self.obj = obj
        self.next = next


def main(test_input = None, ):
   
    inp =(test_input or input()).split()

    grid = Grid(2, 3, inp)

    # implement breadth-first search
    que = Que((0, grid))
    state_history = {}

    while not que.is_empty():
        depth, grid = que.pop()
        solve(grid, que, state_history,  depth)

    data = state_history.items()
    data = sorted(data, key = lambda t: t[1])
    print(*map(lambda d:f"{d[0]} : {d[1]}", data ), sep="\n")




def solve(grid: Grid, que: Que, state_history: dict,  depth: int):
    
    state = grid.get_state()
    if state not in state_history:
        state_history[state] = depth
               
        for move in grid.get_possible_moves():
            new_grid = grid.clone()
            new_grid.do_move(move)
            que.append((depth + 1, new_grid))


main(f"1 2 3 4 5 {HOLE}")