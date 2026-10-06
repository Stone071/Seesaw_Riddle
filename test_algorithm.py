# Figuring out how to operate the C command line game from Python Calls
import ctypes
import matplotlib

# GLOBALS
GameHost = None
DEBUG = False

# CONSTANTS
EMPTY_STRING = "            "
LEFT_SIDE_DOWN = 0
RIGHT_SIDE_DOWN = 1
BALANCED = 2
NO_MORE_ATTEMPTS = 3
T1_IND = 0
T2_IND = 1
T3_IND = 2

def InitializeLibrary():
    """ Initialize the shared library to a global and set up the function calls"""
    global GameHost
    # Assign the shared lib to GameHost
    GameHost = ctypes.CDLL("./bin/gamehost.so")
    # Define the necessary functions
    GameHost.BeginGame.argtypes = [ctypes.c_uint32]
    GameHost.BeginGame.restype = ctypes.c_uint32
    GameHost.TakeTurn.argtypes = [ctypes.c_uint32, ctypes.c_char_p]
    GameHost.TakeTurn.restype = ctypes.c_int
    GameHost.RevealPerson.argtypes = [ctypes.c_uint32]
    GameHost.RevealPerson.restype = ctypes.c_int
    GameHost.RevealWeight.argtypes = [ctypes.c_uint32]
    GameHost.RevealWeight.restype = ctypes.c_int
    GameHost.FreeSlot.argtypes = [ctypes.c_uint32]
    GameHost.FreeSlot.restype = ctypes.c_int

def PrintTurnResult(iRetVal: int):
    """ Check the return value of the turn and print the outcome """
    if (iRetVal == LEFT_SIDE_DOWN):
        print(f"Turn Taken: LEFT SIDE FALLS")
    elif (iRetVal == RIGHT_SIDE_DOWN):
        print(f"Turn Taken: RIGHT SIDE FALLS")
    elif (iRetVal == BALANCED):
        print(f"Turn Taken: SEESAW IS BALANCED")
    elif (iRetVal == NO_MORE_ATTEMPTS):
        print(f"NO MORE ATTEMPTS")
    else:
        print(f"Turn Taken: returned {iRetVal}")

class GameTracker:
    # No static/class variables
    def __init__(self):
        # Game Playing
        self.attemptStrings: list = [EMPTY_STRING, EMPTY_STRING, EMPTY_STRING]
        self.outcomes: list = [0, 0, 0]
        self.currentTurn: int = 0
        # End Game
        self.predictedIndividual: str = ""
        self.uniqueIndividual: str = ""
        self.uniqueWeight: int = 0

    def GetNextTurn(self) -> str:
        nextMove = GetNextMove(self.attemptStrings, self.outcomes)
        self.attemptStrings[self.currentTurn] = nextMove
        return nextMove

    def RecordOutcome(self, outcome: int):
        self.outcomes[self.currentTurn] = outcome
        self.currentTurn += 1

    def Predict(self) -> str:
        prediction = GetPrediction(self.attemptStrings, self.outcomes)
        self.predictedIndividual = prediction
        return prediction

    def RecordEndGame(self, theIndividual: str, theWeight:int):
        self.uniqueIndividual = theIndividual
        self.uniqueWeight = theWeight

    def PrintAll(self):
        print("###### GAME RESULTS ######")
        print(f"Turn 1: {self.attemptStrings[T1_IND]}")
        print(f"Out 1: {["LEFT SIDE DOWN", "RIGHT SIDE DOWN", "BALANCED", "OOA"][self.outcomes[T1_IND]]}")
        print(f"Turn 2: {self.attemptStrings[T2_IND]}")
        print(f"Out 2: {["LEFT SIDE DOWN", "RIGHT SIDE DOWN", "BALANCED", "OOA"][self.outcomes[T2_IND]]}")
        print(f"Turn 3: {self.attemptStrings[T3_IND]}")
        print(f"Out 3: {["LEFT SIDE DOWN", "RIGHT SIDE DOWN", "BALANCED", "OOA"][self.outcomes[T3_IND]]}")
        print(f"Predicted: {self.predictedIndividual}")
        print(f"Actual: {self.uniqueIndividual}, W: {self.uniqueWeight}")
        print("##########################")
    

