// ##############################################
// # screen.c
// #
// # This file is going to handle screen management
// # for the seesaw_riddle game
// # 
// # Zachary Stone, December 2024
// ##############################################

// INCLUDES
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "seesaw.h"

// GLOBALS
unsigned char aucScreen[NUM_SCREEN_LINES][NUM_CHARS_LINE] = {0};
// The instructions for the player
const unsigned char aucInstructions[] = 
{"Your islanders have names which are letters of the alphabet.\n\
To put an islander on the seesaw, enter their name and the\n\
position you would like them in, like 'A9'. Press R to reset\n\
the seesaw. Enter 'T' when you want to let the seesaw drop.\n\
Enter 'Q' when you want to quit. Enter 'W' to find out who\n\
the person of unequal weight is. Enter 'M' for instructions msg.\n"};
// The buffer which will hold status messages
unsigned char aucOnScreenStatus[sizeof(aucInstructions)];
// This is the empty base frame which is flashed to the screen
const char ucScreenBase[NUM_SCREEN_LINES][NUM_CHARS_LINE] = {
  {"1_2_3_4_5_6___7_8_9_0_!_@"},
  {"            ^            "},
  {"                         "},
  {"                         "},
  {"                         "},
  {"YOUR COMMAND: "}
};


void initialize_screen(void)
{
  unsigned int j = 0;
  char cTemp;
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
        get_islander_name(j, &cTemp, sizeof(cTemp));
        memcpy((aucScreen[3] + (j*2)), &cTemp, sizeof(cTemp));
        j++;
      } 
    }
  }
}

int set_status_msg(const char* pacStatString, int size)
{
  if (size < sizeof(aucOnScreenStatus))
  {
    memcpy(aucOnScreenStatus,  pacStatString, size);
    return 0;
  }
  else
  {
    return 1;
  }
}

void print_screen(void)
{
#ifndef DEBUG_MODE
  system("clear");
#endif
  printf("%s\n",aucOnScreenStatus);
  for (int i=0; i<NUM_SCREEN_LINES; i++)
  {
    if (i == NUM_SCREEN_LINES-1)
    {
      printf("%s",aucScreen[i]);
    }
    else
    {
      printf("%s\n",aucScreen[i]);
    }
  }
}

void draw_player_on_seesaw(sIslander* psPlayer, unsigned int uiSeesawIndex)
{
  // Seesaw index needs to be translated into the proper screen position...
  unsigned char ucPlayerName = psPlayer->name;
  unsigned int uiScreenPos;
  if (uiSeesawIndex < 6) // LEFT HALF
  {
    uiScreenPos = uiSeesawIndex * 2;
  }
  else // RIGHT HALF
  {
    uiScreenPos = uiSeesawIndex * 2 + 2;
  }
  memcpy(aucScreen[0] + uiScreenPos, &ucPlayerName, sizeof(unsigned char));

  // Also remove their name from the bottom line
  for (int i=0; i<NUM_CHARS_LINE; i++)
  {
    if (*(aucScreen[3]+i) == ucPlayerName)
    memset(aucScreen[3] + i, ' ', sizeof(unsigned char));
  }
}

unsigned int symbol_pos_to_seesaw_index(char cPos)
{
  unsigned int uiSeesawIndex = 0xFF;
  if (cPos >= '1' && cPos <= '9')
  {
    uiSeesawIndex = (unsigned int)cPos - 0x31;
  }
  else if (cPos == '0')
  {
    uiSeesawIndex = 9;
  }
  else if (cPos == '!')
  {
    uiSeesawIndex = 10;
  }
  else if (cPos == '@')
  {
    uiSeesawIndex = 11;
  }
  return uiSeesawIndex;
}

void reset_status_msg(void)
{
  memset(aucOnScreenStatus, 0x3, sizeof(aucOnScreenStatus));
}

void set_status_instructions(void)
{
  memcpy(aucOnScreenStatus, aucInstructions, sizeof(aucInstructions));
}