from two_lines import main, Vector, Point, parse_input
from two_lines_not_mine import intersection

def generate_test_data():
    import random
    
    test_data = []
    for i in range(20000):
        coords = []
        for x in range(8):
            coords.append(str(random.randint(-100, 100)))
        
        s = " ".join(coords)
        test_data.append(s)
    
    return test_data

def test():

    from two_lines_not_mine import intersection

    test_data = generate_test_data()

    for inp in test_data:
        my_res = main(inp)
        points = list(map(lambda p: p.to_tuple(), parse_input(inp)))
        
        try:
            intersection(*points)
            other = True
        except:
            other = False
        
        if(my_res == other):
            pass

        print(f"Test for |{inp}|, got {my_res}, other program got {other}" + \
            f" ... {'OK' if my_res == other else 'FAIL'}")

test()


