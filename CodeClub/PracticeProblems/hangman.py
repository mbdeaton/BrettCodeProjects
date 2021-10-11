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

phrases = ["there is a house over there",
          "the dog was very fast",
          "jack and jill went up the hill"]

from random import randint
number = randint(0,len(phrases)-1)
chosen_phrase = phrases[number]
player_guesses = []

while True:
    player_input = input("Enter a letter: ")
    player_guesses.append(player_input)
    if player_input in chosen_phrase:
        print("that letter is in the phrase")
    else:
        print("that letter is not in the phrase")
