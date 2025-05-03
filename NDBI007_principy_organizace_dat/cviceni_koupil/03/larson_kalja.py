#!/usr/bin/env python3

import csv
import sys

def hash(i ,k, n) :
    return (k + i) % n

def sign(i, k):
    return (k >> i) % 7

class Page:
    def __init__(self, sep) -> None:
        self.separator = sep
        self.records = []
        
    def __repr__(self) -> str:
        return f" Page(separator:{bin(self.separator)[2:]}, records: { len(self.records)})"
        
class Record:
    def __init__(self, sign, key) -> None:
        self.sign = sign
        self.key = key
        
    def __repr__(self) -> str:
        return f" Record(sign: {bin(self.sign)[2:]}, key:{self.key})"

class LK:
    
    def __init__(self, pages, page_capacity, sep) -> None:
        self.verbose = True
        self.page_capacity = page_capacity
        self.initial_separator = sep
        self.n = pages
        self.pages = [ Page(sep) for a in range(pages) ]
        
    def insert(self, key, i = 0):
        p = hash(i, key, self.n)
        page = self.pages[p]
        s = sign(i, key)
        
        if page.separator <= s:
            if(self.verbose): print(f"Page {p} separator {page.separator} is greater than sign {s}, have to try again")
            self.insert(key, i+1)
            return
        
        if len(page.records) < self.page_capacity:
            page.records.append(Record(s, key))
            if(self.verbose): print(f"Inserting into page {p}")
            
            return
        
        records = page.records + [Record(s, key)]
        
        max_sign = max(map(lambda r: r.sign, records))
        
        max_records =   list(filter(lambda r: r.sign == max_sign, records))
        other_records =filter(lambda r: r.sign != max_sign, records)
        
        page.records = list( other_records)
        
        page.separator = max_sign
        
        if(self.verbose): print(f"Updating separator of {p} to {max_sign}, reinserting {max_records}")
        
        
        for record in max_records:
            self.insert(record.key)
    
    def print(self):
        
        print("LK File:")
        for index, page in enumerate(self.pages):
            print(index, end= "  | ")
            for record in page.records: print(str(record.key).rjust(4), end=" ")
            
            print(f"\n{bin(page.separator)[2:]}| ", end="")
            for record in page.records: print(bin(record.sign)[2:].rjust(4), end=" ")
            print("")


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
        
    n = 4
    s = 4
    file = LK(s,n, 0b111)

    for item in items:            
        file.insert(int(item))
        file.print()     



        
        
if __name__ == "__main__":
    main()