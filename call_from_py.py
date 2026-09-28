# Figuring out how to operate the C command line game from Python Calls

import ctypes

def PrintTurnResult(iRetVal):
  if (iRetVal == 0):
    print(f"Turn Taken: LEFT SIDE FALLS")
  elif (iRetVal == 1):
    print(f"Turn Taken: RIGHT SIDE FALLS")
  elif (iRetVal == 2):
    print(f"Turn Taken: SEESAW IS BALANCED")
  elif (iRetVal == 3):
    print(f"NO MORE ATTEMPTS")
  else:
    print(f"Turn Taken: returned {iRetVal}")

# Assign the shared lib to gamehost
gamehost = ctypes.CDLL("./bin/gamehost.so")

# Define the necessary functions
gamehost.BeginGame.argtypes = [ctypes.c_uint32]
gamehost.BeginGame.restype = ctypes.c_uint32
gamehost.TakeTurn.argtypes = [ctypes.c_uint32, ctypes.c_wchar_p]
gamehost.TakeTurn.restype = ctypes.c_int
gamehost.RevealPerson.argtypes = [ctypes.c_uint32]
gamehost.RevealPerson.restype = ctypes.c_int
gamehost.RevealWeight.argtypes = [ctypes.c_uint32]
gamehost.RevealWeight.restype = ctypes.c_int

for thisToken in range(1, 5):
  print(f"\nRUNNING GAME {thisToken}\n")

  result = gamehost.BeginGame(thisToken)
  #print(f"BeginGame returned: {result}")

  result = gamehost.TakeTurn(thisToken, "ABC   DEF   ")
  PrintTurnResult(result)
  result = gamehost.TakeTurn(thisToken, "ABC   DEF   ")
  PrintTurnResult(result)
  result = gamehost.TakeTurn(thisToken, "ABC   DEF   ")
  PrintTurnResult(result)
  result = gamehost.TakeTurn(thisToken, "ABC   DEF   ")
  PrintTurnResult(result)

  result = gamehost.RevealPerson(thisToken)
  print(f"THE ISLANDER WAS: {result}")
  result = gamehost.RevealWeight(thisToken)
  print(f"THEIR WEIGHT WAS: {result}")