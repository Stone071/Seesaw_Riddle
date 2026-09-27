# Figuring out how to operate the C command line game from Python Calls

import ctypes

# Assign the shared lib to gamehost
gamehost = ctypes.CDLL("./bin/gamehost.so")

# Define the necessary functions
gamehost.BeginGame.argtypes = [ctypes.c_uint32]
gamehost.BeginGame.restype = ctypes.c_uint32
gamehost.TakeTurn.argtypes = [ctypes.c_uint32, ctypes.c_wchar_p]
gamehost.TakeTurn.restype = ctypes.c_int

myToken = 1
result = gamehost.BeginGame(myToken)
print(f"BeginGame return: {result}")

result = gamehost.TakeTurn(myToken, "ABC   DEF   ")
if (result == 0):
  print(f"Turn Taken: LEFT SIDE FALLS")
elif (result == 1):
  print(f"Turn Taken: RIGHT SIDE FALLS")
elif (result == 2):
  print(f"Turn Taken: SEESAW IS BALANCED")
elif (result == 3):
  print(f"NO MORE ATTEMPTS")
else:
  print(f"Turn Taken: returned {result}")