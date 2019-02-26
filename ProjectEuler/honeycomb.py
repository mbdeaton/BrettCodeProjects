#!/usr/bin/env python
"""Euler Project Problem 354.
Given a radius R, B(R) gives the number of cells in a unit-side hexagonal
honeycomb with centers exactly L from the origin cell.
Find the number of radii L<L_max such that B(L)=M, with M some natural number
divisible by 6.
"""
import math
import sys

class RotatedEllipse:
    r"""Rotated Ellipse satisfying a^2+b^2+ab=R^2/3.

    (a,b)    cell address of unit-side honeycomb
    N=a+b    cell column index
    """
    def __init__(self,R):
        r"""Set initial radius, R"""
        if R<=1:
            sys.exit("Error: you must initialize R>0.")
        if R>3e8:
            # Note, this may not be a problem; I haven't researched it yet.
            sys.exit("Error: R^2 too large for double format.")
        else:
            self.R = R
            self.Rvar = 4*R**2/3
            # min N and max N possibly continaing a solution
            self.minN = math.ceil(R/math.sqrt(3))
            self.maxN = math.floor(2*R/3)
    def b(self,a):
        r"""Compute b=sqrt(4R^2/3-3a^2)/2-a"""
        if a<0:
            sys.exit("Error: we must have a>0.")
        return (math.sqrt(self.Rvar-3*a**2)-a)/2

#-------------------------------------------------------------------------------
def main():
    exit(0) # no error on exit

#-------------------------------------------------------------------------------
if __name__ == "__main__":
    main()
