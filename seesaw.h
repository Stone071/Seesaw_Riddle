// ##############################################
// # seesaw.h
// #
// # This header contains data relevant to operation
// # of the seesaw game in main.c
// # 
// # Zachary Stone, December 2024
// ##############################################

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

// DATA TYPES
// The struct which describes each islander
typedef struct{
  unsigned char name;
  int weight;
  bool fOnSeesaw;
}sIslander;

int get_islander_name(unsigned int uiIndex, char* pacDest, int maxSize);

#endif
