#ifndef SEESAW_H
#define SEESAW_H

#include <stdbool.h>

// SIMULATION RELATED
#define NUM_ISLANDERS     12
#define MAX_TURNS          3
#define DEFAULT_WEIGHT   125
#define WEIGHT_VARIANCE    5

// INPUT RELATED
#define INPUT_SIZE         3 // is 3 because it includes the newline

// SEESAW COMPUTE
#define MAX_L_POS          5
#define MAX_R_POS         11

// The struct which each islander is
typedef struct{
  unsigned char name;
  int weight;
  bool fOnSeesaw;
}sIslander;

// Must be extern so compiler doesn't fully define it here in the header.
extern sIslander asIslanders[NUM_ISLANDERS];

#endif
