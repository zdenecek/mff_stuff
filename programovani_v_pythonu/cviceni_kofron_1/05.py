from enum import Enum, auto
import sys

NO_OBSTACLE = False
OBSTACLE = True

class TwoDimensionalArray:

    def num_to_coords(self, num):
        return Coordinates(num // self.cols, num % self.cols)

    def coords_to_num(self, coords):
        return coords.col + coords.row*self.cols

    def __init__(self, rows, cols, initial=None, default=None) -> None:
        self.cols = cols
        self.rows = rows
        self.grid = initial or [default] * (rows * cols)

    def get(self, coords):

        c = self.coords_to_num(coords)
        return self.grid[c]

    def set(self, coords, val):
        c = self.coords_to_num(coords)
        self.grid[c] = val


class BoardWithObstacles(TwoDimensionalArray):

    def __init__(self, rows, cols) -> None:
        super().__init__(rows, cols, default=NO_OBSTACLE)

    def add_obstacle(self, coords):
        self.set(coords, OBSTACLE)




class Maze(BoardWithObstacles):

    def __init__(self, rows, cols) -> None:
        super().__init__(rows, cols)
        self.player = None
        self.player_orientation = None

    

    def __str__(self) -> str:

            
        orientations = {
            Orientation.North: "^",
            Orientation.East: ">",
            Orientation.South: "v",
            Orientation.West: "<",
        }

        lines = []
        for r in range(self.rows):
            
            line = []
            lines.append(line)

            for c in range(self.cols):
                
                if self.player.row == r and self.player.col == c:
                    line.append(orientations[self.player_orientation])
                else:
                    line.append("#" if self.get(Coordinates(r,c)) == OBSTACLE else ".")

        return "\n".join(" ".join(line) for line in lines)

class Coordinates:

    def __init__(self, row, col) -> None:
        self.row = row
        self.col = col

    def next(self, direction):
        if direction == Orientation.North:
            return Coordinates(self.row - 1, self.col)
        elif direction == Orientation.East:
            return Coordinates(self.row, self.col + 1)
        elif direction == Orientation.South:
            return Coordinates(self.row + 1, self.col)
        elif direction == Orientation.West:
            return Coordinates(self.row, self.col - 1)

    def in_bounds(self, max_row_excl, max_col_excl):
        return 0 <= self.row < max_row_excl and  0 <= self.col < max_col_excl

    def __str__(self) -> str:
        return f"{self.row} {self.col}"


class Orientation(Enum):
    North = 0
    East = 1
    South = 2
    West = 3

    def rotate_right(self, times=1):
        return Orientation((self.value + times) % 4)

    def rotate_left(self):
        return self.rotate_right(times=3)


def parse_input(stream):

    steps = int(stream.readline())
    
    lines = [ line.replace(' ', '').replace('\n','' ) for line in stream.readlines() ]

    rows = len(lines)
    cols = len(lines[0]) 

    maze = Maze(rows,cols)

    orientations = {
        "^": Orientation.North,
        ">": Orientation.East,
        "v": Orientation.South,
        "<": Orientation.West,
    }

    for r in range(rows):
        line = lines[r]
        for c in range(cols):
            if line[c] == "#":
                maze.add_obstacle(Coordinates(r,c))
            elif line[c] == ".":
                continue
            elif line[c] in orientations:
                maze.player = Coordinates(r,c)
                maze.player_orientation = orientations[line[c]]
            else:
                raise ValueError()

    return steps, maze
            
def do_step(maze: Maze, count = 0):
    
    if count == 4:
        raise ValueError()
    
    right = maze.player.next(maze.player_orientation.rotate_right())
    forward =  maze.player.next(maze.player_orientation)

    is_free = lambda coords: maze.get(coords) == OBSTACLE or not coords.in_bounds(maze.rows,maze.cols)

    if is_free(right):
        if is_free(forward):
            maze.player_orientation = maze.player_orientation.rotate_left()
            return do_step(maze, count + 1)
        else:
            maze.player = forward
    else:        
        maze.player_orientation = maze.player_orientation.rotate_right()
        maze.player = right

def main(stream):
    steps, maze = parse_input(stream)

    for i in range(steps):
        do_step(maze)

        # print(maze)

    print(maze)


if __name__ == "__main__":

    main(sys.stdin)
