#include "types.h"

#ifndef DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_
#define DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_

extern State_t current_state;
extern Move_t move_to_make;
extern Board_t game_board;

// function that handles the STATE_MAKING_MOVE state
// shows the computer's move on the display, waits for the sensor, updates board and state
void fn_MAKING_MOVE(void);

// function that updates the display depending on the current state
void Display_write(void);

// function that initializes the LCD display and graphics context
void Display_init(void);

// function to clear the entire display
void Display_clear(void);

// function to draw a string centered on the display
// char *text: text to display
void Display_printCentered(char *text);


#endif /* DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_ */
