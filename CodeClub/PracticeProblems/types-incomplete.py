"""Exploring variable types: str, int, float, bool, list, typecasting."""

# Your task is to build a script that queries the user for any number as input
# and prints that input interpreted as each of the major variable types: str,
# int, float, and bool. The minimal user interface might go something like this:
#   OUT  give me a number any number
#   IN   0.9
#   OUT  here it is interpreted in different ways:
#   OUT  str    0.9
#   OUT  int    0
#   OUT  float  0.9
#   OUT  bool   True

val = float(input("give me a number any number: "))
print("Here it is is interpreted in different ways:")
print("str    ", str(val/3))
print("int    ", int(val/3))
print("float  ", float(val/3))
print("bool   ", bool(val/3))