def GetNextMove(attStrs: list, attOuts: list) -> str:
    """ Take previous layout and outcome as inputs and determine next move """
    retString = EMPTY_STRING
    # Check the state of this game. First move is always the same.
    if (attStrs[T1_IND] == EMPTY_STRING):
        retString = "ABCD  EFGH  "
    elif (attStrs[T2_IND] == EMPTY_STRING):
        retString = GetSecondMove(attStrs, attOuts)
    elif (attStrs[T3_IND] == EMPTY_STRING):
        retString = GetThirdMove(attStrs, attOuts)
    return retString

def GetSecondMove(attStrs: list, attOuts: list) -> str:
    """ Get the string for the appropriate second move """
    retStr = EMPTY_STRING
    prevOut = attOuts[T1_IND]
    if (prevOut == BALANCED):
        retStr = "IJ    KA    "
    elif (prevOut == LEFT_SIDE_DOWN or prevOut == RIGHT_SIDE_DOWN):
        retStr = "AFC   EBI   "
    return retStr

def GetThirdMove(attStrs: list, attOuts: list) -> str:
    """ Get the string for the appropriate third move """
    retStr = EMPTY_STRING
    turnTaken = attStrs[T2_IND]
    prevOut = attOuts[T2_IND]
    if (turnTaken == "IJ    KA    "):
        if (prevOut == BALANCED):
            retStr = EMPTY_STRING # we already have a solution
        elif (prevOut == LEFT_SIDE_DOWN or prevOut == RIGHT_SIDE_DOWN):
            retStr = "I     J     "
    elif (turnTaken == "AFC   EBI   "):
        if (prevOut == BALANCED):
            retStr = "G     H     "
        elif (prevOut == LEFT_SIDE_DOWN or prevOut == RIGHT_SIDE_DOWN):
            # check if the imbalance changed sides
            if (attOuts[T1_IND] == prevOut):
                retStr = "A     C     "
            else:
                retStr = "F     I     "
    return retStr

def GetPrediction(attStrs: list, attOuts: list) -> str:
    """ Predict the unique islander from the history captured in GamesTracked """
    retStr = EMPTY_STRING
    t1outcome = attOuts[T1_IND]
    t2attempt = attStrs[T2_IND]
    t2outcome = attOuts[T2_IND]
    t3attempt = attStrs[T3_IND]
    t3outcome = attOuts[T3_IND]
    # Check the prediction possible from turn 2
    if (t2attempt == "IJ    KA    ") and (t2outcome == BALANCED):
        retStr = "L"
    elif (t3attempt == "I     J     "):
        if (t3outcome == BALANCED):
            retStr = "K"
        else:
            if (t2outcome == t3outcome):
                retStr = "I"
            else:
                retStr = "J"
    elif (t3attempt == "G     H     "):
        if (t3outcome == BALANCED):
            retStr = "D"
        else:
            if (t1outcome == t3outcome):
                retStr = "H"
            else:
                retStr = "G"
    elif (t3attempt == "A     C     "):
        if (t3outcome == BALANCED):
            retStr = "E"
        else:
            if (t2outcome == t3outcome):
                retStr = "A"
            else:
                retStr = "C"
    elif (t3attempt == "F     I     "):
        if (t3outcome == BALANCED):
            retStr = "B"
        else:
            retStr = "F"

    return retStr


if __name__ == "__main__":
    GamesTracked: list = []

    InitializeLibrary()
    for thisToken in range(1, 5):
        print(f"\nRUNNING GAME {thisToken}\n")

        result = GameHost.BeginGame(thisToken)
        #print(f"BeginGame returned: {result}")

        # Init a tracker for this game
        thisGame = GameTracker()

        # TakeTurn would get the next attempt string, pass it to the host, and record the result.
        for turn in range(0, 3):
            # Must pass the string as bytes
            turnStr = thisGame.GetNextTurn().encode("ASCII")
            outcome = GameHost.TakeTurn(thisToken, turnStr)
            thisGame.RecordOutcome(outcome)
            #PrintTurnResult(outcome)

        # Predict and then get the outcomes
        prediction = thisGame.Predict()
        #print(f"WE PREDICT: {prediction}")

        theIndividual = chr(ord("A") + GameHost.RevealPerson(thisToken))
        #print(f"THE ISLANDER WAS: {theIndividual}")

        theWeight = GameHost.RevealWeight(thisToken)
        #print(f"THEIR WEIGHT WAS: {theWeight}")

        # Record result and save the game record
        thisGame.RecordEndGame(theIndividual, theWeight)
        thisGame.PrintAll()
        GamesTracked.append(thisGame)

        # Allow host to free game
        result = GameHost.FreeSlot(thisToken)
        print(f"FREEING GAME {thisToken}")