#!/usr/bin/env python3

import os
import sys
import re


class colors:
    CLEAR = "\033[0m"
    BLACK = "\033[30m"
    RED = "\033[31m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    BLUE = "\033[34m"
    MAGENTA = "\033[35m"
    CYAN = "\033[36m"
    WHITE = "\033[37m"


def print_color(text, color):
    print(color + text + colors.CLEAR)


args_list = []
compiler_list = []
linker_list = []

args_list.append("-DINDEX_64_BIT=1")

for arg in sys.argv[1:]:
    # Compiler:
    if arg[0:2] == "-I" or arg[0:2] == "-f":
        compiler_list.append(arg)
    elif arg[0:3] == "-Wl":
        linker_list.append(arg)
    elif arg[0:2] == "-D":
        wrapped_arg = re.sub('"(.*)"', r'"\"\1\""', arg)
        args_list.append(wrapped_arg)
    # Linker:
    elif arg[0:2].upper() == "-L":
        linker_list.append(arg)
    # Other:
    else:
        args_list.append(arg)

args = " ".join(args_list)
compiler_args = ",".join(compiler_list)
linker_args = ",".join(linker_list)

command = (
    "nvcc " + args + " -Xcompiler " + compiler_args + " -Xlinker " + linker_args
)

print_color("\n" + command + "\n", colors.RED)

os.system(command + " -w")
