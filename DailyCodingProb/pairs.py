#!/usr/bin/env python3
"""Daily Coding Problem 5.

Given the following implementation of a pair constructor cons(), extract the
elements of the pair, individually.
"""


import unittest


def cons(a,b):
    """Return a pair."""
    def pair(f):
        return f(a,b)
    return pair


def car(p):
    """Return the first element in a pair."""
    return p(lambda x,y: x)


def cdr(p):
    """Return the last element in a pair."""
    return p(lambda x,y: y)


class TestPairs(unittest.TestCase):
    def test_numerical_examples(self):
        self.assertTrue( car(cons(3,4))==3 )
        self.assertTrue( cdr(cons(3,4))==4 )

    def test_string_examples(self):
        self.assertTrue( car(cons("a","b"))=="a" )
        self.assertTrue( cdr(cons("a","b"))=="b" )


if __name__ == "__main__":
    unittest.main(verbosity=2)