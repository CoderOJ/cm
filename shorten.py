#!/usr/bin/env python3

import re
from os import popen

token_keep = [
    "__FILE__",
    "__LINE__",
    "__FUNCTION__",
    "__cplusplus",
    "__attribute__",
    "__gcd",
    "__builtin_popcount",
    "__builtin_popcountll",
    "__builtin_clz",
    "__builtin_clzl",
    "__builtin_clzll",
    "__builtin_ctz",
    "__builtin_ctzl",
    "__builtin_ctzll",
    "__int128_t",
    "__uint128_t",
]

def get_tokens(code):
    result = {}
    def add_item(item):
        if item != "" and item[0] == "_":
            if item in result:
                result[item] += 1
            else:
                result[item] = 1
    cur = ""
    for c in code:
        if re.match("[0-9a-zA-Z_]", c):
            cur += c
        else:
            add_item(cur)
            cur = ""
    add_item(cur)
    return result

def replace_tokens(code, tr):
    result = []
    def add_item(item):
        if item in tr:
            result.append(tr[item])
        else:
            result.append(item)
    cur = ""
    for c in code:
        if re.match("[0-9a-zA-Z_]", c):
            cur += c
        else:
            add_item(cur)
            cur = ""
            result.append(c)
    add_item(cur)
    return "".join(result)


def get_replaces(tokens):
    def gen_tokens():
        c = 1
        while True:
            t = hex(c)[2:]
            yield "_" + t
            c += 1
    token_list = list(tokens.keys())
    token_list.sort(key = lambda t: tokens[t])
    r_token = gen_tokens()
    result = {}
    for t in token_list:
        if t in token_keep:
            result[t] = t
        else:
            result[t] = next(r_token)
    return result

def do_file(file, min_filename, relable = True):

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
    res_lines = []
    def refresh(res_lines, cur):
        cur = compress(cur)
        if cur != "":
            res_lines.append(cur)

    get_line = iter(file)
    for line in get_line:
        while line.endswith("\\"):
            line = line[:-1].strip(" ") + " " + next(get_line).strip(" ")
        line = list(map(lambda s: s.strip(" "), line.split("//", 1)))
        if len(line) < 2:
            line.append("")
        [line, comment] = line
        if comment == "relable off":
            relable = False
        if line == "":
            continue
        if line.startswith("#"):
            refresh(res_lines, cur)
            cur = ""
            res_lines.append(line)
        else:
            cur += " " + line
    refresh(res_lines, cur)

    res = "\n".join(res_lines)
    if relable:
        tr = get_replaces(get_tokens(res))
        res = replace_tokens(res, tr)

    if min_filename != None:
        output = open(min_filename, "w")
        print(res, file=output)
    else:
        return res

if __name__ == "__main__":
    files = popen("ls", "r").read().split("\n")
    for file in files:
        if file.endswith(".hpp") and not file.endswith(".min.hpp"):
            min_filename = file[:-4]
            lines = map(lambda s: s.strip("\n"), open(file).readlines())
            do_file(lines, min_filename)
