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
#include "seesaw.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// GLOBAlS
sIslander asIslanders[NUM_ISLANDERS];
unsigned char aucScreen[NUM_SCREEN_LINES][NUM_CHARS_LINE] = {0};
unsigned int uiInputBuf[2] = {0xFF}; //0xFF is an invalid ASCII char
unsigned int uiCommand[2] = {0xFF};

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
    sIslander temp = {0x41+i, uiWeight, NONE_HALF, false};
    asIslanders[i] = temp;
  }
}

void initialize_screen(void)
{
  unsigned int j = 0;
  // Copy each line of screen base into screen buffer
  for (int i=0; i<NUM_SCREEN_LINES; i++)
  {
    strcpy(aucScreen[i], ucScreenBase[i]);
  }
  for (int i=0; i<NUM_CHARS_LINE; i++)
  {
    if (i % 2 == 0)
    {
      // Copy the names of the islanders into the bottom line
      if (j < NUM_ISLANDERS)
      {
        memcpy((aucScreen[3] + (j*2)), &(asIslanders[j].name), sizeof(unsigned char));
        j++;
      } 
    }
  }
}

void print_screen(void)
{
  system("clear");
  printf("%s\n",aucInstructions);
  for (int i=0; i<NUM_SCREEN_LINES; i++)
  {
    printf("%s\n",aucScreen[i]);
  }
  printf("\nYOUR COMMAND: ");
}

// Input handling will be split into placing chars into buffer
// and then checking the buffer to see if a complete command is
// present 
unsigned char get_input(void)
{
  int ucInput = getchar();
  // Special commands will occupy position 0 in the buffer.
  if (ucInput == 0x20)
  {
    uiInputBuf[0] = ucInput;
  }
  // grab seesaw position and call this function again.
  else if ((ucInput >= 0x30 && ucInput <= 0x39)
          || ucInput == 0x21 || ucInput == 0x40)
  {
    uiInputBuf[1] = ucInput;
  }
  // grab islander and call this function again.
  else if (ucInput >= 0x41 && ucInput <= 0x4C)
  {
    uiInputBuf[0] = ucInput;
  }
  // reset positions
  else if (ucInput == 0x52)
  {
    uiInputBuf[0] = ucInput;
  }
  else if (ucInput == 0x51)
  {
    uiInputBuf[0] = ucInput;
  }
}

unsigned int check_input_buf(void)
{
  unsigned int uiReturn = COMMAND_INCOMPLETE;
  unsigned int uiBuf0 = uiInputBuf[0];
  unsigned int uiBuf1 = uiInputBuf[1];
  
  // Special commands will occupy position 0 in the buffer.
  if (uiBuf0 == 0x20)
  {
    uiReturn = COMMAND_TEST;
  }
  // grab seesaw position and call this function again.
  else if (((uiBuf1 >= 0x30 && uiBuf1 <= 0x39)
          || uiBuf1 == 0x21 || uiBuf1 == 0x40)
          && (uiBuf0 >= 0x41 && uiBuf0 <= 0x4C))
  {
    uiReturn = COMMAND_PLACE;
    // Move the placement args into command buf
    uiCommand[0] = uiBuf0;
    uiCommand[1] = uiBuf1;
  }
  // reset positions
  else if (uiBuf0 == 0x52)
  {
    uiReturn = COMMAND_RESET;
  }
  else if (uiBuf0 == 0x51)
  {
    uiReturn = COMMAND_QUIT;
  }

  // if we have accepted a command, clear the input buffer.
  if (uiReturn != COMMAND_INCOMPLETE)
  {
    uiInputBuf[0] = 0xFF;
    uiInputBuf[1] = 0xFF;
  }
  return uiReturn;
}

// Definitely need a function to look through seesaw and detect who is there
void id_players_on_seesaw(void)
{
  // just look through the screen and see who is there.
  for (int i=0; i<NUM_CHARS_LINE; i++)
  {
    // For the exact middle position, don't check.
    if (i == 12) continue;
    for (int j=0; j<NUM_ISLANDERS; j++)
    {
      if (aucScreen[0][i] == asIslanders[j].name)
      {
        asIslanders[j].fOnSeesaw = true;
        asIslanders[j].uiSide = (i<12) ? L_HALF : R_HALF;
      } 
    }
  }
}

void reset_player_positions(void)
{
  for (int i=0; i<NUM_ISLANDERS; i++)
  {
    asIslanders[i].uiSide = NONE_HALF;
    asIslanders[i].fOnSeesaw = false;
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
  while (true)
  {
    unsigned int uiCommandGiven;
    // Everything is keyboard driven
    get_input();
    uiCommandGiven = check_input_buf();
    if (uiCommandGiven != COMMAND_INCOMPLETE)
    {
      if (uiCommandGiven == COMMAND_QUIT)
      {
        break;
      }
      else if (uiCommandGiven == COMMAND_RESET)
      {
        reset_player_positions();
        initialize_screen();
        print_screen();
      }
      else if (uiCommandGiven == COMMAND_TEST)
      {
        // Not written yet
      }
      else if (uiCommandGiven == COMMAND_PLACE)
      {
        unsigned char ucSelName = (unsigned char)uiCommand[0];
        char cPosition = (char)uiCommand[1];
        // Look through the screen and move the symbol.
        // Go get the index of the character and the place on the seesaw.
        // then memmove one to the other.
	for (int i=0; i<NUM_CHARS_LINE; i++)
	{
	  
	}
      }
    }
  }
  return true;
}
