#! /usr/bin/env python3
import os
import re
import subprocess
import argparse
import macro_parser 

flash_base_re = re.compile(r"^.*\s*RE_FLASH_BASE\s*(.*)")
offset_re = re.compile(r"^.*\s*RE_([0-9A-Z_]+)_OFFSET\s*(.*)")

def convert_to_hex(args):
    flash_base = macro_parser.evaluate_macro(args.config, flash_base_re, 0, 1)
    offsets = macro_parser.evaluate_macro(args.config, offset_re, 1, 2)
    act_offset = offsets[args.type] + flash_base
    cmd = args.exe + ' ' + args.infile + ' -Binary -offset ' + str(hex(act_offset)) + ' -o ' + args.outfile + ' -Intel'
    #print(cmd)
    subprocess.run(cmd, stdin=None, input=None, stdout=None, stderr=None, shell=True, timeout=None, check=False)    

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--config', metavar='filename', required=True,
                          help='Location of the file that contains macros')
    parser.add_argument('-t', '--type', metavar='type', required=True,
                          help='SYS_SET / MBL / SYS_STATUS / IMG_0 / IMG_1')
    parser.add_argument('-e', '--exe', metavar='filename', required=True,
                          help='Location of the file that contains macros')    
    parser.add_argument("infile")
    parser.add_argument("outfile")
    
    args = parser.parse_args()
    convert_to_hex(args)

if __name__ == '__main__':
    main()