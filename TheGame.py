#@author: Kaitlyn Deppmeier
#@about: This is a game in which the program generates a random number, and the user has
#ten guesses to get it right

import random

#Prints intro message
print("Welcome to the game.\n")
print("I am thinking of an integer between 1 and 99.\n")
print("You have 10 guesses.\n")

theInt = random.randint(1, 99) #Generates random number in range of 1 to 99
guessNum = 1

#print("  DebugHint.  The number is ", theInt)

#Different hint functions

#Shows whether the number is divisible by a random number
def hint0():
    randomDivisor = random.randint(2, 12)
    if theInt % randomDivisor == 0  :  
        print("  Hint: The number is a multiple of", randomDivisor)
    else:
        print("  Hint: The number is NOT divisible by", randomDivisor)
    return

#Shows whether the number is greater than or less than a random number
def hint1():
    randomNum = random.randint(10, 90)
    if theInt > randomNum  :  
        print("  Hint: The number is greater than", randomNum)
    else:
        print("  Hint: The number is less than or equal to", randomNum)
    return

#Shows whether the number's one's digit is a random digit
def hint2():
    randomNum = random.randint(0, 9)
    if theInt % 10 == randomNum  :  
        print("  Hint: The number's one's digit is", randomNum)
    else:
        print("  Hint: The number's one's digit is NOT", randomNum)
    return

#Shows whether the number's ten's digit is a random digit
def hint3():
    randomNum = random.randint(0, 9)
    if int(theInt / 10) == randomNum :  
        print("  Hint: The number's ten's digit is", randomNum)
    else:
        print("  Hint: The number's ten's digit is NOT", randomNum)
    return

#Shows whether the number is within a random range
def hint4():
    randomNum = random.randint(0, 79)
    if randomNum <= theInt <= randomNum + 20  :  
        print("  Hint: The number is in the range of ", randomNum, "-", randomNum + 20, "(inclusive)")
    else:
        print("  Hint: The number is NOT in the range of ", randomNum, "-", randomNum + 20, "(inclusive)")
    return

#Shows whether the one's and ten's digits are the same
def hint5():
    onesDigit = theInt % 10
    tensDigit = int(theInt / 10)
    if onesDigit == tensDigit  :  
        print("  Hint: The one's and ten's digits are the same")
    else:
        print("  Hint: The one's and ten's digits are NOT the same")
    return

#Shows wheter the one's digit is greater than a random digit
def hint6():
    randomNum = random.randint(1, 9)
    if theInt % 10 > randomNum  :  
        print("  Hint: The one's digit is greater than", randomNum)
    else:
        print("  Hint: The one's digit is less than or equal to", randomNum)
    return

#Shows wheter the ten's digit is greater than a random digit
def hint7():
    randomNum = random.randint(1, 9)
    if int(theInt / 10) > randomNum  :  
        print("  Hint: The ten's digit is greater than", randomNum)
    else:
        print("  Hint: The ten's digit is less than or equal to", randomNum)
    return

hints = {
    0: hint0,
    1: hint1,
    2: hint2,
    3: hint3,
    4: hint4,
    5: hint5,
    6: hint6,
    7: hint7,
    }

#A function that provides a hint to the player
def dropHint():
    hintType = random.randint(0, 7)
    hints[hintType]()
    return

#Gives the player 10 attempts to guess the correct number
while True:
    print("Guess ", guessNum, ": ", end = "")#Prints the current guess number
    guess = input("")   #Accepts an integer from the user
    
    try:
        guess = int(guess)
        
    except ValueError:          #If the user enters something that isn't an int
        print("  That's not an integer.")
        continue
    
    if guess < 1 or guess > 99:#If the user enters an int outside of the range
        print("  The integer is between 1 and 100")
        continue
    
    elif guess == theInt:       #Checks if the user guessed correctly
        print("  Congrats, you got it!")
        break                   #Ends the game
    
    else:        
        guessNum += 1              #incriments the guessCount
        if guessNum > 10:    #Breaks when the guessCount passes 10
            print("  The number was", theInt, ".  Better luck next time!")
            break               #Ends the game
        if(abs(guess - theInt) > 50):   #Gives the user some feedback as to how close they are
            print("  That was way off")
        elif(abs(guess - theInt) < 10):
            print("  That's getting close")
        dropHint()              #Provides a hint

