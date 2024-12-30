#!/usr/bin/env python3

from tap.parser import Parser
import sys, os
import json

def main():

    parser = Parser()
    data = []

    for file in sys.argv[1:]:

        data_obj = { 
            "filename": file,
            "total": 0,
            "passed": 0,
            "skipped": 0,
            "failed": 0}

        num = 0

        if os.path.isfile(file):

            try:

                for line in parser.parse_file(file):
                    if line.category == "plan":
                        data_obj["total"] = line.expected_tests
                    if line.category == "test":

                        num += 1
                        if line.skip:
                            data_obj["skipped"] += 1
                        elif line.ok:
                            data_obj["passed"] += 1
                    elif line.category == "bail":
                        data_obj['skipped'] += data_obj["total"] - num
                
                data_obj["failed"] = data_obj["total"] - data_obj["skipped"] - data_obj["passed"]

            except FileNotFoundError:
                pass
            
        data.append(data_obj)

    print(json.dumps({"summary" : data}, indent=True))

if __name__ == "__main__":
    main()   