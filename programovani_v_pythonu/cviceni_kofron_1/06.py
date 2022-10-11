

from enum import Enum, auto

from datetime import date as datetimedate
import json
import sys


class date(datetimedate):

    @staticmethod
    def getRange(start, end):
        for ordinal in range(start.toordinal(), end.toordinal() + 1):
            yield date.fromordinal(ordinal)

    @staticmethod
    def dayDiff(start, end):
        return end.toordinal() - start.toordinal() 

    @staticmethod
    def parse(string):
        return date(year=int(string[0:4]), month=int(
            string[4:6]), day=int(string[6:8]))

    def __str__(self) -> str:
        return f"{self.year}{str(self.month).rjust(2, '0')}{str(self.day).rjust(2, '0')}"


class SyntaxError(Exception):
    pass


class NoInputError(Exception):
    pass


class DataError(Exception):
    pass


class ArgFormat:

    @staticmethod
    def parse(string):
        pass


class StringArg(ArgFormat):

    @staticmethod
    def parse(string):
        return string


class DateArg(ArgFormat):

    @staticmethod
    def parse(string):
        return date.parse(string)


class CommandDispatcher:

    def __init__(self) -> None:
        self.commands = {}

    def register(self, command):
        # TODO checks
        self.commands[command.name] = command

    def dispatch(self, command, dataset, args):
        command.dispatch(dataset, args)

    def parse(self, inp: str):
        inp = inp.split()
        if len(inp) == 0:
            raise NoInputError("No input provided")

        name = inp.pop(0)

        if name not in self.commands:
            raise SyntaxError("No such command")

        command = self.commands[name]

        args = {}

        while len(inp) > 0:

            arg = inp.pop(0).split(":")

            if(len(arg) != 2):
                raise SyntaxError(
                    "Invalid argument syntax: missing colon or more colons")

            arg_name = arg[0]

            arg_data = arg[1]

            if arg_name not in command.allowed_args:
                raise SyntaxError(f"Arg not allowed: {arg_name}")

            format = command.allowed_args[arg_name]["format"]

            try:
                args[arg_name] = format.parse(arg_data)
            except:
                raise SyntaxError("Invalid argument format")

        for arg in command.allowed_args:
            if command.allowed_args[arg].get("required", False) and arg not in args:
                raise SyntaxError(f"Missing argument: {arg}")

        return command, args


class Command:

    def __init__(self, name, description, allowed_args) -> None:
        self.name = name
        self.description = description
        self.allowed_args = allowed_args

    def print(self, obj):
        print(str(obj))

    def dispatch(self, dataset, args: dict):
        pass


class displayCommand(Command):

    def __init__(self, name, description, value_name, value_selector) -> None:
        super().__init__(name, description, {
            "city": {"format": StringArg, "required": True},
            "date": {"format": DateArg, "required": True}
        })
        self.value_name = value_name
        self.value_selector = value_selector

    def dispatch(self, dataset, args: dict):
        val = dataset.get(args['city'], args['date'], self.value_selector)

        self.print(
            f"city:{args['city']} date:{args['date']} {self.value_name}:{val}")


class tempCommand(displayCommand):

    def __init__(self) -> None:
        super().__init__("temp", "temperature display", 'temp', 'temp')


class humidityCommand(displayCommand):

    def __init__(self) -> None:
        super().__init__("humidity", "display relative humidity", 'humidity', 'relhum')


class pressureCommand(displayCommand):

    def __init__(self) -> None:
        super().__init__("pressure", "display atmospheric pressure", 'pressure', 'ap')


class maximumCommand(Command):

    def __init__(self, name, description, value_name, value_selector) -> None:
        super().__init__(name, description, {
            "city": {"format": StringArg, "required": True},
            "startdate": {"format": DateArg, "required": True},
            "enddate": {"format": DateArg, "required": True}
        })
        self.value_name = value_name
        self.value_selector = value_selector

    def dispatch(self, dataset, args: dict):

        maximum = maximum_date = None

        for d in date.getRange(args['startdate'], args['enddate']):
            dt = str(d)
            if dt not in dataset.data[args['city']]:
                continue

            val = dataset.get(args['city'], d, self.value_selector)

            if maximum == None or val > maximum:
                maximum, maximum_date = val, dt

        if maximum == None:
            raise ValueError("No data in city")

        self.print(
            f"city:{args['city']} date:{maximum_date} {self.value_name}:{maximum}")


class maxtempCommand(maximumCommand):

    def __init__(self) -> None:
        super().__init__("maxtemp",
                         "the day with the maximum temperature in the specified city in the specified period", "temp", "temp")


class maxhumidityCommand(maximumCommand):

    def __init__(self) -> None:
        super().__init__("maxhumidity",
                         "the day with the maximum humidity in the specified city in the specified period", "humidity", "relhum")


