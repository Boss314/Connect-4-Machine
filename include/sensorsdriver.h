/*
 * sensorsdriver.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */

#include "types.h"


#ifndef SENSORSDRIVER_SENSORSDRIVER_H_
#define SENSORSDRIVER_SENSORSDRIVER_H_


extern State_t current_state;


/* function that prepares the sensors at the top of the columns and waits for them to detect the human players move
 * the sensors will trigger an interrupt that will update the data structure that holds the current state of the game
 * board will then return control to main
 */
void fn_WAITING_FOR_MOVE(void);



#endif /* SENSORSDRIVER_SENSORSDRIVER_H_ */
