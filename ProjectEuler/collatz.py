#!/usr/bin/env python
# Brett Deaton - Feb 2019
"""Euler Project Problem 14.

Find the input in range that generates the longest Collatz sequence.
Given a natural number n, the next term in the sequence is
  n/2    n even
  3n+1   n odd
"""

import argparse
import math
import numpy as np
import sys

class CollatzFunction:
    """Collatz Function T(1;n).

    T(k;n) = k applications of { n/2    n even
                               { 3n+1   n odd
    """
    def __init__(self,n):
        r"""Set initial number, n"""
        if n <= 1:
            sys.exit("Error: you must initialize n>1.")
        if not isinstance(n,int):
            sys.exit("Error: n must be an integer.")
        else: 
            self.n=n
    def __iter__(self):
        return self
    def __next__(self):
        r"""Return next number in the sequence"""
        if self.n%2:
            self.n = 3*self.n+1
        else:
            self.n = self.n/2
        return self.n

def CollatzSequence(n):
    """Collatz sequence starting with input n, ending with 1."""
    seq = [n]
    func = iter( CollatzFunction(n) )
    # Only compute next term if last term isn't a power of 2.
    # frexp returns [man,exp] defining a number in base 2 as man*2^exp,
    # where mantissa is a double \in [0.5,1], and exponent is an integer.
    while seq[-1]!=1 and math.frexp(seq[-1])[0]!=0.5:
        seq.append( next(func) )
    # Compute remaining terms in sequence, all powers of two.
    while seq[-1]!=1:
        seq.append( int(seq[-1]/2) )
    return seq

#-------------------------------------------------------------------------------
def main():
    p = argparse.ArgumentParser(
        description = "Find input generating the longest Collatz sequence.")
    p.add_argument("-max", default=None, metavar="N", type=int,
                   help = "int: maximum input N, so the search range is [2,N]")
    p.add_argument("-every", default=None, metavar="M", type=int,
                   help = "int: print every M sequences searched")
    p.add_argument("-sample", default=None, metavar="S", type=int,
                   help = "int: print sequence starting with this input")
    args = p.parse_args()
    
    # Either print the sequence starting at sample S
    if args.sample is not None:
        seq = CollatzSequence(args.sample)
        print format( len(seq), ">6d"), ": ", seq
    # or find the longest sequence with initial values less than max N
    else:
        input_max = 1
        max_length = 1
        for i in range(1,args.max+1):
            seq = CollatzSequence(i)
            if max_length<len(seq):
                input_max=i
                max_length=len(seq)
            if args.every is not None and not i%args.every:
                print format( len(seq), ">6d"), ": ", seq
        print "Longest sequence ( length =", max_length, ") starts with n =", input_max

    exit(0) # no error on exit

#-------------------------------------------------------------------------------
if __name__ == "__main__":
    main()
