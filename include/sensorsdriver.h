/*
 * sensorsdriver.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */

#include "include/types.h"

#ifndef SENSORSDRIVER_SENSORSDRIVER_H_
#define SENSORSDRIVER_SENSORSDRIVER_H_

extern State_t current_state;
extern Board_t game_board;

/* function that prepares the sensors at the top of the columns and waits for
 * them to detect the human players move the sensors will trigger an interrupt
 * that will update the data structure that holds the current state of the game
 * board will then return control to main
 */
void fn_WAITING_FOR_MOVE(void);

// function to initialise the code related to the sensors
// should be called before the sensors ever need to be used and only ONCE during
// the entire lifetime of the program
void Sensors_init(void);

// function that activates the sensor on a specified column and returns once it
// detects a piece being placed in that column used by the fn_MAKING_MOVE
// function Col_t column:    column on which we are requesting a piece to be
// placed
void Sensors_request_piece(Col_t column);

#endif /* SENSORSDRIVER_SENSORSDRIVER_H_ */
