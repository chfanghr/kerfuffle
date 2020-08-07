#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os


dir_cont_sep = "│"
entry_sep = "├─ "
last_entry_sep = "└─ "
dir_color = "\033[1;34m"
end_color = "\033[0m"



def add_file(d, f):
    for dir in f[:-1]:
        if dir not in d:
            d[dir] = {}
        d = d[dir]
    d[f[-1]] = f[-1]

def color_dir_name(dirname):
    return dir_color + dirname + end_color


def print_dir(directory, prefix=""):
    if not directory:
        return
    sep = entry_sep
    dir_sep = dir_cont_sep
    entries = list(directory)
    for filename in entries:
        if filename is entries[-1]:
            sep = last_entry_sep
            dir_sep = ""
        if isinstance(directory[filename], dict):
            print (prefix + sep + color_dir_name(filename))
            print_dir(directory[filename], prefix + dir_sep + "  ")
        else:
            print (prefix + sep + filename)


def tree_from_listing(listing, prim_dir=None):
    files = [file.strip().split(os.sep) for file in listing]
    d = {}
    for file in files:
        add_file(d, file)
    if prim_dir is not None:
        print (color_dir_name(prim_dir))
    print_dir(d)

def get_listing_from_stdin():
    return [line for line in sys.stdin.readlines()]

tree_from_listing(get_listing_from_stdin(), ".")
