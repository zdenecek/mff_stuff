#!/usr/bin/env python3

import csv
import sys

ERROR = "#ERROR"
CYCLE = "#CYCLE"

############################################################################################################################
############################################################################################################################
############################################################################################################################


class CellNameConvertor:

    @staticmethod
    def validate_range(rng):
        if ":" not in rng:
            return False

        bounds = rng.split(":")

        if len(bounds) != 2:
            return False

        if not CellNameConvertor.validate_name(bounds[0]) or not CellNameConvertor.validate_name(bounds[1]):
            return False

        r1, c1 = CellNameConvertor.name_to_coords(bounds[0])
        r2, c2 = CellNameConvertor.name_to_coords(bounds[1])

        if r1 == r2:
            return c1 < c2
        if c1 == c2:
            return r1 < r2

        return False

    @staticmethod
    def range_to_coords(range):
        bounds = range.split(":")
        r1, c1 = CellNameConvertor.name_to_coords(bounds[0])
        r2, c2 = CellNameConvertor.name_to_coords(bounds[1])
        return r1, c1, r2, c2

    @staticmethod
    def validate_name(name):
        if len(name) < 2:
            return False
        col = ord(name[0])
        if not (ord("A") <= col <= ord("Z")):
            return False

        r = name[1:]
        try:
            row = int(r) - 1
        except ValueError:
            return False

        return 0 <= row <= 98

    @staticmethod
    def name_to_coords(name):
        c = name[0]
        col = ord(c) - ord("A")
        row = int(name[1:]) - 1

        return row, col

    @staticmethod
    def coords_to_name(name):
        raise NotImplemented()


############################################################################################################################
############################################################################################################################
############################################################################################################################


class Evaluatable:

    def __init__(self) -> None:
        self._evaluating = False
        self._evaluated = False
        self._error = None
        self._value = None

    def get_value(self):
        return self._value

    def get_error(self):
        return self._error

    def eval(self):
        if self._evaluated:
            return

        if self._evaluating:
            self._error = CYCLE
            self._evaluated = True
            self._evaluating = False
            return
    
        self._evaluating = True
        self.do_eval()
        self._evaluating = False
        self._evaluated = True

    def do_eval(self):
        pass
    
    def __str__(self) -> str:
        return self.get_error() or str(self.get_value())

class Cell(Evaluatable):
    def __init__(self) -> None:
        super().__init__()

class ValueCell(Cell):
    def __init__(self, value) -> None:
        super().__init__()
        self._value = value

    def get_error(self):
        return None

class ErrorCell(Cell):
    def __init__(self) -> None:
        super().__init__()
        self._error = ERROR

class EmptyCell(ValueCell): 
    def __init__(self) -> None:
        super().__init__(0)

    def __str__(self) -> str:
        return ""

class FormulaCell(Cell):
    def __init__(self, sheet, expression) -> None:
        super().__init__()
        self._sheet = sheet
        self._expression = expression
        self._evaluating = False
        self._evaluated = False
        self._error = None

    def get_value(self):
        self.eval()

        return self._expression.get_value()


    def get_error(self):
        self.eval()  
        
        return self._error

    def do_eval(self):
        self._expression.eval()
        self._error = self._expression.get_error()

###################################################################################################################
###################################################################################################################
###################################################################################################################
            

class Expression(Evaluatable):
    def __init__(self) -> None:
        super().__init__()

class ValueExpression(Expression):
    def __init__(self, value = None) -> None:
        super().__init__()
        self._value = value

    def get_value(self):
        return self._value

class EvaluatableExpression(ValueExpression):
    def __init__(self) -> None:
        super().__init__()

        
    def get_value(self):
        self.eval()
        return self._value

    def get_error(self):
        self.eval()  
        return self._error


class ReferenceExpression(EvaluatableExpression):
    def __init__(self, ref, sheet) -> None:
        super().__init__()
        self._ref = ref
        self._sheet = sheet

    def do_eval(self):
        cell = self._sheet.get_by_ref(self._ref)
        cell.eval()
        self._value = cell.get_value()

        error = cell.get_error()
        if error:
            if error == CYCLE:
                self._error = CYCLE
            else:
                self._error = ERROR

class PrimitiveExpresion(Expression):
    def __init__(self, val) -> None:
        super().__init__()
        self._value = val

class BinaryExpression(EvaluatableExpression):
    def __init__(self, expr1, expr2, operation) -> None:
        super().__init__()
        self._expr1 = expr1
        self._expr2 = expr2
        self._operation = operation

    def do_eval(self):
        errors = [self._expr1.get_error(), self._expr2.get_error()]

        errors = [error for error in errors if error]

        if errors:
            if all(map(lambda er: er == CYCLE, errors)):
                self._error = CYCLE
            else:
                self._error = ERROR
            return

        self._value = self._operation(self._expr1.get_value(),  self._expr2.get_value())
        
class PlusExpression(BinaryExpression):
    def __init__(self, expr1, expr2) -> None:
        super().__init__(expr1, expr2, lambda a, b: a + b)

        
class MinusExpression(BinaryExpression):
    def __init__(self, expr1, expr2) -> None:
        super().__init__(expr1, expr2, lambda a, b: a - b)

        
class TimesExpression(BinaryExpression):
    def __init__(self, expr1, expr2) -> None:
        super().__init__(expr1, expr2, lambda a, b: a * b)

        
