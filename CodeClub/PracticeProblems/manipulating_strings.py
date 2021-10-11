"""Mangling a name."""

# Write a program that asks for a user's name, then prints some silly
# manipulations of the name. The minimal user interface might go something like
# this:
#   OUT  What's your name?
#   IN   Goliath
#   OUT  htailoG
#   OUT  Glah
#   OUT  gOLIATH

# This script fragment needs at least the following modifications to complete
# the task:
#   1) print the name backward,
#   2) print every other letter in the name,
#   3) print the name with the first letter lowercase and the rest uppercase
#      (for an example see
#      https://stackoverflow.com/questions/6797984/how-do-i-lowercase-a-string-in-python).

# Note, you are NOT allowed to use these three lines of code ;)
#print(name[::-1])
#print(name[::2])
#print(name[0].lower(), name[1:].upper(), sep='')

#################################################
# Get input from the user

name = input("What is your name?: ")
print("Your name is ...")
print(name)

length = len(name)
print("with", length, "characters in it")

first_letter = name[0]
print("and the first is", first_letter)
