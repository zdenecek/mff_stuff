
class Forrest:

    def __init__(self, dimension) -> None:
        self.dimension = dimension
        self.forrest = [["." for u in range(dimension)] for i in range(dimension)]

    def grow_tree(self, x, y, height, height_bole):
        

        width = 1
        halfwidth = height - 1

        while(halfwidth >= 0 and y <= self.dimension):
            
            for i in range(width):
                if  self.dimension <= x + halfwidth + i:
                    break
                self.forrest[y][x + halfwidth + i] = "*"
                
            halfwidth -= 1
            width += 2
            y += 1

        halfwidth = max(height - 1, 0)

        if  self.dimension <= x + halfwidth:
            return

        while(height_bole > 0 and y <= self.dimension):

            self.forrest[y][x + halfwidth] = "*"
            y += 1
            height_bole -= 1

    def print(self):
        for line in self.forrest:
            print(*line, sep="")

    def trim(self):
        x = list(enumerate(self.forrest))
        lines_with_something = list(filter(lambda a: "*" in a[1], x))

        if len(lines_with_something) == 0:
            return []

        indexes_with_something = list(map( lambda a: a[0], lines_with_something ))
        
        ma = max(indexes_with_something)
        mi = min(indexes_with_something)

        self.forrest = self.forrest[mi:ma+1]

        ma = max(map(lambda a: last_index_of(a[1], "*"), lines_with_something))
        mi = min(map(lambda a: a[1].index("*"), lines_with_something))

        self.forrest = list(map(lambda line: line[mi:ma+1], self.forrest))

    

def last_index_of(l, value):
    return len(l) - 1 - l[::-1].index(value)

    
def get_inputs():

    res = []

    inps = input().split("\n")

    while(True):
        for inp in inps:
            if(len(inp) == 0):
                return res
            res.append(list(map(lambda z: int(z), inp.split(" "))))
        inps = input().split("\n")


def main():

    forrest = Forrest(50)

    tree_inputs = get_inputs()

    for tree in tree_inputs:
        forrest.grow_tree(*tree)

    forrest.trim()
    forrest.print()

main()