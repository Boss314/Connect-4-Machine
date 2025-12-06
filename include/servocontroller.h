/*
 * servocontroller.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */

#include "types.h"


#ifndef SERVOCONTROLLER_SERVOCONTROLLER_H_
#define SERVOCONTROLLER_SERVOCONTROLLER_H_

extern State_t current_state;
extern Move_t move_to_make;


/* function that uses the value stored in the global variable move_to_make
 * and actionates the servos to make the device make a move in the corresponding column,
 * puts the machine in the next state,
 * then returns control to main
 */
void fn_MAKING_MOVE(void);



#endif /* SERVOCONTROLLER_SERVOCONTROLLER_H_ */
