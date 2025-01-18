// This program should let the user play the 12 person seesaw game.
// The game is a riddle the user is trying to solve.
//
// There are 12 people on an island. There is a seesaw on the island.
// 11 of the people weigh the exact same amount. The other person weighs
// a slightly different weight. The user must determine who the person of
// unequal weight is using only the seesaw, and the seesaw can only be used
// 3 times.
//

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "seesaw.h"

// GLOBAlS
sIslander asIslanders[NUM_ISLANDERS] = {0};
sIslander* apsSeesaw[NUM_ISLANDERS] = {NULL};
char acInputBuf[INPUT_SIZE] = {0xFF}; //0xFF is an invalid ASCII char
unsigned int uiCommand[2] = {0xFF};
bool fGameLoop = true;
bool fScreenRefresh = false;

// HELPER FUNCTIONS
unsigned int get_random(unsigned int uiMaxNumber)
{
  unsigned int uiRand = rand() % uiMaxNumber;
  return uiRand;
}

void seed_rand(void)
{
  srand(time(NULL)); 
}

void populate_island(void)
{
  // PICK THE ONE  
  unsigned int uiTheOne = get_random(NUM_ISLANDERS);

  for (int i=0; i<NUM_ISLANDERS; i++)
  {
    unsigned int uiWeight = DEFAULT_WEIGHT;
    if (i == uiTheOne)
    {
      unsigned int uiNegative = get_random(2);
      // Get the variance, but don't allow it to be zero.
      unsigned int uiVariance = get_random(WEIGHT_VARIANCE) + 1;
      if (uiNegative == 0)
      {
        uiWeight -= uiVariance;
      }
      else if (uiNegative == 1)
      {
        uiWeight += uiVariance;
      }
    }
    sIslander temp = {0x41+i, uiWeight, false};
    asIslanders[i] = temp;
  }
}

sIslander* player_name_lookup(char cName)
{
  sIslander* psPlayer = NULL;
  // Find the player with the given name
  for (int i=0; i<NUM_ISLANDERS; i++)
  {
    if (asIslanders[i].name == cName)
    {
      psPlayer = &asIslanders[i];
      break;
    }
  }
  return psPlayer;
}

void place_player(void)
{
    // Translate the char position into the seesaw index
  char cName = acInputBuf[0];
  char cPos = acInputBuf[1];
  unsigned int uiSeesawIndex = symbol_pos_to_seesaw_index(cPos);
  sIslander* psPlayer = player_name_lookup(cName);

  if (uiSeesawIndex != 0xFF)
  {
    // Put the player on the seesaw!
    apsSeesaw[uiSeesawIndex] = psPlayer;
    psPlayer->fOnSeesaw = true;
    draw_player_on_seesaw(psPlayer, uiSeesawIndex);
  }
  else
  {
    //printf("Error determining seesaw position\n");
    reset_status_msg();
    char cNewStatus[] = "Error determining seesaw position\n";
    memcpy(aucOnScreenStatus, &cNewStatus, sizeof(cNewStatus));
    fScreenRefresh = true;
  }
}

void reset_seesaw(void)
{
  for (int i=0; i<NUM_ISLANDERS; i++)
  {
    apsSeesaw[i] = NULL;
    asIslanders[i].fOnSeesaw = false;
  }
}

// Function retrieves 3 chars from stdin, and executes commands if input is valid.
unsigned char get_input(void)
{
  sIslander* psPlayer;
  unsigned int uiSeesawIndex;

  memset(acInputBuf, 0, INPUT_SIZE);
  // must use fgets so we can get more than one character at a time
  char* cRet = fgets(acInputBuf, INPUT_SIZE, stdin);

  if (acInputBuf[0] == 'Q') // QUIT
  {
    fGameLoop = false;
  }
  else if (acInputBuf[0] == 'T') // TEST
  {
    // Have not written this yet.
    fScreenRefresh = true;
  }
  else if (acInputBuf[0] == 'R') // RESET
  {
    // reset all postions and update screen
    reset_seesaw();
    initialize_screen();
    fScreenRefresh = true;
  }
  else if (acInputBuf[0] == 'I')
  {
    reset_status_msg();
    set_status_instructions();
    fScreenRefresh = true;
  }
  else if (acInputBuf[0] >= 'A' && acInputBuf[0]<= 'L')
  {
    if (acInputBuf[1] == '1'
      || acInputBuf[1] == '2'
      || acInputBuf[1] == '3'
      || acInputBuf[1] == '4'
      || acInputBuf[1] == '5'
      || acInputBuf[1] == '6'
      || acInputBuf[1] == '7'
      || acInputBuf[1] == '8'
      || acInputBuf[1] == '9'
      || acInputBuf[1] == '0'
      || acInputBuf[1] == '!'
      || acInputBuf[1] == '@')
      {
        // Check the position is not already filled and player is not
        // already on seesaw
        uiSeesawIndex = symbol_pos_to_seesaw_index(acInputBuf[1]);
        psPlayer = player_name_lookup(acInputBuf[0]);
        if (apsSeesaw[uiSeesawIndex] == NULL && uiSeesawIndex != 0xFF
          && psPlayer != NULL && psPlayer->fOnSeesaw == false)
        {
          // MOVE THE PLAYER!
          place_player();
          fScreenRefresh = true;
        }
        else
        {
          //printf("Player is already on seesaw or position is already filled.\n");
          reset_status_msg();
          char cNewStatus[] = "Player is already on seesaw or position is already filled.\n";
          memcpy(aucOnScreenStatus, &cNewStatus, sizeof(cNewStatus));
          fScreenRefresh = true;
        }
      }
      else
      {
        //printf("Your command was not recognized.\n");
        reset_status_msg();
        char cNewStatus[] = "Your command was not recognized.\n";
        memcpy(aucOnScreenStatus, &cNewStatus, sizeof(cNewStatus));
        fScreenRefresh = true;
      }
  }
  else
  {
    //printf("Your command was not recognized.\n");
    reset_status_msg();
    char cNewStatus[] = "Your command was not recognized.\n";
    memcpy(aucOnScreenStatus, &cNewStatus, sizeof(cNewStatus));
    fScreenRefresh = true;
  }
}


bool main(void)
{
  // Seed rand, use epoch time.
  seed_rand();
  // Generate the islanders, with one unequal weight.
  populate_island();

  //for (int i=0; i<NUM_ISLANDERS; i++)
  //{
  //  printf("%c: %i\n", asIslanders[i].name, asIslanders[i].weight);
  //}

  initialize_screen();
  print_screen();

  // The actual game loop!
  while (fGameLoop)
  {
    // Everything is keyboard driven, get the input, trigger changes, 
    //then print screen.
    get_input();
    if (fScreenRefresh == true)
    {
      print_screen();
      fScreenRefresh = false;
    }
  }
  return true;
}