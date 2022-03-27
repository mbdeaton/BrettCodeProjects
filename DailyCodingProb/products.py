#!/usr/bin/env python3
"""Daily Coding Problem 2.

Given a list of integers, return a new list such that the i-th element is
the product of all the other elements except the i-th element from the original
list.
"""


import unittest


def products(alist):
    """Return list of products of all the input integers except i-th. O(n)"""
    num_zeroes = 0
    alist_trimmed = alist[:] # copy instead of reference
    while (0 in alist_trimmed) and (num_zeroes<2): # no need to count past 2
        num_zeroes += 1
        alist_trimmed.remove(0)

    rlist = [] # return list
    if num_zeroes > 1:
        rlist = [0]*len(alist)
    else:
        prod = 1
        for num in alist_trimmed:
            prod *= num
        if num_zeroes == 1:
            rlist = [0]*(len(alist)-1) # non positive multiplier produces empty
            i = alist.index(0)
            rlist.insert(i, prod)
        else:
            rlist = [int(prod/num) for num in alist]
    return rlist


def products_no_div(alist):
    """Same as products(), but implemented without division in O(n^2)"""
    rlist = []
    for i in range(len(alist)):
        prod = 1
        for j, num in enumerate(alist):
            if i!=j:
                prod *= num
        rlist.append(prod)
    return rlist


class TestProducts(unittest.TestCase):

    def examples(self, f):
        """"Base test function"""
        self.assertEqual( f([1, 2, 3, 4, 5]),
                          [120, 60, 40, 30, 24] )
        self.assertEqual( f([3, 2, 1]),
                          [2, 3, 6] )
        self.assertEqual( f([0, 1]),
                          [1, 0] )
        self.assertEqual( f([0, 0, 0]),
                          [0, 0, 0] )
        self.assertEqual( f([0, 0, 1e16, 1e16]),
                          [0, 0, 0, 0] )
        self.assertEqual( f([-1, 0, 1]),
                          [0, -1, 0] )

    def test_implementations(self):
        """Test all by looping over implementations"""
        for f in (products, products_no_div):
            self.examples(f)


if __name__ == "__main__":
    unittest.main(verbosity=2)