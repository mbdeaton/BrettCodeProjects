#!/usr/bin/env python3
# Brett Deaton - Oct 2021
"""Daily Coding Problem 4.

Given a list of integers, find the smallest whole number missing from the list.
"""

# TO DO:
# Implement a solution in O(n) time and O(1) space.

import unittest

def find_missing_int(alist):
    """Find smallest whole number missing from alist.
    
    Implemented in O(nlogn) time, since CPython uses timsort.
    """
    alist.sort()
    start = 0
    try:
        prev = 1
        start = alist.index(prev)
        for num in alist[start+1:]:
            if num-prev>1:
                return prev+1
            prev = num
        return alist[-1]+1 # didn't return so list must be continuous
    except ValueError:
        return 1


def find_missing_int_quick(alist):
    """Same as find_missing_int(), but in O(n) time and O(1) space."""
    # Probably some xor approach
    return None


class TestFindMissingInt(unittest.TestCase):
    def test_examples(self):
        self.assertTrue( find_missing_int([3, 4, -1, 1]) == 2 )
        self.assertTrue( find_missing_int([1, 2, 0]) == 3 )


if __name__ == "__main__":
    unittest.main(verbosity=2)
