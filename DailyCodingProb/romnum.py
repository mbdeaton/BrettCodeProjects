#!/usr/bin/env python3
"""Daily Coding Problem 216.

Given a number in Roman numeral format, convert it to decimal.
The Roman numerals are (M,    D,   C,   L,  X,  V, I),
which represent values (1000, 500, 100, 50, 10, 5, 1).
"""

import unittest

def romnum(num):
    """Print the roman numeral equivalent to the decimal num"""
    # require int-able input
    try:
        digits = list(map(lambda x: int(x), str(num)))
    except ValueError:
        return None
    this_roman = ""
    # require smallish numbers
    if len(digits) > 4:
        this_roman = None
    else:
        numerals = (("I", "V"), ("X", "L"), ("C", "D"), ("M"))
        # convert three least-significant digits
        for i, d in enumerate(reversed(digits[-3:])):
            ones = numerals[i][0]
            fives = numerals[i][1]
            tens = numerals[i+1][0]
            if d==9:
                this_roman = ones + tens + this_roman
            else:
                this_roman = (d//5)*fives + (d%5)*ones + this_roman
        # convert fourth-most-significant digit
        try:
            this_roman = digits[-4]*numerals[3][0] + this_roman
        except IndexError:
            pass
    return this_roman


class TestRomNum(unittest.TestCase):

    def test_romnum_not_number(self):
        num = "ten"
        self.assertEqual(romnum(num), None)

    def test_romnum_small(self):
        nums = {1:"I", 29:"XXIX", 65:"LXV", 321: "CCCXXI", 894:"DCCCXCIIII"}
        for arabic, roman in nums.items():
            self.assertEqual(romnum(arabic), roman)

    def test_romnum_large(self):
        nums = {1000: "M", 1111: "MCXI", 9999: "MMMMMMMMMCMXCIX"}
        for arabic, roman in nums.items():
            self.assertEqual(romnum(arabic), roman)

    def test_romnum_too_large(self):
        num = 10000
        self.assertEqual(romnum(num), None)

if __name__ == "__main__":
    unittest.main(verbosity=2)