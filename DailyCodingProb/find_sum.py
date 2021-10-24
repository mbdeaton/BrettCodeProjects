#!/usr/bin/env python3.8
"""Daily Coding Problem 1.
Given a list of numbers and a number k, return whether two numbers in the list
sum to k.
"""

import unittest

def find_sum(asum, alist):
    """Return whether alist contains two numbers that add to asum"""
    for i, first in enumerate(alist):
        for second in alist[i+1:]:
            if first+second == asum:
                return True
    return False


class TestFindSum(unittest.TestCase):

    def test_example(self):
        a = [10, 15, 3, 7]
        self.assertTrue( find_sum(17,a) )
        self.assertFalse( find_sum(12,a) )

    def test_empty(self):
        a = []
        self.assertFalse( find_sum(0,a) )
        self.assertFalse( find_sum(1,a) )

    def test_range(self):
        bot, top = 0, 10
        a = list(range(bot,top+1))
        for i in range(bot+1, 2*top):
            self.assertTrue( find_sum(i,a) )


if __name__ == "__main__":
    unittest.main()
