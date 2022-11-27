#!/usr/bin/env python3
# Brett Deaton - Fall 2019
"""Find the sum of the even fibonacci terms less than or equal to 4 million.

Our fibonacci terms a_n are
  a_0 = 1
  a_1 = 2
  a_n = a_{n-2}+a_{n-1}

This executable implements a brute_force solution to Project Euler Problem 2,
and also introduces some exploratory functions.
"""

__author__ = "Brett Deaton"

import sys

class FibSeq:
    """Fibonacci sequence as an iterator."""
    def __iter__(self):
        self.a_p = 0 # previous term
        self.a = 1   # current term
        return self

    def __next__(self):
        temp = self.a
        self.a = self.a_p+self.a
        self.a_p = temp
        return self.a_p # return previous term

def fibmods(n):
    """Print the Fibonacci terms not greater than n, modulo some number."""
    i = 1       # current term index [0,1,2,...]
    i_max = 100 # to fend off the endless loop
    a_p = int(input("Enter first term f_0: "))  # e.g. 1
    a   = int(input("Enter second term f_1: ")) # e.g. 1
    print ("n\tf_n\tf_n%2\tf_n%3\tf_n%5\tf_n%8\tf_n%13")
    print(i-1, a_p, a_p%2, a_p%3, a_p%5, a_p%8, a_p%13, sep='\t')
    while a<=n:
        print(i, a, a%2, a%3, a%5, a%8, a%13, sep='\t')
        i+=1
        temp = a
        a = a + a_p
        a_p = temp
        if i > i_max:
            err = 1
            break
    
def fibsum_evens(n):
    """Sum the even Fibonacci terms not greater than n."""
    err = 0     # error code, 0 means all is well
    i = 1       # current term index [0,1,2,...]
    i_max = 100 # to fend off the endless loop
    a = 2       # current term
    a_p = 1     # previous term
    tot = 0     # cumulative sum
    print ("i\ta_p\ta\ttot")
    while a<=n:
        if a%2==0:
            tot += a
        print (i, a_p, a, tot, sep='\t')
        i+=1
        temp = a
        a = a + a_p
        a_p = temp
        if i > i_max:
            err = 1
            break
    print ("Sum of even fibonacci terms <=", n,
           " (i.e. up to the", i, "th term):", tot)
    sys.exit(err)

if __name__ == "__main__":
    n = int(input("Enter an inclusive upper bound: "))
    fibsum_evens(n)
