// This source file is going to handle screen management
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "seesaw.h"

// Globals
unsigned char aucScreen[NUM_SCREEN_LINES][NUM_CHARS_LINE] = {0};
// The instructions for the player
const unsigned char aucInstructions[] = 
{"Your islanders have names which are letters of the alphabet.\n\
To put an islander on the seesaw, enter their name and the,\n\
position you would like them in, like 'A9'. Press R to reset\n\
the seesaw. Press SPACE when you want to let the seesaw drop.\n"};
// This is the empty base frame which is flashed to the screen
const char ucScreenBase[NUM_SCREEN_LINES][NUM_CHARS_LINE] = {
  {"1_2_3_4_5_6___7_8_9_0_!_@"},
  {"            ^            "},
  {"                         "},
  {"                         "},
};


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
