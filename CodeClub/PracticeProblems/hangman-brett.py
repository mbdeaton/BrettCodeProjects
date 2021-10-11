"""Hangman Game."""

# Your task is to build a game in which the user tries to guess all the letters
# in a phrase. The minimal user interface might go something like this:
#   OUT  the phrase is "##### ### #####", guess a letter
#   IN   e
#   OUT  no, your fail count is 1, guess a letter
#   IN   r
#   OUT  no, your fail count is 2, guess a letter
#   IN   a
#   OUT  yes, the phrase is "##a## a## #####", guess a letter

# This script fragment needs at least the following modifications to complete
# the task:
#   1) display the discovered phrase
#   2) add ability to guess entire phrase
#   3) tell the player what state the man is in
#   4) tell player when they have won or lost and ask to play again

from random import randint
import sys

phrases = ["there is a house over there",
          "the dog was very fast",
          "jack and jill went up the hill"]
number = randint(0,len(phrases)-1)
chosen_phrase = phrases[number]
player_guesses = []
incorrect_guesses = 0

while True:
    print()

    # Check for error input
    user_input = input("Guess a letter: ")
    if user_input=="quit" or user_input=="exit": sys.exit()
    if len(user_input)!=1:
        print("Invalid input.")
        continue
    if user_input in player_guesses:
        print("You already guessed that letter.")
        break

    player_guesses.append(user_input)

    # Print phrase guessed so far
    missing_chars = 0
    for char in chosen_phrase:
        if char in player_guesses or char.isspace():
            print(char, end='')
        else:
            print('#', end='')
            missing_chars += 1
    print()

    # Check for win
    if missing_chars==0:
        print("You win!")
        break

    # Track incorrect guesses
    if not user_input in chosen_phrase:
        incorrect_guesses += 1
        print("Wrong. You have guessed wrong", incorrect_guesses, "times.")

    # Check for lose
    if incorrect_guesses>=6:
        print("You're hung.")
        break
