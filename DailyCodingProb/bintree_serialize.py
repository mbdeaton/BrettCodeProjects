#!/usr/bin/env python3.8
"""Daily Coding Problem 3.
Transform a binary tree back and forth from a serial string representation.
"""

import unittest

class Node:
    """Fundamental node for a binary tree"""
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


def serialize(node):
    """Return string representation of a binary tree data structure."""
    tree_ser = ""
    if node != None:
        tree_ser += "Node("        
        tree_ser += repr(node.val)
        tree_ser += "," if node.left!=None else ""
        tree_ser += serialize(node.left)
        tree_ser += "," if node.right!=None else ""
        tree_ser += serialize(node.right)
        tree_ser += ")"
    return tree_ser


def deserialize(string):
    """Return a binary tree data structure from a representative string."""
    node = None
    try:
        node = eval(string)
    except:
        print("Input string is not deserializable as a binary tree node")
    return node


def print_tree(node, pre="_"):
    """Pretty-print the binary tree"""
    if node != None:
        print(" "*len(pre), pre, node.val)
        print_tree(node.left, pre+"._")
        print_tree(node.right, pre+"._")


def find_height(node):
    """Trace the tree to find its height.

    -1 means the root Node is empty
    0  means the root Node is leafless
    """
    height = -1
    if node != None:
        height = 0
        lh = find_height(node.left)
        rh = find_height(node.right)
        height += 1 + max(lh, rh)
    return height


class TestSerializeTree(unittest.TestCase):

    def deserialize_serialize(self, tree, leaf):
        self.assertEqual(eval("deserialize(serialize(tree))."+leaf+".val",
                             {"tree" : tree,
                              "deserialize" : deserialize,
                              "serialize" : serialize}),
                         leaf)

    def test_deserialize_serialize(self):
        tree = Node("root", Node("left", Node("left.left")), Node("right"))
        for leaf in ("left", "left.left", "left.left", "right"):
            self.deserialize_serialize(tree, leaf)

    def test_heights(self):
        self.assertTrue(find_height(Node("root"))==0)
        self.assertTrue(find_height(Node("root",Node("left")))==1)
        tree = Node("root", Node("l", Node("l.l", Node("l.l.l"))))
        self.assertTrue(find_height(tree)==3)


if __name__ == "__main__":
    unittest.main()
