// ##############################################
// # screen.h
// #
// # This is the header for screen.c
// # 
// # Zachary Stone, December 2024
// ##############################################

#ifndef SCREEN_H
#define SCREEN_H

#include "seesaw.h"

// DEFINES
#define NUM_SCREEN_LINES   6
#define NUM_CHARS_LINE    26

void initialize_screen(void);
void print_screen(void);
void draw_player_on_seesaw(sIslander* psPlayer, unsigned int uiSeesawIndex);
unsigned int symbol_pos_to_seesaw_index(char cPos);
void reset_status_msg(void);
int set_status_msg(const char* pacOnScreenStatus, int size);
void set_status_instructions(void);

#endif