class maxpressureCommand(maximumCommand):

    def __init__(self) -> None:
        super().__init__("maxpressure",
                         "the day with the maximum pressure in the specified city in the specified period", "pressure", "ap")


class extremeCityCommand(Command):

    def __init__(self, name, description, value_name, value_selector, value_comparer) -> None:
        super().__init__(name, description, {
            "date": {"format": DateArg, "required": True}
        })
        self.value_name = value_name
        self.value_selector = value_selector
        self.value_comparer = value_comparer

    def dispatch(self, dataset, args: dict):

        extreme_city = extreme_value = None

        date = str(args['date'])

        for city in dataset.data:
            if date not in dataset.data[city]:
                continue

            val = dataset.get(city, args['date'], self.value_selector)

            if extreme_city == None or self.value_comparer(val, extreme_value):
                extreme_city, extreme_value = city, val

        if extreme_city == None:
            raise ValueError("No data for selected date")

        self.print(
            f"city:{extreme_city} date:{date} {self.value_name}:{extreme_value}")


class warmestcityCommand(extremeCityCommand):

    def __init__(self) -> None:
        super().__init__("warmestcity",
                         "the city with the highest temperature on the specified day", 'temp', 'temp', lambda new, old: new > old)


class coldestcityCommand(extremeCityCommand):

    def __init__(self) -> None:
        super().__init__("coldestcity",
                        "the city with the lowest temperature on the specified day",  'temp', 'temp', lambda new, old: new < old)


class graphCommand(Command):

    def __init__(self, name, description, value_selector) -> None:
        super().__init__(name, description,
                        {"city": {"format": StringArg, "required": True},
                        "startdate": {"format": DateArg, "required": True},
                        "enddate": {"format": DateArg, "required": True}})
        self.value_selector = value_selector

    def dispatch(self, dataset, args: dict):
        
        data = []
        for d in date.getRange(args['startdate'], args['enddate']):
            data.append(dataset.get(args['city'], d, self.value_selector))

        data = self.reduce_data(data)

        mintemp = min(data)
        step = (max(data) - mintemp) / 19
        
        cols = []

        for entry in data:
            val = round ((entry - mintemp) / step) + 1
            cols.append(val)

        self.print_cols(cols)

    def reduce_data(self, ls):

        M = len(ls)

        if M <= 50:
            return ls

        daysInLastEntries = M // 50
        daysInFirstEntries = daysInLastEntries + 1
        firstEntries = (M % 50) * daysInFirstEntries

        data = []


        for x in range(0, firstEntries, daysInFirstEntries):
            data.append(sum(ls[x: x + daysInFirstEntries]) / daysInFirstEntries )

        for x in range(firstEntries, M, daysInLastEntries):
            data.append( sum(ls[x: x + daysInLastEntries]) / daysInLastEntries)

        return data

    def print_cols(self, cols):

        for x in reversed(range(20)):
            self.print("".join(["#" if col > x else " " for col in cols]))


class graphtempCommand(graphCommand):

    def __init__(self) -> None:
        super().__init__("graphtemp",
                        "graph of temperature in the specified city in the specified period", "temp")

class graphpressureCommand(graphCommand):

    def __init__(self) -> None:
        super().__init__("graphpressure",
                        "graph of the values for atmospheric pressure in the specified city in the specified period", 'ap')


def register_commands(dispatcher):

    dispatcher.register(tempCommand())
    dispatcher.register(humidityCommand())
    dispatcher.register(pressureCommand())
    dispatcher.register(maxtempCommand())
    dispatcher.register(maxhumidityCommand())
    dispatcher.register(maxpressureCommand())
    dispatcher.register(warmestcityCommand())
    dispatcher.register(coldestcityCommand())
    dispatcher.register(graphtempCommand())
    dispatcher.register(graphpressureCommand())


class Dataset():

    def __init__(self, data: dict) -> None:
        self.data = data

    def get(self, city, date, entry):

        date = str(date)

        if city in self.data and date in self.data[city] and entry in self.data[city][date]:
            val = self.data[city][date][entry]
            if entry == "relhum":
                val = round(val * 100)
            return val

        raise DataError("Entry not found")


def load_data(filename):

    with open(filename) as file:
        data = json.loads(file.read())
        return Dataset(data)


def main():
    dispatcher = CommandDispatcher()
    register_commands(dispatcher)

    filename = sys.argv[1] #"06data2.json"

    data = load_data(filename)

    for inp in sys.stdin:

        try:
            command, args = dispatcher.parse(inp)
            dispatcher.dispatch(command, data, args)
        except NoInputError:
            continue
        except Exception as e:
            print("Invalid input")
            # print(e)


if __name__ == "__main__":
    main()
