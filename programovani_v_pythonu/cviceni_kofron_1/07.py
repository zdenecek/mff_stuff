#!/usr/bin/env python3

import csv
import sys

ERROR = "#ERROR"
CYCLE = "#CYCLE"


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


class Cell:

    def __init__(self, val) -> None:
        self.val = val

    def get_value(self, context):
        return self.val

    def is_error(self, context):
        return False

    def get_error_val(self, context):
        raise NotImplemented()

    def __str__(self) -> str:
        return str(self.val)


class EmptyCell(Cell):

    def __init__(self) -> None:
        super().__init__(val=0)

    def __repr__(self) -> str:
        return "Empty"

    def __str__(self) -> str:
        return ""


class ValueCell(Cell):

    def __init__(self, val) -> None:
        super().__init__(val)

    def __repr__(self) -> str:
        return f"Val({self.val})"


class ComplexCell(Cell):

    def __init__(self, val = None) -> None:
        super().__init__(val)
        self.error = False
        self.error_val = None
        self.evaluated = False
        self.evaluating = False

    def get_value(self, context):
        if not self.evaluated:
            self.evaluate(context)

        return self.val

    def is_error(self, context):
        if not self.evaluated:
            self.evaluate(context)

        return self.error

    def get_error_val(self, context):
        return self.error_val

    def evaluate(self, context):

        if self.evaluating:
            self.error = True
            self.error_val = CYCLE
            self.evaluating = False
            self.evaluated = True
            return

        self.evaluating = True
        self.val = self._do_eval(context)
        self.evaluating = False
        self.evaluated = True

    def _do_eval(self, context):
        raise NotImplemented()

    def __repr__(self) -> str:
        return f"Complex({self.val},{self.error_val})"
    
    def __str__(self) -> str:
        if not self.evaluated:
            raise MemoryError()
        if self.error:
            return self.error_val
        return super().__str__()

    def _choose_error(self, cells, context):
        self.error = True
        
        if all( map(lambda c: c.get_error_val(context) == CYCLE, cells)):
            self.error_val = CYCLE
            return

        self.error_val = ERROR



class ErrorCell(ComplexCell):
    def __init__(self) -> None:
        super().__init__()
        self.error = True
        self.error_val = ERROR

    def _do_eval(self, context):
        pass

    def __repr__(self) -> str:
        return "Err"


class BinaryOperationCell(ComplexCell):

    def __init__(self, val1, val2, operation) -> None:
        super().__init__()
        self.val1 = val1
        self.val2 = val2
        self.operation = operation

    def _do_eval(self, context):

        v1 = self.val1.get_value(context)
        v2 = self.val2.get_value(context)
        

        errors = []

        if self.val1.is_error(context):
            errors.append(self.val1)
        if self.val2.is_error(context):
            errors.append(self.val2)

        if errors:
            self._choose_error(errors, context)
            return


        return self.operation(v1, v2)




class PlusCell(BinaryOperationCell):

    def __init__(self, val1, val2) -> None:
        super().__init__(val1, val2, lambda a, b: a + b)


class MinusCell(BinaryOperationCell):

    def __init__(self, val1, val2) -> None:
        super().__init__(val1, val2, lambda a, b: a - b)


class TimesCell(BinaryOperationCell):

    def __init__(self, val1, val2) -> None:
        super().__init__(val1, val2, lambda a, b: a * b)


