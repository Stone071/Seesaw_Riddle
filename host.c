// ##############################################
// # host.c
// #
// # The host instance to play out the seesaw 
// # island riddle from Brooklyn 99
// #
// # Zachary Stone, September 2026
// ##############################################

// INCLUDES
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>

// TYPES, DEFINES, GLOBALS
#define MAX_GAMES             (1000)
#define STORAGE_FILE          ("/home/zachary/Desktop/software_projects/Seesaw_Riddle/PersistentGames")
#define PERSISTENT_GAMES_SIZE (sizeof(game_t) * MAX_GAMES)
#define NUM_ISLANDERS          (12)
#define MAX_TURNS               (3)
#define DEFAULT_WEIGHT        (125)
#define WEIGHT_VARIANCE         (5)
#define LEFT_SIDE_FALLS         (0) // Returns for TakeTurn
#define RIGHT_SIDE_FALLS        (1) // Returns for TakeTurn
#define SEESAW_IS_BALANCED      (2) // Returns for TakeTurn
#define NO_MORE_ATTEMPTS        (3) // Returns for TakeTurn
#define SEESAW_LEFT_END         (5)
#define SEESAW_RIGHT_BEGIN      (6)

typedef struct {
  int iToken; // the token belonging to this game
  unsigned char ucAttemptsMade; // number of seesaw tests performed in this game
  unsigned char ucStdWeight; // the standard weight of islanders in this game
  unsigned char ucDiffWeight; // the weight of the different islander in this game
  char cDiffIslander; // the ID of the different islander in this game
}game_t;

game_t* psPersistentGames = NULL;

// FUNCTION PROTOTYPES
int InitializeHost(void);
int CloseHost(void);
int BeginGame(int iNewToken);
static int InitializeGame(game_t* pGame, int iToken);
static unsigned int GetRandom(unsigned int uiMaxNumber);
static void SeedRand(void);
static void PopulateGame(game_t* pGame);

