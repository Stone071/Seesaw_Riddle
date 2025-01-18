#ifndef SEESAW_H
#define SEESAW_H

// INCLUDES
#include <stdbool.h>

// SIMULATION RELATED
#define NUM_ISLANDERS     12
#define MAX_TURNS          3
#define DEFAULT_WEIGHT   125
#define WEIGHT_VARIANCE    5

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

// The struct which each islander is
typedef struct{
  unsigned char name;
  int weight;
  unsigned int uiSide;
  bool fOnSeesaw;
}sIslander;

// Must be extern so compiler doesn't fully define it here in the header.
extern sIslander asIslanders[NUM_ISLANDERS];

#endif
