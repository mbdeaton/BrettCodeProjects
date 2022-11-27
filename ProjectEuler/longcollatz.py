#!/usr/bin/env python3
# Brett Deaton - Fall 2019
"""Find the input in range that generates the longest Collatz sequence.

Given a natural number n, the next term in the collatz sequence is
  n/2    n even
  3n+1   n odd

This module implements a brute force solution to Project Euler Problem 14.
"""

__author__ = "Brett Deaton"

import argparse
import math
import numpy as np
import sys

class CollatzFunction:
    """Collatz Function T(k;n).

    T(k;n) = k applications { n/2    n even
                            { 3n+1   n odd
    """
    def __init__(self, k):
        """Set number of iterations, k"""
        if k<1 or not isinstance(k,int):
            sys.exit("Error: k must be a positive integer.")
        else: 
            self.k=k
    def next(self, n):
        """Return next number in the sequence"""
        if n%2:
            next = 3*n+1
        else:
            next = n/2
        return next

def CollatzSequence(n):
    """Collatz sequence starting with input n, ending with 1."""
    seq = [n]
    func = CollatzFunction(k=1)
    # only compute next term if last term isn't a power of 2
    while seq[-1]!=1 and math.frexp(seq[-1])[0]!=0.5:
        seq.append( func.next(seq[-1]) )
    # compute remaining trajectory which is powers of two
    while seq[-1]!=1:
        seq.append( int(seq[-1]/2) )
    return seq

#-------------------------------------------------------------------------------
def main():
    p = argparse.ArgumentParser(
        description = "Find input generating the longest Collatz sequence.")
    p.add_argument("-max", default=10, metavar="N", type=int,
                   help = "int: maximum input N, so the search range is [2,N]")
    p.add_argument("-every", default=None, metavar="M", type=int,
                   help = "int: print every M sequences searched")
    p.add_argument("-sample", default=None, metavar="S", type=int,
                   help = "int: print sequence starting with this input")
    args = p.parse_args()
    
    if args.sample is not None:
        seq = CollatzSequence(args.sample)
        print(len(seq), ":", seq)
    else:
        input_max = 1
        max_length = 1
        for i in range(1,args.max+1):
            seq = CollatzSequence(i)
            if max_length<len(seq):
                input_max=i
                max_length=len(seq)
            if args.every is not None and not i%args.every:
                print(len(seq), ":", seq)
        print ("Longest sequence ( length =", max_length, ") starts with n =", input_max)

    sys.exit(0) # no error on exit

#-------------------------------------------------------------------------------
if __name__ == "__main__":
    main()
