// This is the header for screen.c
#ifndef SCREEN_H
#define SCREEN_H

#include "seesaw.h"

// DEFINES
#define NUM_SCREEN_LINES   6
#define NUM_CHARS_LINE    26

// Must be extern to avoid being fully defined here and thus multiply defined.
extern unsigned char aucScreen[NUM_SCREEN_LINES][NUM_CHARS_LINE];
extern const unsigned char aucInstructions[];
extern unsigned char aucOnScreenStatus[];

void initialize_screen(void);
void print_screen(void);
void draw_player_on_seesaw(sIslander* psPlayer, unsigned int uiSeesawIndex);
unsigned int symbol_pos_to_seesaw_index(char cPos);
void reset_status_msg(void);
void set_status_instructions(void);

#endif
