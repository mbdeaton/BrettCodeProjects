#!/usr/bin/env python3
# Brett Deaton - Mar 2022
"""Daily Coding Problem 142.
 
Given a string consisting solely of (, ), and *, determine whether the
parentheses are balanced. * can represent either ( or ) or an empty string.
For example, (*) and *()) are balanced, but )*( is not balanced.
"""


import unittest


def find_inner_pair(string, permutation=0):
    """Return the lowest index for the position of a balanced pair of parens.
    
    Balanced inner pairs could take any of four forms: (), (*), (*, *).
    The search proceeds in the order specified by the permutation param.
 
    Args:
        string: an input sequence of (, ), or * characters
        permutation: an int defining the eval order of paren blocks.
          First calculate permutation%3:
          if 0: (), (*), (*, *)
          if 1: (), (*, *), (*)
          if 2: (), *), (*, (*)
 
    Returns:
        (index, span) of the paren block, or (-1, 0) if none is found.
        index: the lowest index of the opening paren of the block
        span: the number of characters in the paren block
    """
    index = string.find("()")
    span = 2
    
    patterns = ["(*)", "(*", "*)"]
    if permutation%3==1:
        patterns = ["(*", "*)", "(*)"]
    elif permutation%3==2:
        patterns = ["*)", "(*", "(*)"]
    for pattern in patterns:
        if index < 0: # only evaluate if no previous matching pair was found
            index = string.find(pattern)
            span = len(pattern)
    if index == -1:
        span = 0
    return index, span
 
 
def parens_balanced(string):
    """Evaluate whether a string of tokens can yield balanced parentheses.
    
    Tokens can be any of these three characters: ()*
    with the wildcard * representing either parenthesis or an empty string.
 
    Args:
        string: an input sequence of (, ), or * characters
    
    Returns:
        True if the input can be interpreted as a balanced set of parantheses.
        An empty string is balanced, so a single * is also balanced.
    """
    # Algorithm: Repeatedly search for inner pairs of parens and trim them
    # out recursively. If we're left with an empty string, input was balanced.
    balanced = False
    if len(string) == 0: # balanced because it's empty
        balanced = True
    elif set(string) == {"*"}: # balanced because it's all *
        balanced = True
    else: # we have to figure out if it's balanced
        for permutation in (0, 1, 2):
            index, span = find_inner_pair(string, permutation)
            if index >= 0:
                trimmed = string[:index] + string[index+span:] # trim inner pair
                balanced = parens_balanced(trimmed) # recurse with trimmed str
                if balanced:
                    break # don't try more permutations if balanced pair found
    return balanced
 

class TestParens(unittest.TestCase):
    def setUp(self):
        self.error_msg = "in {}"
 
    def testFindInnerPair_none(self):
        string = ""
        expected = (-1, 0)
        self.assertEqual(find_inner_pair(string),
                         expected,
                         self.error_msg.format(string))
 
    def testFindInnerPair_simple(self):
        # Test strings with no inner pairs
        examples_no_inner_pairs = ("(", ")", ")(", "))(", ")((")
        expected = (-1, 0)
        for string in examples_no_inner_pairs:
            self.assertEqual(find_inner_pair(string),
                             expected,
                             self.error_msg.format(string))
 
        # Test strings with inner pairs
        # key is input string, val is index and span of first inner pair
        examples_inner_pairs_str_to_index = {
            "()": (0, 2),
            "())": (0, 2),
            "(()": (1, 2),
            "(())": (1, 2),
            "()()": (0, 2),
            "((()))": (2, 2),
            }
        for string, expected in examples_inner_pairs_str_to_index.items():
            self.assertEqual(find_inner_pair(string),
                             expected,
                             self.error_msg.format(string))
    
    def testFindInnerPair_complex(self):
        # Test strings with no inner pairs
        examples_no_inner_pairs = ("*", "*(", ")*", ")*(", "))*(", ")*((")
        expected = (-1, 0)
        for string in examples_no_inner_pairs:
            self.assertEqual(find_inner_pair(string),
                             expected,
                             self.error_msg.format(string))
 
        # Test strings with inner pairs
        # key is input string
        # val is index and span of the first inner pair found with
        #   the default permutation
        examples_inner_pairs_str_to_index = {
            "(*)": (0, 3),    # finds (*)
            "*)": (0, 2),     # finds *)
            "(*": (0, 2),     # finds (*
            "(*))": (0, 3),   # finds (*)
            "((*)": (1, 3),   # finds (*)
            "(*()": (2, 2),   # finds ()
            "()*)": (0, 2),   # finds ()
            "(*()*)": (2, 2), # finds ()
            }
        for string, expected in examples_inner_pairs_str_to_index.items():
            self.assertEqual(find_inner_pair(string),
                             expected,
                             self.error_msg.format(string))
 
    def testParensBalanced_none(self):
        string = ""
        self.assertTrue(parens_balanced(string),
                        self.error_msg.format(string))
    
    def testParensBalanced_simple(self):
        examples_unbalanced = ("(", ")", ")(", ")()(", "(()", "())")
        for string in examples_unbalanced:
            self.assertFalse(parens_balanced(string),
                             self.error_msg.format(string))
 
        examples_balanced = ("()", "(())", "()()", "(()())", "(())()", "()(())")
        for string in examples_balanced:
            self.assertTrue(parens_balanced(string),
                            self.error_msg.format(string))
 
    def testParensBalanced_complex(self):
        examples_unbalanced = ("*(", ")*", ")*(", ")**(", ")*(*", "*)*(", "(((*)", "(*)))")
        for string in examples_unbalanced:
            self.assertFalse(parens_balanced(string),
                             self.error_msg.format(string))
 
        examples_balanced = ("*", "(*", "*)", "(*)", "(**)", "*(*)", "(*)*", "(*(*", "*)*)")
        for string in examples_balanced:
            self.assertTrue(parens_balanced(string),
                            self.error_msg.format(string))


if __name__ == "__main__":
    unittest.main(verbosity=2)
