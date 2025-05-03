
class Cup:

    def __init__(self, capacity: int, initial_volume: int = 0) -> None:
        if capacity < 0:
            raise ValueError("Cup capacity cannot be negative")
        
        self.capacity = capacity
        self.set_filled_volume(initial_volume)

    def clone(self):
        return Cup(self.capacity, self.get_filled_volume())
    
    def get_filled_volume(self):
        return self._filled_volume

    def set_filled_volume(self, value):
        if self.capacity < value:
            raise ValueError("Capacity cannot be overreached")
        if value < 0:
            raise ValueError("Volume cannot be negative")

        self._filled_volume = value
        self.empty_volume = self.capacity - value

    def is_full(self):
        return self.get_filled_volume() == self.capacity

    def is_empty(self):
        return self.get_filled_volume() == 0

    def can_pour_to(self, cup_to_pour_to):
        return (not self.is_empty()) and (not cup_to_pour_to.is_full())

    def pour_to(self, cup_to_pour_to):
        v = min(self.get_filled_volume(), cup_to_pour_to.empty_volume)
        self.set_filled_volume(self.get_filled_volume() - v)
        cup_to_pour_to.set_filled_volume(cup_to_pour_to.get_filled_volume() + v)





def main(cup_count, test_input = None, ):
    global combinations

        
    # all ordered pair chosen from three
    combinations = [ (f, t) for f in range(cup_count) for t in range (cup_count)]
    

    d =  list(map(int, input().split())) if test_input is None else test_input
    cups = [Cup(d[i], d[i+3]) for i in range(cup_count)]

    # implement breadth-first search
    que = Que((0, cups))
    state_history = set()
    volumes_list = {}

    while not que.is_empty():
        depth, cups = que.pop()
        solve(cups, que, state_history, volumes_list, depth)

    data = volumes_list.items()
    data = sorted(data, key = lambda t: t[0])
    print(*map(lambda d:f"{d[0]}:{d[1]}", data ))


def solve(cups: list, que: Que, state_history: set, volumes_list: dict, depth: int):
    #
    for cup in cups:
        v = cup.get_filled_volume()
        if v not in volumes_list:
            volumes_list[v] = depth
        # else there is no point in checking for greater depth because the states get checked in order by depth

    for fr, to in combinations:
        if not cups[fr].can_pour_to(cups[to]):
            continue

        new_cups = [cup.clone() for cup in cups]
        new_cups[fr].pour_to(new_cups[to])

        state = tuple((cup.get_filled_volume() for cup in new_cups))

        if state in state_history:
            continue
        
        state_history.add(state)
        que.append((depth + 1, new_cups))


main(cup_count=3)