class DividedByCell(BinaryOperationCell):

    def __init__(self, val1, val2) -> None:
        super().__init__(val1, val2, lambda a, b: a // b)

    def evaluate(self, context):
        try:
            super().evaluate(context)
        except ZeroDivisionError:
            self.evaluating = False
            self.evaluated = True
            self.error = True
            self.error_val = "#DIV0"


class RangeCell(ComplexCell):
    def __init__(self, range, operation) -> None:
        super().__init__()
        self.range = range
        self.operation = operation

    def _do_eval(self, context):

        range_cells = context.get_range(self.range)

        errors = [cell for cell in range_cells if cell.is_error(context)]

        if errors:
            self._choose_error(errors, context)
            return

        values = map(lambda cell: cell.get_value(context), range_cells)

        return self.operation(values)



class Primitive:
    def __init__(self, val) -> None:
        self.val = val
    
    def get_value(self, context):
        return self.val

    def is_error(self, context):
        return False

    def get_error_val(self, context):
        raise EOFError()
    

class Reference:

    def __init__(self, name) -> None:
        self.name = name

    def get_value(self, context):
        return context.get_by_name(self.name).get_value(context)

    def is_error(self, context):
        return context.get_by_name(self.name).is_error(context)

    def get_error_val(self, context):
        return context.get_by_name(self.name).get_error_val(context)

    def get_cell(self, context):
        return context.get_by_name(self.name)

class ReferenceCell(ComplexCell):

    def __init__(self, ref) -> None:
        super().__init__()
        self.ref = ref

    def _do_eval(self, context):
        if self.ref.is_error(context):
            self._choose_error([self.ref.get_cell(context)], context)
            return

        return self.ref.get_value(context)

class SumCell(RangeCell):

    def __init__(self,  range) -> None:
        super().__init__( range, lambda vals: sum(vals))


class AverageCell(RangeCell):

    def __init__(self, range) -> None:
        super().__init__(range, lambda vals: sum(vals) // len(vals))


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
        cell = self.cells[row][col]
        cell.get_value(self)
        return cell

    def get_str(self, row, col):
        
        cell = self.get(row, col)
        
        return str(cell)
        

    def get_by_name(self, name):
        row, col = CellNameConvertor.name_to_coords(name)
        return self.cells[row][col]

    def get_range(self, rng):
        cs = []

        for r in range(rng[0], rng[2] + 1):
            for c in range(rng[1], rng[3] + 1):
                cs.append(self.get(r, c))

        return cs


class SheetParser:

    def __init__(self) -> None:
        self.binaries = {
            "+": PlusCell,
            "-": MinusCell,
            "*": TimesCell,
            "/": DividedByCell
        }

    def parse_sheet(self, table):
        sheet = Sheet(99, 99)

        for r, rowobj in enumerate(table):
            for c, cellstr in enumerate(rowobj):
                cell = self.parse_cell(cellstr)
                sheet.put(r, c, cell)

        return sheet



    def parse_cell(self, cellStr):

        valid = CellNameConvertor.validate_name
        valid_rng = CellNameConvertor.validate_range
        valid_operand = self.valid_operand


        if cellStr == "":
            return EmptyCell()

        try:
            num = int(cellStr)
            return ValueCell(num)
        except ValueError:
            pass

        if cellStr.startswith("="):
            cellStr = cellStr[1:]
        else:
            return ErrorCell()


        for symbol in self.binaries:
            if symbol in cellStr:
                operands = cellStr.split(symbol)
                if len(operands) == 2 and valid_operand(operands[0]) and valid_operand(operands[1]): 
                    return self.binaries[symbol](self.parse_operand(operands[0]), self.parse_operand(operands[1]))

                return ErrorCell()

        if cellStr.startswith("AVG(") and cellStr.endswith(")"):
            rng = cellStr[4:-1]
            if valid_rng(rng):
                return AverageCell(CellNameConvertor.range_to_coords(rng))
            return ErrorCell()


        if cellStr.startswith("SUM(") and cellStr.endswith(")"):
            rng = cellStr[4:-1]
            if valid_rng(rng):
                return SumCell(CellNameConvertor.range_to_coords(rng))
            return ErrorCell()

        if valid_operand(cellStr):
            return ReferenceCell(self.parse_operand(cellStr))

        return ErrorCell()

    def valid_operand(self, operand_str):
        try:
            int(operand_str)
            return True
        except:
            pass
        return CellNameConvertor.validate_name(operand_str)

    def parse_operand(self, operand_str):
        try:
            return Primitive(int(operand_str))
        except:
            pass
        return Reference(operand_str)


class CsvParser:

    def parse(self, rowstream):

        table = []

        reader = csv.reader(rowstream, delimiter=";")
        for row in reader:
            table.append(row)

        return table


def main():

    test="5;=A1;=Z1+A1;=B1+B1"
    csvparser = CsvParser()
    sheetparser = SheetParser()
    table = csvparser.parse(test.split())
    table = csvparser.parse(sys.stdin)
    sheet = sheetparser.parse_sheet(table)

    for r in range(len(table)):

        p = [sheet.get_str(r, c) for c in range(len(table[r]))]
    
        print( ";".join(p))



if __name__ == "__main__":
    main()
