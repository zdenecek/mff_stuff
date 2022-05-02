#!/usr/bin/env python3

import sys

def parse_length():

    if len(sys.argv) != 2:
        raise Exception()

    num = int(sys.argv[1])

    if num < 1:
        raise Exception()

    return num

def parse_length_or_fail():
    try:
        length = parse_length()
        return length
    except:
        print("Error")
        exit(1)

class Printer:

    def __init__(self,line_size) -> None:
        self.line_length = line_size
        self.buffer = []
        self.buffer_length = 0
        self.paragraph_started = False

    def print_justified(self, stream):
        
        for line in stream:
            self.add_line(line)

        self.end_paragraph(False)

    def add_line(self, line):
        words = line.split()

        if(len(words) == 0):
            self.end_paragraph()
            return

        self.paragraph_started = True

        for word in words:
            self.add_word(word)
        
    def add_word(self, word):
        space = 0 if self.buffer_length == 0 else 1

        new_length = self.buffer_length + space + len(word)

        if(new_length > self.line_length):

            if(self.buffer_length == 0): # lone word too long for one line
                self.buffer.append(word)
                self.buffer_length = new_length
                self.flush_buffer()
                return

            self.flush_buffer()
            return self.add_word(word)

        self.buffer.append(word)
        self.buffer_length += space + len(word)
            

    def end_paragraph(self, add_empty_line = True):
        if(not self.paragraph_started):
            return
        self.flush_buffer(False)
        self.paragraph_started = False
        if add_empty_line:
            print("")

    def flush_buffer(self, justify = True):
        if len(self.buffer) == 1:
            print(self.buffer[0])

        elif not justify:
            print(" ".join(self.buffer))

        else:
            space_count = len(self.buffer) - 1
            fillable_space = self.line_length - self.buffer_length
            space_size = fillable_space // space_count
            extra_spaces = fillable_space % space_count

            space = " " * space_size
            extra_space = space + " "
            last_word = self.buffer.pop()

            for i in range(len(self.buffer)):
                print(self.buffer[i], extra_space if i < extra_spaces else space, end="")

            print(last_word)

            
        self.buffer = []
        self.buffer_length = 0


def main(stream):
    length = parse_length_or_fail()
    printer = Printer(length)
    printer.print_justified(stream)
    
def test():
    with open("03INPUT") as file:
        main(file)
        

if __name__ == "__main__":
    main(sys.stdin)
