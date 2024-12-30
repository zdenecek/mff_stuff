#!/usr/bin/env python3

from os import getcwd, scandir

# Ahoj

def main():
    print_dir(getcwd())

def print_dir(directory, prefix = ""):

    for file in sorted(scandir(directory), key = lambda d: d.name):
        if file.name.startswith(".") or file.is_symlink():
            continue
        
        is_dir = file.is_dir()

        print(prefix, file.name, "/" if is_dir else "", sep = "")

        if is_dir:
            print_dir(file, "    " + prefix)


if __name__ == "__main__":
    main()

