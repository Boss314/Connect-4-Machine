/*
 * displaycontroller.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */

#include "types.h"


#ifndef DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_
#define DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_

extern State_t current_state;
extern Move_t move_to_make;
extern Board_t game_board;


/* function that uses the value stored in the global variable move_to_make
 * and tells the player to insert a piece in the corresponding column,
 * waits for that piece to be inserted,
 * puts the machine in the next state,
 * then returns control to main
 */
void fn_MAKING_MOVE(void);

/* function to display data on the 7 segment display via the shift register
 * SevenSegment_t a: data to display
 */
void Display_write(SevenSegment_t a);

// function to initialise the code related to the 7 segment display
// should be called before the display ever needs to be used and only ONCE during the entire lifetime of the program
void Display_init(void);


#endif /* DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_ */