// Open the persistent games file and map it to psPersistentGames
int InitializeHost(void)
{
  int iRetVal = -1;

  // Open the file and map the games. The file can then be closed.
  // Must set the file mode if we allow the file to be created.
  int fd = open(STORAGE_FILE, (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR));
  
  // Check for error
  if (fd < 0)
  {
    // Do something
    close(fd);
    error(fd, fd, "%s", "ERROR IN OPEN");
  }
  
  // Ensure the file has the expected size
  ftruncate(fd, PERSISTENT_GAMES_SIZE);  

  // Mapping the games. NULL means let the kernel choose the actual address used.
  psPersistentGames = mmap(NULL, PERSISTENT_GAMES_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  // On success, mmap() returns a pointer to the mapped area.  On error, the value MAP_FAILED
  // (that is, (void *) -1) is returned, and errno is set to indicate the error.
  if (psPersistentGames == MAP_FAILED)
  {
    // Do something
    printf("MAP FAILED\n");
  }
  else
  {
    iRetVal = 0;
  }

  // Close the file descriptor
  close(fd);

  // Return success or fail
  return iRetVal;
}

// Unmap the persistent games
int CloseHost(void)
{
  // Unmap the games
  return (munmap(psPersistentGames, PERSISTENT_GAMES_SIZE));
  // On success, munmap() returns 0.  On failure, it returns -1, 
  // and errno is  set  to  indicate the error (probably to EINVAL).
}

// Look through the games. If you see the given token, return that you found a game
// and set slot offset to that game. If you don't find the game, return the offset of
// the first open slot
bool GameLookup(int iGivenToken, unsigned short* pusGameOffset, unsigned short* pusEmptyOffset)
{
  bool fGameFound = false;
  bool fEmptySlotFound = false;
  game_t* pGame = NULL;

  // Check the persistent games
  #ifdef DEBUG
  printf("Looking up game: %d\n", iGivenToken);
  #endif
  for (unsigned int i = 0; i < MAX_GAMES; i++)
  {
    pGame = psPersistentGames + i;
    if (pGame->iToken == iGivenToken)
    {
      // We found an existing game with this token
      fGameFound = true;
      *pusGameOffset = i;
      #ifdef DEBUG
      printf("Game found at slot: %d\n", i);
      #endif
    }
    // This is an empty spot
    else if (pGame->iToken == 0 && fEmptySlotFound == false)
    {
      fEmptySlotFound = true;
      *pusEmptyOffset = i;
    }
    // If we've found the game and an empty slot already, we can conclude our search
    else if (fGameFound == true && fEmptySlotFound == true)
    {
      break;
    }
  }

  #ifdef DEBUG
  if (fGameFound == false)
  {
    printf("Game not found. Empty slot %d logged\n", *pusEmptyOffset);
  }
  #endif

  // Return whether you found the game or not
  return fGameFound;
}

// Take the token, determine if we already have it, begin a game under it
int BeginGame(int iNewToken)
{
  game_t* pGame;
  bool fGameFound = false;
  unsigned short usGameOffset = 0;
  unsigned short usEmptyOffset = 0;

#ifdef DEBUG
  printf("BEGIN GAME ENTERED\n");
#endif

  // Check that host has mapped the persistent games
  if (psPersistentGames == NULL)
  {
    #ifdef DEBUG
    printf("Initializing host files...\n");
    printf("Seeding random numbers...\n");
    #endif
    InitializeHost();
    SeedRand();
  }
  else if (psPersistentGames == MAP_FAILED)
  {
    printf("MAP FAILED\n");
    return -1;
  }

  // Check if a game exists under this token.
  fGameFound = GameLookup(iNewToken, &usGameOffset, &usEmptyOffset);

  // If no game already using this token, create a game
  if (fGameFound == false)
  {
    // First check if the open slot offset is really open
    pGame = psPersistentGames + usEmptyOffset;
    if (pGame->iToken == 0)
    {
      InitializeGame(pGame, iNewToken);
    } 
  }

  return 0;
}

// Allow users to submit a turn as a string
// "ABC   DEF   " is A,B,C on left side, D,E,F on right
int TakeTurn(int iToken, char* acSeesawLayout)
{
  unsigned short usGameSlot = 0;
  unsigned short usEmptySlot = 0;
  int iRetVal = -1;
  bool fOnSeesaw = false;
  unsigned char ucSeesawIndex = 0;
  game_t* pGame = NULL;
  bool fGameFound = GameLookup(iToken, &usGameSlot, &usEmptySlot);

  if (fGameFound == true)
  {
    pGame = psPersistentGames + usGameSlot;

    // Check if another turn can be taken
    if (pGame->ucAttemptsMade < 3)
    {
      // check if special islander is on the seesaw
      for (ucSeesawIndex = 0; ucSeesawIndex < NUM_ISLANDERS; ucSeesawIndex++)
      {
        if (acSeesawLayout[ucSeesawIndex] == pGame->cDiffIslander)
        {
          fOnSeesaw = true;
          break;
        }
      }

      if (fOnSeesaw == false)
      {
        // the seesaw is balanced
        iRetVal = SEESAW_IS_BALANCED;
      }
      else if (ucSeesawIndex <= SEESAW_LEFT_END)
      {
        if (pGame->ucDiffWeight < pGame->ucStdWeight)
        {
          // left end rises
          iRetVal = RIGHT_SIDE_FALLS;
        }
        else
        {
          // left end falls
          iRetVal = LEFT_SIDE_FALLS;
        }
      }
      else if (ucSeesawIndex >= SEESAW_RIGHT_BEGIN)
      {
        if (pGame->ucDiffWeight < pGame->ucStdWeight)
        {
          // right end rises
          iRetVal = LEFT_SIDE_FALLS;
        }
        else
        {
          // right end falls
          iRetVal = RIGHT_SIDE_FALLS;
        }
      }
      else
      {
        // should not be here...
        iRetVal = -1;
      }

      // Log that a turn was taken
      pGame->ucAttemptsMade += 1;
    }
    else
    {
      iRetVal = NO_MORE_ATTEMPTS;
    }
  }
  else
  {
    iRetVal = -1;
  }

  return iRetVal;
}

// Reveal who the differently weighted individual is
// returns
// 0: A
// 1: B
// etc...
int RevealPerson(int iToken)
{
  int iRetVal = -1;
  unsigned short usGameOffset = 0;
  unsigned short usEmptySlot = 0;
  game_t* pGame = NULL;
  bool fGameFound = GameLookup(iToken, &usGameOffset, &usEmptySlot);

  if (fGameFound == true)
  {
    pGame = psPersistentGames + usGameOffset;
    iRetVal = (int)(pGame->cDiffIslander - 'A');
  }
  else
  {
    iRetVal = -1;
  }

  return iRetVal;
}

// Reveal the different weight
int RevealWeight(int iToken)
{
  int iRetVal = -1;
  unsigned short usGameOffset = 0;
  unsigned short usEmptySlot = 0;
  game_t* pGame = NULL;
  bool fGameFound = GameLookup(iToken, &usGameOffset, &usEmptySlot);

  if (fGameFound == true)
  {
    pGame = psPersistentGames + usGameOffset;
    iRetVal = (int)(pGame->ucDiffWeight);
  }
  else
  {
    iRetVal = -1;
  }

  return iRetVal;
}

// Clean up a game slot after the game has ended
int FreeSlot(int iToken)
{
  int iRetVal = -1;
  unsigned short usGameOffset = 0;
  unsigned short usEmptySlot = 0;
  game_t* pGame = NULL;
  game_t sEmptyGame = {0};
  bool fGameFound = GameLookup(iToken, &usGameOffset, &usEmptySlot);

  if (fGameFound == true)
  {
    pGame = psPersistentGames + usGameOffset;
    *pGame = sEmptyGame;
    iRetVal = 0;
  }
  else
  {
    iRetVal = -1;
  }

  return iRetVal;
}

// Initialize the values for a new game at the address pGame
static int InitializeGame(game_t* pGame, int iToken)
{
  #ifdef DEBUG
  printf("Initializing game with token %d\n", iToken);
  #endif
  pGame->iToken = iToken;
  PopulateGame(pGame);
  return 0;
}

// GetRandom returns a random uint in range [0,uiMaxNumber)
static unsigned int GetRandom(unsigned int uiMaxNumber)
{
  unsigned int uiRand = rand() % uiMaxNumber;
  return uiRand;
}

// seed_rand seeds than random number generator with epoch time
static void SeedRand(void)
{
  srand(time(NULL));
}

// Initialize values for this game
static void PopulateGame(game_t* pGame)
{
  // PICK THE ONE
  pGame->cDiffIslander = (char)('A' + GetRandom(NUM_ISLANDERS));

  unsigned int uiNegative = GetRandom(2);
  // Get the variance, but don't allow it to be zero.
  unsigned int uiVariance = GetRandom(WEIGHT_VARIANCE) + 1;
  if (uiNegative == 0)
  {
    pGame->ucDiffWeight = DEFAULT_WEIGHT + uiVariance;
  }
  else if (uiNegative == 1)
  {
    pGame->ucDiffWeight = DEFAULT_WEIGHT - uiVariance;
  }
  pGame->ucAttemptsMade = 0;
  pGame->ucStdWeight = DEFAULT_WEIGHT;
}

#ifndef SHARED_LIB
int main(void)
{
#ifdef DEBUG
  printf("MAIN ENTERED\n");
#endif

  int iMyToken = 1;
  BeginGame(iMyToken);
  TakeTurn(iMyToken, "ABC   DEF   ");
  CloseHost();

#ifdef DEBUG
  printf("MAIN EXIT\n");
#endif
  return 1;
}
#endif
