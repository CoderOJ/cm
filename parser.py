#!/usr/bin/env python3

import os
import sys
import re

macro_list = {}

def get_dir(filename):
    if not filename.startswith("/"):
        filename = "./" + filename
    return "/".join(filename.split("/")[:-1])

def parse_file(filename):
    global macro_list

    try:
        file = open(filename)
    except FileNotFoundError:
        print("\033[;31merror: file " + filename+ " not found", file=sys.stderr)
        return
    lines = map(lambda x: x.strip("\n"), file.readlines())
    ori_dir = os.getcwd()
    os.chdir(get_dir(filename))

    def parse_to_endif(lines, p = True):
        while True:
            try:
                line = next(lines)
            except StopIteration:
                return "EOF"

            sline = line.strip(" ")

            if sline == "#endif" or sline == "#else":
                return sline
            elif sline.startswith('#include "'):
                if p:
                    header_name = sline.split('"')[1]
                    parse_file(header_name)
            elif sline.startswith('#define '):
                if p:
                    ls = re.split("\s+", sline, 2)
                    if len(ls) < 3:
                        ls.append("1")
                    _, name, cont = ls
                    macro_list[name] = cont
                    print(line)
            elif sline.startswith('#undef '):
                if p:
                    _, name = re.split("\s+", sline, 1)
                    del macro_list[name]
            elif sline.startswith('#ifdef '):
                if p:
                    _, name = re.split("\s+", sline, 1)
                    cond = name in macro_list
                    e = parse_to_endif(lines, cond)
                    if e  == "#else":
                        parse_to_endif(lines, not cond)
                else:
                    e = parse_to_endif(lines, False)
                    if e  == "#else":
                        parse_to_endif(lines, False)
            elif sline.startswith('#ifndef '):
                if p:
                    _, name = re.split("\s+", sline, 1)
                    cond = name in macro_list
                    e = parse_to_endif(lines, not cond)
                    if e  == "#else":
                        parse_to_endif(lines, cond)
                else:
                    e = parse_to_endif(lines, False)
                    if e  == "#else":
                        parse_to_endif(lines, False)
            elif sline.startswith('#if '):
                if p:
                    print(line)
                    e = parse_to_endif(lines)
                    print(e)
                    while e != "#endif":
                        e = parse_to_endif(lines)
                        print(e)
                else:
                    e = parse_to_endif(lines, False)
                    while e != "#endif":
                        e = parse_to_endif(lines, False)
            else:
                if p:
                    print(line)

    parse_to_endif(lines)
    os.chdir(ori_dir)

filename = None
for arg in sys.argv[1:]:
    if arg.startswith("-D"):
        macro_def = arg[2:].split("=")
        if len(macro_def) < 2:
            macro_def.append("1")
        name, cont = macro_def
        macro_list[name] = cont
        print("#define", name, cont)
    else:
        filename = arg

if filename == None:
    print("\033[;31merror: filename expected", file=sys.stderr)
    exit(1)

parse_file(filename)
