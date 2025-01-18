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
unsigned int uiAttempts = 3;

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

void get_input(void)
{
  // Clear acInputBuf
  memset(acInputBuf, 0, INPUT_SIZE);
  // Grab everything in stdin and discard everything but the first two chars.
  int iter = 0;
  int c;
  while ( (c = getchar()) != '\n' && c != '\r' && c != EOF)
  {
    if (iter < INPUT_SIZE)
    {
      acInputBuf[iter] = c;
    }
    iter++;
  }
}

// Function retrieves 3 chars from stdin, and executes commands if input is valid.
unsigned char get_command(void)
{
  sIslander* psPlayer;
  unsigned int uiSeesawIndex;

  get_input();

  if (acInputBuf[0] == 'Q') // QUIT
  {
    fGameLoop = false;
  }
  else if (acInputBuf[0] == 'T') // TEST
  {
    if (uiAttempts == 0)
    {
      // Set a status message
      reset_status_msg();
      char acNewStatus[] = "You have exhausted your three attempts.\n\
      Do you know who it is?\n";
      memcpy(aucOnScreenStatus, acNewStatus, sizeof(acNewStatus));
      fScreenRefresh = true;
    }
    else
    {
      // At the moment, we are not using their distance from the fulcrum at all
      // we are computing as if they are all sitting equidistant from the fulcrum.
      unsigned int uiLeftSum = 0;
      unsigned int uiRightSum = 0;
      for (int i=0; i<=MAX_L_POS; i++)
      {
        // Make sure we are at a filled position
        if (apsSeesaw[i] != NULL)
        {
          uiLeftSum += apsSeesaw[i]->weight;
        }
      }
      for (int i=MAX_L_POS+1; i<= MAX_R_POS; i++)
      {
        if (apsSeesaw[i] != NULL)
        {
          uiRightSum += apsSeesaw[i]->weight;
        }
      }

      // Set a status message
      reset_status_msg();
      if (uiLeftSum == uiRightSum)
      {
        char acNewStatus[] = "The seesaw is perfectly still...\n";
        memcpy(aucOnScreenStatus, acNewStatus, sizeof(acNewStatus));
      }
      else if (uiLeftSum > uiRightSum)
      {
        char acNewStatus[] = "The LEFT half of the seesaw drops toward the ground!\n";
        memcpy(aucOnScreenStatus, acNewStatus, sizeof(acNewStatus));
      }
      else if (uiRightSum > uiLeftSum)
      {
        char acNewStatus[] = "The RIGHT half of the seesaw drops toward the ground!\n";
        memcpy(aucOnScreenStatus, acNewStatus, sizeof(acNewStatus));
      }
      else
      {
        char acNewStatus[] = "THIS SHOULD NOT HAPPEN\n";
        memcpy(aucOnScreenStatus, acNewStatus, sizeof(acNewStatus));
      }
      uiAttempts -= 1;
      fScreenRefresh = true;
    }
  }
  else if (acInputBuf[0] == 'R') // RESET
  {
    // reset all postions and update screen
    reset_seesaw();
    initialize_screen();
    reset_status_msg();
    char acNewStatus[] = "Islander positions reset...\n";
    memcpy(aucOnScreenStatus, acNewStatus, sizeof(acNewStatus));
    fScreenRefresh = true;
  }
  else if (acInputBuf[0] == 'I') // INSTRUCTIONS
  {
    reset_status_msg();
    set_status_instructions();
    fScreenRefresh = true;
  }
  else if (acInputBuf[0] == 'W') // WHO IS IT
  {
    reset_status_msg();
    for (int i=0; i<NUM_ISLANDERS; i++)
    {
      if (asIslanders[i].weight != DEFAULT_WEIGHT)
      {
        char acMessage[20];
        char cName = asIslanders[i].name;
        unsigned int uiWeight = asIslanders[i].weight;
        memcpy(acMessage, &cName, sizeof(cName));
        memcpy(acMessage + 2*sizeof(cName), &uiWeight, sizeof(uiWeight));
        memcpy(aucOnScreenStatus, acMessage, sizeof(acMessage));
        break;
      }
    }
    fScreenRefresh = true;
  }
  else if (acInputBuf[0] >= 'A' && acInputBuf[0]<= 'L') // PLACE AN ISLANDER
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
          reset_status_msg();
          char cNewStatus[] = "Islander placed...\n";
          memcpy(aucOnScreenStatus, &cNewStatus, sizeof(cNewStatus));
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
  set_status_instructions();
  print_screen();

  // The actual game loop!
  while (fGameLoop)
  {
    // Everything is keyboard driven, get the input, trigger changes, 
    //then print screen.
    get_command();
    if (fScreenRefresh == true)
    {
      print_screen();
      fScreenRefresh = false;
    }
  }
  return true;
}