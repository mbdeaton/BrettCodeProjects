#!/usr/bin/env python3
# Brett Deaton - Fall 2019
"""Executable wrapper for solution to Project Euler Problem 1."""


__author__ = "Brett Deaton"

import msum_eqn
import argparse
import sys

def main():
    p = argparse.ArgumentParser(
        description="Sum the multiples of A and B less than N.")
    p.add_argument("-bound", required=True, metavar='N', type=int,
                   help="upper bound")
    p.add_argument("-factors", required=True, metavar=('A','B'), type=int,
                   nargs=2, help="factors")
    args = p.parse_args()
    N = args.bound
    A, B = args.factors
    msum_eqn.print_sum(A,B,N)

if __name__ == '__main__':
    main()