class DividedByExpression(BinaryExpression):
    def __init__(self, expr1, expr2) -> None:
        super().__init__(expr1, expr2, lambda a, b: a // b)

    def do_eval(self):
        if self._expr2.get_value() == 0:
            self._error = "#DIV0"
        else:
            super().do_eval()

class RangeReferenceExpression(EvaluatableExpression):
    def __init__(self, range_ref, sheet) -> None:
        super().__init__()
        self._range_ref = range_ref
        self._sheet = sheet
        self._vals = []

    def get_values(self):
        self.eval()
        return self._vals

    def do_eval(self):
        cells = self._sheet.get_by_range_ref(self._range_ref)
        
        errors = []
        for cell in cells:
            cell.eval()
            errors.append(cell.get_error())

        errors =  [error for error in errors if error]

        if errors:
            if all(map(lambda er: er == CYCLE, errors)):
                self._error = CYCLE
            else:
                self._error = ERROR
            return

        self._vals = [ cell.get_value() for cell in cells]
    
class RangeFunctionExpression(EvaluatableExpression):
    def __init__(self, range_expr, func) -> None:
        super().__init__()
        self._range = range_expr
        self._func = func

    def do_eval(self):
        error = self._range.get_error()
        if error:
            self._error = error
            return
        
        self._value = self._func(self._range.get_values())

class SumExpression(RangeFunctionExpression):
    def __init__(self, range_expr) -> None:
        super().__init__(range_expr, lambda vals: sum(vals))

class AverageExpression(RangeFunctionExpression):
    def __init__(self, range_expr) -> None:
        super().__init__(range_expr, lambda vals: sum(vals) // len(vals))


############################################################################################################################
############################################################################################################################
############################################################################################################################


class CsvParser:

    def parse(self, rowstream):

        table = []

        reader = csv.reader(rowstream, delimiter=";")
        for row in reader:
            table.append(row)

        return table


class Sheet:

    def __init__(self, rows, cols) -> None:
        self.rows = rows
        self.cols = cols

        e = EmptyCell()
        self.cells = [ ]

        for i in range(rows):
            self.cells.append([e] * cols)

    def put(self, row, col, cell):
        self.cells[row][col] = cell

    def get(self, row, col):
        return self.cells[row][col]

    def get_str(self, row, col):
        cell = self.get(row, col)
        return str(cell)

    def get_by_ref(self, ref):
        row, col = ref
        return self.cells[row][col]

    def get_by_range_ref(self, rng):

        cells = []

        for r in range(rng[0], rng[2] + 1):
            for c in range(rng[1], rng[3] + 1):
                cells.append(self.get(r, c))

        return cells


class SheetParser:

    def __init__(self) -> None:
        self.expr_parser = ExpressionParser()

    def parse_sheet(self, table):
        sheet = Sheet(99, 99)

        for r, rowobj in enumerate(table):
            for c, cellstr in enumerate(rowobj):
                cell = self.parse_cell(cellstr, sheet)
                sheet.put(r, c, cell)

        return sheet


    def parse_cell(self, cellStr, sheet):

        if cellStr == "":
            return EmptyCell()

        try:
            num = int(cellStr)
            return ValueCell(num)
        except ValueError:
            pass

        if cellStr.startswith("="):
            expStr = cellStr[1:]
            try:
                expression = self.expr_parser.parse_expression(expStr, sheet)
                return FormulaCell(sheet, expression)
            except:
                pass

            
        return ErrorCell()


class ExpressionParser:

    def __init__(self) -> None:
        self.binaries = {
            "+": PlusExpression,
            "-": MinusExpression,
            "*": TimesExpression,
            "/": DividedByExpression
        }
        self.range_funcs = {
            "AVG": AverageExpression,
            "SUM": SumExpression
        }

    def parse_expression(self, expStr, sheet):
    
        valid_operand = self.valid_operand
        valid_rng = CellNameConvertor.validate_range

        for symbol in self.binaries:
            if symbol in expStr:
                operands = expStr.split(symbol)
                if len(operands) == 2 and valid_operand(operands[0]) and valid_operand(operands[1]): 
                    return self.binaries[symbol](self.parse_operand(operands[0], sheet), self.parse_operand(operands[1], sheet))

                return ErrorCell()

        for signature in self.range_funcs:
            if expStr.startswith(f"{signature}(") and expStr.endswith(")"):
                rng = expStr[4:-1]
                if valid_rng(rng):
                    return self.range_funcs[signature](RangeReferenceExpression(CellNameConvertor.range_to_coords(rng), sheet))
                return ErrorCell()

        if valid_operand(expStr):
            return self.parse_operand(expStr, sheet)

        raise Exception("Invalid expression")

    def valid_operand(self, operand_str):
        try:
            int(operand_str)
            return True
        except:
            pass
        return CellNameConvertor.validate_name(operand_str)

    def parse_operand(self, operand_str, sheet):
        try:
            return PrimitiveExpresion(int(operand_str))
        except ValueError:
            pass
        return ReferenceExpression(CellNameConvertor.name_to_coords(operand_str), sheet)


def main():

    csvparser = CsvParser()
    sheetparser = SheetParser()
    table = csvparser.parse(sys.stdin)
    sheet = sheetparser.parse_sheet(table)

    for r in range(len(table)):

        p = [sheet.get_str(r, c) for c in range(len(table[r]))]
    
        print( ";".join(p))

if __name__ == "__main__":
    main()
