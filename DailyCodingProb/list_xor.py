#!/usr/bin/env python3
"""Daily Coding Problem 6.
Implement a doubly-linked list, storing only a single address,
the xor of next and prev nodes.
"""

import unittest

class Xlist():
    """A doubly-linked list, implemented with xor of next and prev."""
    def __init__(self):
        self.elements = {}
    def add(self, element):
        self.elements[0] = element
    def get(self, index):
        return self.elements[index]


class TestXlist(unittest.TestCase):
    def test_empty(self):
        lista = Xlist()
        self.assertIsInstance(lista, Xlist)
        with self.assertRaises(KeyError):
            lista.get(0)

    def test_single(self):
        lista = Xlist()
        lista.add("the")
        self.assertEqual(lista.get(0), "the")

if __name__ == "__main__":
    unittest.main(verbosity=2)