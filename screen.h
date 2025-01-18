// This is the header for screen.c
#ifndef SCREEN_H
#define SCREEN_H

// DEFINES
#define NUM_SCREEN_LINES   4
#define NUM_CHARS_LINE    26

// Must be extern to avoid being fully defined here and thus multiply defined.
extern unsigned char aucScreen[NUM_SCREEN_LINES][NUM_CHARS_LINE];
extern const unsigned char aucInstructions[];

void initialize_screen(void);
void print_screen(void);

#endif
