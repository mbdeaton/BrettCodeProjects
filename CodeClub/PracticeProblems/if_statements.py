"""Grade translator."""

# Your task is to build a grade translator, translating from a number 0-100 to a
# letter (A: 90-100, B: 80-89, C: 70-79, D: 60-69, F: <60). Actually, you
# will build multiple versions of the translator, each using if statements in
# different ways. The minimal user interface might go something like this:
#  OUT   enter a number grade
#  IN    89
#  OUT   the letter grade is B

# This script fragment needs at least the following modifications to complete
# the task:
#   1) correct errors in the first function, translate_if(),
#   2) write if statements that fulfill the function descriptions for the
#      remaining two functions, translate_if_else(), and
#      translate_if_elif_else().

def translate_if(percentage):
    """Translate a number grade using only if statements."""
    if percentage > 90:
        return "A"
    if percentage < 90:
        if percentage > 80:
            return "B"
        if percentage < 80:
            if percentage > 70:
                return "C"
            if percentage < 70:
                if percentage > 60:
                    return "D"
                if percentage < 60:
                    return "F"

def translate_if_else(percentage):
    """Translate a number grade using if else statements."""
    # The function translate_if() checks to see what the value of percentage is
    # several times. If percentage is 81, then we don't need to check if it is
    # less than 80, because we already checked to see that is was greater than
    # 80. Else statements always execute when the if statement does not. Or in
    # other words, an else statement executes when the if statement evaluates as
    # false. Write this function using else statements.
    return "OMG!" # delete this

def translate_if_elif_else(percentage):
    """Translate a number grade using if elif else statements."""
    # Nested if loops are often useful when writing a program, but can become
    # hard to read the more nested it becomes. When possible, it is preferable
    # to keep all functional logic at the highest possible level to maintain
    # readability and keep code organized. elif statements are used to evaluate
    # multiple conditions. Once an elif statement evaluates to true, the
    # following code block is executed and the if stucture is finished.
    # Write this function using elif statmeents, so there are no nested if
    # statements. (This is called refactoring the above code.)
    return "OMG!" # delete this

################################################################################
# Here's the main script.

percentage = int(input("Enter a number grade: "))
print("1st translator grade: ", translate_if(percentage))
print("2nd translator grade: ", translate_if_else(percentage))
print("3rd translator grade: ", translate_if_elif_else(percentage))
