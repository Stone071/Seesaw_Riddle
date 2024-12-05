#ifndef SEESAW_H
#define SEESAW_H

// INCLUDES
#include <stdbool.h>


// SIMULATION RELATED
#define NUM_ISLANDERS     12
#define MAX_TURNS          3
#define DEFAULT_WEIGHT   125
#define WEIGHT_VARIANCE    5

// DISPLAY RELATED
#define NUM_SCREEN_LINES   4
#define NUM_CHARS_LINE    26

// INPUT RELATED
#define COMMAND_RESET      0
#define COMMAND_TEST       1
#define COMMAND_PLACE      2
#define COMMAND_INCOMPLETE 3
#define COMMAND_QUIT       4

// SEESAW COMPUTE
#define L_HALF             0
#define R_HALF             1
#define NONE_HALF          2

const char ucScreenBase[NUM_SCREEN_LINES][NUM_CHARS_LINE] = {
  {"1_2_3_4_5_6___7_8_9_0_!_@"},
  {"            ^            "},
  {"                         "},
  {"                         "},
};

const unsigned char aucInstructions[] = 
{"Your islanders have names which are letters of the alphabet.\n\
To put an islander on the seesaw, enter their name and the,\n\
position you would like them in, like 'A9'. Press R to reset\n\
the seesaw. Press SPACE when you want to let the seesaw drop.\n"};

typedef struct{
  unsigned char name;
  int weight;
  unsigned int uiSide;
  bool fOnSeesaw;
}sIslander;


#endif
