# Programming Problems
Ideas (beyond Project Euler) for programming problems to use in code club.

#### Basic Problems
* **Input and output**.
  1. Code up a number guessing game. Pick a random number between 1 and 100.
     Repeatedly query the user for guess, and print "too high", "too low",
     "yes!", or "I don't understand your input". See `guessing_game.py`.
* **If statements**: if, if-else, if-elif-else, nested ifs.
  1. See `if_statements.py`.
* **Basic types**: str, int, float, bool, typecasting.
  1. Given an input string, return another string appending "not " to the front,
     unless the input string begins with "not", in which case return as is.
  2. Given an input number x, return True if it is an integer, False if not.
  3. Given an input integer n, return n/2 if even, (n-1)/2 if odd, "your input
     is not an integer" if neither.
* **Formatting output**: str.format().
  1. Retrieve the user's name and height in inches. Print the following string,
      with only 2 decimal places after the height: "[name]'s height is [x]
      meters."
* **Arrays**: lists, tuples, sets, dictionaries.
  1. See `hangman.py`.
  2. See `manipulating_strings.py`.
  3. Given an array of some length and with random values, write an algorithm
     that sorts the array from smallest to largest.
  4. Given a sorted array, use binary search to find an item.
* **Loops**: for i in range(), for i in iterable, while.

#### More Advanced Problems
* **Objects**: str, dates.
* **Functions**.
* **Comments and docstrings**.
* **Errors**: exit codes, try/except.
* **Defining your own types**: classes.

#### SEL-specific
* Input build rate (units/hr) and output total number of of units built over
  a different time interval with a different number of workers.
* Computes the voltage difference between any two phases of a 3-phase power
  system, at any given time. This should allow the user to explore questions
  like: is the voltage difference constant in time? etc.
* Input density or water concentration in potting resin for transformers, and
  output whether it needs refreshing or not. (Ask Trini.)
* Compute some summarizing information about a living database: defect rate,
  build rate, etc.
* James in Shipping has built an Excel program to generate a placard with some
  info on it. We could do something similar for other repeated display problems.
* Compare and clean up AX database with shipping info in one field mismatched
  from info in another field.
* Optimize the number of unique reel setups in SMT by designing some efficient
  hybrid reel setups. (Talk to Alan Nelson.)

#### Advanced Applications
* Compare two lists for overlapping elements, and report some relevant stats.
* Use pandas to do some data analysis on a spreadsheet.
* Download and print some data from a website.
