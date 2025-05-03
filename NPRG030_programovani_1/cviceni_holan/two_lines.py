from enum import Enum, auto
from math import sqrt


class Orientation(Enum):
    CLOCKWISE = auto()
    LINEAR = auto()
    ANTICLOCKWISE = auto()


class TwoCoordinatesObject:
    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

    def to_tuple(self):
        return (self.x, self.y)


class Point(TwoCoordinatesObject):

    def __repr__(self) -> str:
        return f"p({self.x}, {self.y})"


class Vector(TwoCoordinatesObject):

    def __init__(self, startpoint: Point, endpoint: Point) -> None:
        self.startpoint = startpoint
        self.endpoint = endpoint
        super().__init__(endpoint.x - startpoint.x, endpoint.y - startpoint.y)

    def __repr__(self) -> str:
        return f"vec({self.x}, {self.y})"

    def get_length(self):
        return sqrt(self.x**2 + self.y**2)


def calculate_orientation_vectors(v1: Vector, v2: Vector):

    a1 = (v1.endpoint.x - v1.startpoint.x)*(v2.endpoint.y - v2.startpoint.y)
    a2 = (v2.endpoint.x - v2.startpoint.x)*(v1.endpoint.y - v1.startpoint.y)

    return Orientation.LINEAR if a1 == a2 else Orientation.CLOCKWISE if a1 > a2 else Orientation.ANTICLOCKWISE


def calculate_orientation_points(p1: Point, p2: Point, p3: Point):

    return calculate_orientation_vectors(Vector(p1, p2), Vector(p2, p3))


def is_point_on_vector_whey_they_are_on_a_line(point: Point, vector: Vector):
    return min(vector.startpoint.x, vector.endpoint.x) <= point.x <= max(vector.startpoint.x, vector.endpoint.x) \
        and min(vector.startpoint.y, vector.endpoint.y) <= point.y <= max(vector.startpoint.y, vector.endpoint.y)


def calculate_do_line_segments_intersect(A: Point, B: Point, C: Point, D: Point):


    # shortcuts for legibility
    o = calculate_orientation_points
    pov = is_point_on_vector_whey_they_are_on_a_line

    oCDA = o(C, D, A)
    oCDB = o(C, D, B)

    CD = Vector(C, D)
    AB = Vector(A, B)

    # first check for zero-length lines:
    if AB.get_length() == 0:
        return oCDA == Orientation.LINEAR and pov(A, CD)
    elif CD.get_length() == 0:
        return o(A, B, C) == Orientation.LINEAR and pov(C, AB)

    # if both line segments are on a line, then they must overlap
    if oCDA == Orientation.LINEAR and oCDB == Orientation.LINEAR:
        return pov(A, CD) or pov(B, CD) or pov(C, AB) or pov(D, AB)
    # if one point is on the line, then that point must lie on the line segment
    elif oCDA is Orientation.LINEAR:
        return pov(A, CD)
    elif oCDB is Orientation.LINEAR:
        return pov(B, CD)
    # if none is linear, than for both AB and CD as perspectives, orientation must differ
    else:
        return oCDA != oCDB and o(A, B, C) != o(A, B, D)


def parse_input(input):

    coords = list(map(lambda c: int(c), input.split()))
    return [Point(coords[i*2], coords[i*2 + 1]) for i in range(4)]


def main(testinput=None):
    inp = testinput if testinput is not None else input()
    points = parse_input(inp)
    intersect = calculate_do_line_segments_intersect(*points)

    if testinput is None:
        print("ANO" if intersect else "NE")
    return intersect


if __name__ == "__main__":
    main()
