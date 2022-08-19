#!/usr/bin/env python

from logging import root
from os.path import exists
from sys import argv
import re
import json


def main():
    if len(argv) < 2:
        print("need a input file")
        return

    rootDirectory = argv[1]
    file = argv[2]
    if not exists(file):
        print(f"\"{file}\" is not a file or does not exist")
        return

    with open(file, 'r') as f:
        lines = f.readlines()
        regex = r"\S+(gcc|g\+\+)\s.+-c\s(\S+)\s-o\s(.+)$"
        compile_commands = []
        for line in lines:
            matches = re.finditer(regex, line, re.MULTILINE)
            for match in matches:
                # TODO a remove list is required
                arguments = match.group(0).replace('-fipa-pta', '').split()
                inputFile = match.group(2)
                outputFile = match.group(3)
                compile_commands.append({
                    "arguments": arguments,
                    "directory": rootDirectory,
                    "file": inputFile,
                    "output": outputFile
                })
        with open(rootDirectory + "/compile_commands.json", 'w') as jsonFile:
            jsonFile.write(json.dumps(compile_commands, indent=2))


if __name__ == '__main__':
    main()
