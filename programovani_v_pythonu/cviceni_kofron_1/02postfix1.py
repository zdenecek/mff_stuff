#!/usr/bin/env python3


from collections import deque
import sys

class MalformedError(ValueError):
    
    def __str__(self) -> str:
        return "Malformed expression"

class ZeroDivisionError(ValueError):
    
    def __str__(self) -> str:
        return "Zero division"

class Element():

    def __init__(self) -> None:
        self.is_number = False
        self.is_operation = False
        self.is_error = False

class DivisionError(Element):

    def __init__(self) -> None:
        super().__init__()
        self.is_error = True
    
    def __str__(self) -> str:
        return "Zero division"

class Operation(Element):

    def __init__(self) -> None:
        super().__init__()
        self.is_operation = True
    
    def evaluate(self, op1, op2):
        if op1.is_operation or op2.is_operation:
            raise MalformedError()

        if op1.is_error:
            return op1
        if op2.is_error:
            return op2

        return self.operate(op1, op2)

class Number(Element):
    def __init__(self, value) -> None:
        super().__init__()
        self.is_number = True
        self.value = value

    def __str__(self) -> str:
        return str(self.value)

class Plus(Operation):

    def __init__(self) -> None:
        super().__init__()

    def __str__(self) -> str:
        return "+"
    
    def operate(self, op1, op2):
        return Number(op1 + op2)

class Minus(Operation):

    def __init__(self) -> None:
        super().__init__()

    def __str__(self) -> str:
        return "-"
    
    def operate(self, op1, op2):
        return Number(op1 - op2)

class Times(Operation):

    def __init__(self) -> None:
        super().__init__()

    def __str__(self) -> str:
        return "*"
    
    def operate(self, op1, op2):
        return Number(op1 * op2)

class DividedBy(Operation):

    def __init__(self) -> None:
        super().__init__()

    def __str__(self) -> str:
        return "/"
    
    def operate(self, op1, op2):
        return Number(op1 / op2) if op2 != 0 else DivisionError()
             

def main(stream):
    for data in stream:
        try:
            data = list(map(lambda s: Element(s),  data.split()))
            if len(data) > 0:
                print(compute(data))
        except ValueError as e:
            print(e)
        
        
def raise_error(error):
    raise error

def compute(data):
    stack = deque()
    
    for el in data:
        if el.is_number:
            stack.append(el)
        else:
            if len(stack) < 2:
                raise MalformedError()
            op2, op1 = stack.pop(), stack.pop()
            stack.append(el.evaluate(op1, op2))

    if len(stack) != 1:
        raise MalformedError()

    return stack.pop().value


if __name__ == "__main__":
    main(sys.stdin)
        
        
    