#!/usr/bin/env python
# A simply Python TicTacToe game using the turtle package for drawing.
# Initially taken from Jedidiah Bartlett's edit of stack-exchage post:
#   https://stackoverflow.com/a/49785949/6146369
from turtle import Turtle, Screen
CURSOR_SIZE = 20
SQUARE_SIZE = 50
FONT_SIZE = 30
FONT = ('Arial', FONT_SIZE, 'bold')
class TicTacToe:
    def __init__(self):
        # Setup 3x3 board to track state and initialize it all to '?'
        self.boardState = [['?' for i in range(3)] for j in range(3)]
        # Turtle Objects representing the visual state of the board
        self.boardTiles = []
        # Initialize the turn variable to start with player 'X'
        self.turn = 'X'
    def drawBoard(self):
        background = Turtle('square')
        background.shapesize(SQUARE_SIZE * 3 / CURSOR_SIZE)
        background.color('black')
        background.stamp()
        background.hideturtle()
        for j in range(3):
            lineOfTurtles = []
            for i in range(3):
                box = Turtle('square', visible=False)
                box.shapesize(SQUARE_SIZE / CURSOR_SIZE)
                box.color('white')
                box.penup()
                box.goto(i * (SQUARE_SIZE + 2) - (SQUARE_SIZE + 2),
                         j * (SQUARE_SIZE + 2) - (SQUARE_SIZE + 2))
                box.showturtle()
                box.stamp()  # blank out background behind turtle (for later)
                box.onclick(lambda x, y, box=box, i=i, j=j: self.mouse(box, i, j))
                lineOfTurtles.append(box)
            self.boardTiles.append(lineOfTurtles)
    def mouse(self, box, i, j):
        box.onclick(None)  # disable further moves on this square by overwriting lambda
        # replace square/turtle with (written) X or O
        box.hideturtle()
        box.color('black')
        box.sety(box.ycor() - FONT_SIZE / 2 - 10 ) # Need to move the letters down 10 points
        box.write(self.turn, align='center', font=FONT)
        self.boardState[j][i] = self.turn  # record move
        # Game logic goes here: (maybe a function call?)
    
        # switch turns, evaluate truthiness of expression "is it X's turn?",
        # and use that to use the value indexed as 0 or 1 in a literal array
        self.turn = ['X', 'O'][int(self.turn == 'X')]
    
    def printboard(self):
        """ Print the board state to the terminal, prints inverted"""
        for i in self.boardState:
            print(i)
        print('') # End with an empty line
screen = Screen()
game = TicTacToe()
game.drawBoard()
screen.mainloop()
