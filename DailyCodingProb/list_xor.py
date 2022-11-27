#!/usr/bin/env python3
# Brett Deaton - Nov 2021
"""Daily Coding Problem 6.

Implement a doubly-linked list, storing only a single address,
the xor of next and prev nodes.
"""


import unittest


def get_pointer(node):
    return id(node)


def dereference_pointer(address):
    return None


class Xlist():
    """A doubly-linked list, implemented with xor of next and prev."""
    def __init__(self):
        self.elements = {}

    def add(self, element):
        index = get_pointer(element)
        self.elements[index] = element

    def get(self, index):
        return self.elements[index]


class TestXlist(unittest.TestCase):
    def test_empty(self):
        lista = Xlist()
        self.assertIsInstance(lista, Xlist)
        with self.assertRaises(KeyError):
            lista.get(0)

    def test_adding_elements(self):
        lista = Xlist()
        lista.add("the")
        lista.add("quick")
        self.assertEqual(lista.get(0), "the")
        self.assertEqual(lista.get(1), "quick")


if __name__ == "__main__":
    unittest.main(verbosity=2)
