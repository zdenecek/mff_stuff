NO_OBSTACLE = False
OBSTACLE = True

ROW = 0
COL = 1


class TwoDimensionalArray:
    
    def cords_to_rc(self, num):
        return (num // self.cols, num % self.cols)

    def rc_to_coords(self, r, c):
        return c + r*self.cols

    def __init__(self, rows, cols, initial = None, default = None) -> None:
        self.cols = cols
        self.rows = rows
        self.grid = initial or [default] * (rows * cols)

    def get(self, row, col):

        c = self.rc_to_coords(row, col)
        return self.grid[c]

    def set(self, row, col, val):
        c = self.rc_to_coords(row, col)
        self.grid[c] = val

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



class BoardWithObstacles(TwoDimensionalArray):

    def __init__(self, rows, cols) -> None:
        super().__init__(rows, cols, default=NO_OBSTACLE)

    def add_obstacle(self, position):
        self.set(*position, OBSTACLE)

    def possible_next_positions(self, current):
        
        moves = []

        # moves in all eight directions
        for dir in [(1,0),(-1,0),(0,1),(0,-1)]:
            curr = current
            while True:

                curr =  (curr[ROW] + dir[ROW], curr[COL] + dir[COL])
                if not (0 <= curr[ROW] < self.rows and 0 <= curr[COL] < self.cols):
                    break
                if self.get(curr[ROW], curr[COL]) == OBSTACLE:
                    break       
                moves.append(curr)

        return moves

    def __str__(self) -> str:
        s = []
        d = {OBSTACLE: "X", NO_OBSTACLE: "."}
        for r in range(self.rows):
            s.append("".join(map(d.get, self.grid[self.rc_to_coords(r, 0): self.rc_to_coords(r, self.cols)])))

        return "\n".join(s)


class Solver:

    def __init__(self) -> None:
        pass

    def solve(self, board: BoardWithObstacles, fr, to):

        # breadth search

        que = Que((fr, 0))
        visited = set()

        

        while not que.is_empty():
            position, depth = que.pop()

            if position == to:
                return depth

            next_moves = board.possible_next_positions(position)
            for move in next_moves:                
                if move not in visited:
                    que.append((move, depth + 1))
            
            visited.add(position)
        
        # position unreachable
        return -1

def parseBoard():
    board = BoardWithObstacles(8, 8)
    start = stop = None

    for r in range(8):
        row = input()
        for c in range(8):
            if row[c] == "x":
                board.add_obstacle((r,c))
            elif row[c] == "v":
                start = (r,c)
            elif row[c] == "c":
                stop = (r,c)

    return board, start, stop

def main():

    solver = Solver()

    solution = solver.solve(*parseBoard())

    print(solution)

main()

