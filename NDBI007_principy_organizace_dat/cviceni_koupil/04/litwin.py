#!/usr/bin/env python3

import sys
import csv


def hash1(k, bits) :
    return k & ((1 << bits) - 1)


        
class Column:
    def __init__(self, page_capacity, key, key_len) -> None:
        self.key = key
        self.key_len = key_len
        self.data = []
        self.capacity = page_capacity
        
    def __repr__(self) -> str:
        
        over =   len(self.data) - self.capacity 
        over_str = f"({over} elements on overflow page)" if over > 0 else ""
        return f"Column ( key: {bin(self.key)[2:].rjust(self.key_len, '0')}, data: {self.data} {over_str} )"

class Litwin:
    
    def __init__(self, page_capacity, split_after) -> None:
        
        self.directory = [ Column(page_capacity, 0, 0) ]
        
        self.split_after = split_after
        self.page_capacity = page_capacity
        self._inserts = 0
        self.stage = 0
        self.checking_bit = 1
        self.split_pointer = 0
        self.verbose = True
        
    
        
    def insert(self, key, split = True):
        h = hash1(key, self.stage)
        
        if h < self.split_pointer:
            if self.verbose: print(f"(Inserting into split page:)")
            
            h = hash1(key, self.stage + 1)
        
        
        col = self.directory[h]
        
        if self.verbose: print(f"Inserting into page {bin(h)} key {key}")
        if(key in col.data): 
            if self.verbose: print(f"Key already present")
            return
        
        
        if self._inserts == self.split_after and split:
            self._split()
            self._inserts = 0
            
         
        col.data.append(key)
        
        if split: self._inserts += 1
    
    def _split(self):
        
        if self.verbose: print(f"Splitting page {bin(self.split_pointer)}")
        
        p = self.directory[self.split_pointer]
        p.key_len += 1
        self.directory.append(Column(self.page_capacity, p.key | (1 << (p.key_len - 1)), p.key_len ))
        
        self.split_pointer += 1
            
        stop = 0 if self.stage == 0 else 1 << (self.stage)
        if self.split_pointer >= stop:
            self.new_stage()
        
        
        data = p.data
        p.data = []    
        
        for e in data:
            self.insert(e, False)
            
        if self.verbose: self.print()
            
            
    def new_stage(self):
        if self.verbose: print(f"New stage: {self.stage + 1}")
        self.stage += 1
        self.split_pointer = 0
        

    def print(self) -> str:
        print(f"Litwin file:")
        print(f"Stage: {self.stage}")
        
        for i, e in enumerate(self.directory):
            print("-> " if self.split_pointer == i else "   ", end="")
            print(f"{  bin(i).ljust(8, ' ')}: {e}")
            if(i + 1 == (1 << self.stage)): print("--- <divider>")
        



def main():
    records = sys.argv[1]
    personal = sys.argv[2]
    items = None
    
    with open(records, "r") as f:
        reader = csv.reader(f)
        next(reader, None) # skip header
        
        for line in reader:
            if line[0] == personal:
                items = list(map(int, line[1:]))
    
    if not items:
        print("No input.")
        return 
        
    c = 4
    s = 2
    file = Litwin(c,s)

    for item in items:            
        file.insert(int(item))
        
    print("Final structure:")
    file.print()

if __name__ == "__main__":
    main()