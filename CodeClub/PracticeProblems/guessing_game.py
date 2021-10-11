"""Number guessing game."""

# Your task is to build a game in which the user tries to determine what random
# number was chosen by the computer. The minimal user interface might go
# something like this:
#   OUT  guess my number
#   IN   37
#   OUT  too high, guess again
#   IN   10
#   OUT  too low, guess again
#   IN   23
#   OUT  yes!

# This script fragment needs at least the following modifications to complete
# the task:
#   1) use a random number in the range [1,100] (for an example see
#      https://stackoverflow.com/questions/3996904/generate-random-integers-between-0-and-9),
#   2) repeatedly query the user for a guess,
#   3) report "too high", "too low", "yes!", or "I don't understand your input".

name = input("What's your name? ")
print("Let's play a game, ", name, ".", sep='')
number = 73
guess = int(input("Guess my number: "))
if guess==number:
    print("Yes!")
else:
    print("Nope.")
