#!/usr/bin/env python
"""Euler Project Problem 354.
Given a radius R, B(R) gives the number of cells in a unit-side hexagonal
honeycomb with centers exactly L from the origin cell.
Find the number of radii L<L_max such that B(L)=M, with M some natural number
divisible by 6.
"""
import argparse
import math
import numpy as np
import sys

