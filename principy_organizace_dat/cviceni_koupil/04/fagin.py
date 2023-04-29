#!/usr/bin/env python3

import sys
import csv


def hash1(k, bits) :
    return k & ((1 << bits) - 1)


        
class Page:
    def __init__(self, key, local_depth = 0) -> None:
        self.key = key
        self.data = []
        self.local_depth = local_depth
        
    def __repr__(self) -> str:
        return f" Page( key: {bin(self.key)[2:].rjust(self.local_depth, '0')}, depth: {self.local_depth}, data: {self.data})"

class Fagin:
    
    def __init__(self, capacity) -> None:
        p = Page(0)
        self.directory = { 0: p, 1: p }
        
        self.global_depth = 1
        self.capacity = capacity
        self.verbose = True
        
    
        
    def insert(self, key):
        h = hash1(key, self.global_depth)
        page = self.directory[h]
        
        
        
        
        if self.verbose: print(f"Inserting {key} into page {bin(h)}")
        
        if key in page.data:
            if self.verbose: print(f"Key already present")
            return
            
        if len(page.data) < self.capacity:
            page.data.append(key)
            return
        
        if page.local_depth < self.global_depth:
            self._split_page(h)  
            
        else:
            self._increase_global_depth()
        
        self.insert(key)      
        
    
    def _increase_global_depth(self):
        
        if self.verbose: print(f"Increasing global depth to { self.global_depth + 1}")
        new_bit = 1 << self.global_depth
        
        for index in  range( (1 << self.global_depth) + 1):
            self.directory[index | new_bit] = self.directory[index]
        
        self.global_depth += 1
    
    def _split_page(self, index):
                
        page = self.directory[index]
        index = page.key
        if self.verbose: print(f"Increasing page {bin(index)} depth to { page.local_depth + 1}")
        
        bit = 1 << (page.local_depth)
        
        index1 = index | bit
        index2 = index & ~bit
        
        p1 =  Page(index1, page.local_depth + 1)
        p2 =  Page(index2, page.local_depth + 1)
        
        bit <<= 1
        

        while index1  in self.directory:
            if self.verbose: print(f"Changing pointer {bin(index1)} to {p1}")
            
            self.directory[index1] = p1
            index1 += bit    
            
        while index2 in self.directory:
            if self.verbose: print(f"Changing pointer {bin(index2)} to {p2}")
            
            self.directory[index2] = p2
            index2 += bit   
                
                
        for e in page.data:
            self.insert(e)
            
        if self.verbose: self.print()
        
    
    
    def print(self) -> str:
        print(f"Fagin file:")
        print(f"Depth: {self.global_depth}")
        
        for x in range( (1 << self.global_depth) ):
            print(f"{  bin(x).ljust(8, ' ')}: {self.directory[x]}")
        



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
        
    c = 3
    file = Fagin(c)

    for item in items:            
        file.insert(int(item))
        
    print("Final structure:")
    file.print()

        
if __name__ == "__main__":
    main()