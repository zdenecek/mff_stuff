#!/usr/bin/env python3

import sys
import csv


def hash1(k, s) :
    return k % s

def hash2(i, k, r):
    return (k >> i) % r

class DirectoryEntry:
    def __init__(self, i, r, p) -> None:
        self.i = i
        self.r = r
        self.p = p
        self.empty = 0
        
    def __repr__(self) -> str:
        return f" DirEntry(i:{self.i}, r:{self.r}, p:{self.p}, [empty:{self.empty}])"
        
class FileEntry:
    def __init__(self, key, data) -> None:
        self.key = key
        self.data = data
        
    def __repr__(self) -> str:
        return f" FileEntry(key:{self.key})"

class Cormack:
    
    def __init__(self, directory_size) -> None:
        self.directory = {}
        self.primary_file = []
        self.directory_size = directory_size
        self.last_inserted_directory = None
        self.verbose = True
        
        
    def retrieve_directory(self, key):
        directory = self.directory[key] 
        index = directory.p
        return self.primary_file[index:index+directory.r]
    
    def _get_new_primary_file_pointer(self):
        return len(self.primary_file)
    
    def _get_directory_keys(self, dir):
        keys = []
        for i in range(dir.p, dir.p + dir.r):
            if isinstance(self.primary_file[i], FileEntry) :
                e = self.primary_file[i]
                keys.append(e.key)
                
        return keys
    
    def insert(self, key, data): 
        h = hash1(key, self.directory_size)
        if(self.verbose): print(f"inserting {key} to directory {h}")
        e =  FileEntry(key, data)
        
        if h not in self.directory:
            if(self.verbose): print(f"inserting into empty-directory")
            
            dir_entry = DirectoryEntry(0, 1, self._get_new_primary_file_pointer())
            self.directory[h] = dir_entry
            self.primary_file.append(e)

        else:
            dir_entry =self.directory[h]
            items =  self._get_directory_keys(dir_entry)
            
            if key in items: 
                if(self.verbose): print(f"duplicate key, ignoring insertion")
                
                return
            
            
            i, r, empty = self._find_perfect_hashing(items + [key], dir_entry.r + 1)
            if(self.verbose): print(f"perfect hashing found for i={i}, r={r}, that is {r - dir_entry.r - 1} new empty records")
            
            
            if h == self.last_inserted_directory:
                p = dir_entry.p
                self.primary_file += ["<empty>"] * (r - dir_entry.r)
                if(self.verbose): print(f"inserting into last class, simply expanding")
                delete = False
            else:
                # annulate old entries:
                delete = True
                
                p = self._get_new_primary_file_pointer()
                self.primary_file += ["<empty>"] * r
                if(self.verbose): print(f"moving class to the end of file at {p}")
            
            
            old_data = self.primary_file[dir_entry.p: dir_entry.p + dir_entry.r]
            
            if not delete:
                for x in range(dir_entry.p, dir_entry.p + dir_entry.r):
                    self.primary_file[x] = "<empty>"
            
            for k in items:
                new_index = hash2(i, k, r)
                old_index = hash2(dir_entry.i, k, dir_entry.r)
                self.primary_file[p + new_index] = old_data[old_index]
            
            
            
            self.primary_file[p + hash2(i, key, r)] = e
                
            if delete:
                for x in range(dir_entry.p, dir_entry.p + dir_entry.r ):
                    self.primary_file[x] = "<unusable>"
                    
            dir_entry.i = i
            dir_entry.r = r
            dir_entry.p = p
            dir_entry.empty += empty
                
        self.last_inserted_directory = h
        
        if(self.verbose): print(f"updated directory entry: {h} - {dir_entry}")

            
    def _find_perfect_hashing(self, items, min_r):
        empty = 0
        i = 0
        r = min_r
        while True:
            hashes = [True]
            while all(hashes):
                hashes = list( map(lambda x: hash2(i, x, r), items))
                if len(set(hashes)) == len(hashes):
                    return i, r, empty
                i +=1
                
            
            empty += 1
            r += 1 # collision for every i, try incrementing r
            i = 0
            
        
    def print(self, file = True) -> str:
        print(f"File:\n Directory:")
        for index in range(self.directory_size):
            print(str(index).rjust(2), end=":")
            print("<unused>" if index not in self.directory else self.directory[index])
        if(not file): return
        
        starts = {}
        for index, dir in self.directory.items():
            starts[dir.p] = index
        
        print("Primary file:")
        for i, entry in enumerate(self.primary_file):
            if i in starts:
                print(f"start of directory {starts[i]}")
            print(f"   {str(i).rjust(4)}: {entry}")
        



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
        
    s = 7
    file = Cormack(s)

    for item in items:            
        file.insert(int(item), None)

    file.print()     

            
        
        
        
if __name__ == "__main__":
    main()