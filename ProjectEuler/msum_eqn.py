#!/usr/bin/env python3
# Brett Deaton - Fall 2019
"""Compute the sum of multiples of A and B less than N.

The total sum is the sum of multiples of A, plus those of B, less those of AB:
  sum = sum(N,A) + sum(N,B) - sum(N,AB)
where
  sum(N,X) = X + 2X + 3X + ... + floor(N/X)*X

This module implements a closed-form solution to Project Euler Problem 1.
"""


__author__ = "Brett Deaton"

def sum_factors_less_than(N, X):
    """Sum the series X+2X+3X+...+maxX, terminating with maxX<N<(maxX+X).

    Compute it from the equivalent series of constant half-terms, where the
    half-term is the sum of the first and last term divided by 2. Both series
    have floor((N-1)/X) terms.
    """
    max_X = (N-1)-(N-1)%X
    half_term = (X+max_X)/2
    num_terms = (N-1)//X
    return int(half_term*num_terms)

def print_sum(A,B,N):
    """Print sum of the multiples of A and B less than N."""
    sum_As  = sum_factors_less_than(N, A)
    sum_Bs  = sum_factors_less_than(N, B)
    sum_ABs = sum_factors_less_than(N, A*B)
    sum_tot = sum_As + sum_Bs - sum_ABs
    print("Sum of multiples of", A, "and", B, "less than", N, ":", sum_tot)

if __name__ == '__main__':
    A = int(input("Enter a factor: "))
    B = int(input("Enter another factor: "))
    N = int(input("Enter an upper bound: "))
    print_sum(A,B,N)
