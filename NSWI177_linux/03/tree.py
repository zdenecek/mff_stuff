#!/usr/bin/env python3

from os import getcwd, scandir
from argparse import ArgumentParser


def main():
    args = parseArgs()
    print_dir(args.path, args.only_dirs)

def parseArgs():
    parser = ArgumentParser()
    parser.add_argument("-d", "--only-dirs", action='store_true')
    parser.add_argument("path",  nargs='?', default=getcwd())
    return parser.parse_args()


def print_dir(directory, only_dirs = False, prefix = ""):

    for file in sorted(scandir(directory), key = lambda d: d.name):
        if file.name.startswith(".") or file.is_symlink():
            continue
        
        is_dir = file.is_dir()

        if is_dir or not only_dirs:
            print(prefix, file.name, "/" if is_dir else "", sep = "")

        if is_dir:
            print_dir(file, only_dirs, "    " + prefix)


if __name__ == "__main__":
    main()

