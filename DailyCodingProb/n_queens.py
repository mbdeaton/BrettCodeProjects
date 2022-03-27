#!/usr/bin/env python3
"""Daily Coding Problem 38.

Count the number of unique configurations of n nonattacking queens on an n*n
chess board. See [OEIS A000170](https://oeis.org/A000170).
"""


from random import randrange
from string import ascii_lowercase
from typing import List
import unittest


class NQueensConfig:
    """Configs of n queens each on a distinct column of an n*n chessboard.
    
    Each queen has her own column numbered 0..n-1, and varies her position on
    the rows numbered 0..n-1. Valid configurations are those wherein no queen
    is in immediate danger.
    """
    def __init__(self, n: int):
        """Args:
            n: int specifying the number of queens and side length of the board
        """
        self.n = n
        self.queens = [0]*self.n

    def num_nonattacking_configs(self, check_col: int = 0) -> int:
        """Counts arrangements of n nonattacking queens on n*n chessboard.
        
        Start the search assuming all queens on columns < check_col are in a
        valid configuration, and only move queens on check_col or higher. When
        the search begins forward of any valid configurations, the count
        ignores these. For definition of "forward" see nudge_config().
        For n>12, the computation takes a *long* time. Complexity is O(n!),
        I think.
        
        Args:
            check_col: search board above this int column, and freeze lower
              queens (def: 0, meaning all possible configurations are found)
            
        Returns:
            An integer specifying the number of valid configurations above the
            current configuration.
        """
        if check_col == self.n:
            return 1 if self.is_valid() else 0
        current_config = self.queens
        if check_col == 0:
            self.set_config() # begin search from standard config
            
        count = 0
        for test_row in self.blank_rows(check_col):
            self.queens[check_col] = test_row
            count += self.num_nonattacking_configs(check_col+1)
        self.queens[check_col] = 0
        
        if check_col == 0:
            self.queens = current_config # return to pre-search state
        return count

    def blank_rows(self, check_col: int = 0) -> List[int]:
        """Returns list of subsequent rows unthreathened by lower queens.
        
        The order of rows returned is sequential, starting from the current
        position of check_col's queen. (That is, we never return rows lower
        than the current row.) Complexity is O(n^3), I think.
        
        Args:
            check_col: find free rows on this int column, only comparing
              against lower queens (def: 0, meaning all rows are returned)
        
        Returns:
            A list of integers, the list having varying length (but always less
            than n). The integers specify the rows in the current column
            that are free from attack by lower queens.
        """
        start_row = self.queens[check_col]
        blanks = []
        for i in range(start_row, self.n):
            self.queens[check_col] = i
            if self.is_valid(check_col):
                blanks.append(i)
        self.queens[check_col] = start_row
        return blanks
    
    def is_valid(self, check_col: int = -1) -> bool:
        """Returns false if queen in check_col is in danger from lower queens.
        
        If check_col<0 or check_col>n, check validity of all queens.
        Complexity is O(n^2), I think.
        
        Args:
            check_col: int column of the checked queen, higher queens are
              ignored when check_col is within bounds (def: -1, meaning check
              validity of all queens)
        
        Returns:
            A bool specifying whether the examined queens are safe.
        """
        def col_is_valid(self, c: int) -> bool:
            """Returns false if queen in col c in danger from lower queens."""
            check_queen = self.queens[c] # row of current queen
            check_rows = self.queens[:c] # rows of lower queens
            valid = check_queen not in check_rows
            slopes = list(map(lambda x,y: (check_queen-y)/(c-x),
                              range(c),
                              check_rows))
            EPS = 1e-3
            clear_diagonals = list(map(lambda s: abs(abs(s)-1)>EPS, slopes))
            valid &= all(clear_diagonals)
            if not valid:
                return False
            else:
                return True

        if check_col == 0:
            return True
        elif check_col>0 and check_col<self.n:
            return col_is_valid(self, check_col)
        else:
            for c in reversed(range(self.n)):
                if not col_is_valid(self, c):
                    return False
            return True

    def set_config(self, queens=None) -> None:
        """Moves each column's queen to the row given in the list queens.
        
        Args:
            queens: list of n integers within the range [0,n-1] inclusive.
              (def: set to minimum config, i.e. all queens on row 0)
        """
        if queens is None:
            self.queens = [0]*self.n
        else:
            if len(queens) != self.n:
                raise ValueError(f"arg has length {str(len(queens))}," +
                                 f"expected length {str(self.n)}")
            else:
                self.queens = queens
            for col, row in enumerate(self.queens):
                if row<0 or row>(self.n-1):
                    raise ValueError(f"the {col}-th queen, set to " +
                                     f"row {row}, is out of bounds")

    def find_next_random_config(self) -> None:
        """Sets to random configuration."""
        self.queens = [randrange(self.n) for i in range(self.n)]
        
    def find_next_nonattacking_config(self, check_col: int = 0) -> bool:
        """Steps forward from current config, to find next valid config.

        Start the search assuming all queens on columns<check_col are in a
        valid configuration, and only move queens on check_col and higher.
        Note, the default search always begins one config forward of the
        current config; if the queens are currently in their final config,
        the search begins again at the beginning. The sense of "forward" is
        specified in method nudge_config(). Complexity is ... a lot better
        than num_nonattacking_configs(), but not sure what ...
        
        Args:
            check_col: freeze all queens below this column (def: all queens
              can move)
        
        Returns:
            A bool specifying whether a valid configuration was found above
            the current config.
        """
        if check_col == self.n:
            return self.is_valid()
        if check_col == 0:
            self.nudge_config() # begin search from nudged initial config
        for test_row in self.blank_rows(check_col):
            self.queens[check_col] = test_row
            if self.find_next_nonattacking_config(check_col+1):
                return True
        self.queens[check_col] = 0
        return False

    def nudge_config(self, nudge: int = 1) -> None:
        """Move the config forward by the amount specified in nudge.
        
        The sense of "forward" is increasing numerical value, with rows giving
        digits and columns giving places (zeroth column the most significant).
        If the nudge pushes the configuration x steps past its max state
        (i.e. all queens on the (n-1)th row) wrap to x-1 steps past min state
        (i.e. all queens on the 0th row).
        
        Args:
            nudge: int specifying number of moves to add to current config
              (def: 1)
        """
        # Algorithm:
        #   1. convert config to a base-n number
        #   2. increase the number by nudge
        #   3. convert base-n number back to a config
        values = [x*self.n**(self.n-i) for i, x in enumerate(self.queens, 1)]
        total = sum(values)
        total += nudge
        total %= self.n**self.n
        config = []
        for i in range(self.n):
            base = self.n
            powr = base-i-1
            div, mod = divmod(total, base**powr)
            config.append(div)
            total = mod
        self.queens = config

    def display_blank_board(self, algebraic: bool = False) -> None:
        """Prints a blank board with col,row coordinates.
        
        Args:
            algebraic: bool specifying whether to print algebraic coordinates
              (def: False, meaning print cartesian coordinates)
        """
        start = 1 if algebraic else 0
        rows = tuple(reversed(range(start,self.n+start)))
        cols = tuple(ascii_lowercase[:self.n]) if algebraic else tuple(reversed(rows))
        for row in rows:
            print(row, " ".join(list("."*self.n)))
        print(" ", *cols)

    def display_config(self) -> None:
        """Prints the current configuration on a display board."""
        for row in reversed(range(self.n)):
            print(' '.join("Q" if q==row else "." for q in self.queens))


class TestNQueens(unittest.TestCase):

    def test_total_num_valid_configs(self):
        calct = [NQueensConfig(n).num_nonattacking_configs() for n in range(10)]
        expct = [1, 1, 0, 0, 2, 10, 4, 40, 92, 352]
        self.assertEqual(calct, expct)


if __name__ == "__main__":
    unittest.main(verbosity=2)