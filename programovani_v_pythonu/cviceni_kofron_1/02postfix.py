#!/usr/env python3

import sys

class MalformedError(ValueError):
    
    def __str__(self) -> str:
        return "Malformed expression"



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

        return self.operate(op1.value, op2.value)

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
        return Number(int(op1 / op2)) if op2 != 0 else DivisionError()

def evaluate(inputs):
    stack = []
    for a in inputs:
        try:
            stack.append(Number(int(a)))
            continue
        except:
            pass

        if len(stack) < 2:
            raise MalformedError()

        op1, op2 = stack.pop(), stack.pop()

        if a == '+':
            stack.append(Plus().evaluate(op2, op1))
        elif a == '-':
            stack.append(Minus().evaluate(op2, op1))
        elif a == '*':
            stack.append(Times().evaluate(op2, op1))
        elif a == '/':
            stack.append(DividedBy().evaluate(op2, op1))
        else:
            raise MalformedError()

    if len(stack) != 1:
        raise MalformedError()
    return stack.pop()

def main(lines):
    for line in lines:
        data = line.split()
        if len(data) == 0:
            continue
        try:
            print(evaluate(data))
        except ValueError as e:
            print(e)

if __name__ == "__main__":
    main(sys.stdin)

        