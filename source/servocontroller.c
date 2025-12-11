/*
 * servocontroller.c
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */



/* this file contains the implementation of the functions that allow the device to make a move on the game board by moving the arm.
 * it is accessed by the main application by way of the fn_MAKING_MOVE function
 */



#include "include/servocontroller.h"
#include "include/types.h"


void fn_MAKING_MOVE(void){
    current_state=STATE_WAITING_FOR_MOVE;
}
