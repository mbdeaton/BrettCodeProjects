#!/usr/bin/env python3
"""Sum the multiples of 3 or 5 less than some upper bound.

This script implements a brute force solution to Project Euler Problem 1.
"""

__author__ = "Brett Deaton"

upper = int(input("enter upper bound: "))
fac1 = 3
fac2 = 5
tot = 0

for i in range(1,upper):
    if i%fac1 and i%fac2:
        pass
    else:
        tot += i
print ("sum of multiples of", fac1, "or", fac2, "less than", upper, ":", tot)
