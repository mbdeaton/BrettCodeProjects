#!/usr/bin/env python3
"""Find the prime factorization of a number.

This module implements a recursive solution to Project Euler Problem 3.
"""

__author__ = "Brett Deaton"

import math

def prime_list(n):
    """Return list of all primes not greater than n.

    This is not working yet!
    """
    p = []
    if n>=2:
        p = [2]
        for x in range(3,n+1):
            maybe_prime = True
            for k in p:
                if k>=math.sqrt(x): break
                if x%k:
                    maybe_prime = False
                    break
            if maybe_prime:
                p.append(x)
    return p

def smallest_factor(n):
    """Find the smallest factor of n. Return n if n is prime or 1."""
    if n==1:
        return n
    else:
        f=2
        while n%f != 0:         # search for the smallest factor of n
            if f>=math.sqrt(n): # more obvious and costly: compare to n/2 or n/f
                return n
            f+=1
        return f

def factor_recursive(n):
    """Print the prime factors of n.

    Generate n's factor tree by printing its smallest factor f, then recursing
    on r=n/f. If n==1 do nothing.
    """
    f = smallest_factor(n)
    if f!=1:
        print(f, end=' ')
    r = n//f # we know n divisible by f, so store it as int
    if r!=1 and r!=n:
        factor_recursive(r)

def factor_loop(n):
    """Print the prime factors of n.

    Generate n's factor tree. If n==1 do nothing.
    """
    p = [] # prime factors n
    f = 1
    while n!=f:
        f = smallest_factor(n)
        if f==n:
            break
        p.append(f)
        n = n//f # we know n divisible by f, so store it as int
    if f!=1:
        p.append(f)
    print(p)

if __name__ == "__main__":
    """Factor the script argument."""
    n = int(input("Enter a number to factor: "))
    print("Recursive method:")
    factor_recursive(n)
    print()
    print("Loop method:")
    factor_loop(n)
