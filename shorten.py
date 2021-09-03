#!/usr/bin/env python3

import sys
from os import popen

def do_file(filename, min_filename):
    output = open(min_filename, "w")

    file = open(filename).readlines()

    def replace_rec(code, pat, rep):
        while code.find(pat) != -1:
            code = code.replace(pat, rep)
        return code

    def compress(code):
        code = code.strip(" \n")
        
        no_space = " {}()[]<>;,.+-*/%!^&|=?:"
        for c in no_space:
            code = replace_rec(code, " " + c, c)
            code = replace_rec(code, c + " ", c)
        return code

    cur = ""
    def refresh(cur):
        cur = compress(cur)
        if cur != "":
            print(cur, file=output)

    for line in file:
        line = line.strip(" \n")

        if line == "":
            continue
        elif line[0:2] == "//":
            continue
        elif line[0] == "#":
            refresh(cur)
            cur = ""
            print(line, file=output)
        else:
            cur += " " + line

    refresh(cur)
    cur = ""

files = popen("ls", "r").read().split("\n")
for file in files:
    if file.endswith(".hpp") and not file.endswith(".min.hpp"):
        min_filename = file[:-4]
        do_file(file, min_filename)
