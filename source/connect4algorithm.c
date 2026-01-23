/*
 * connect4algorithm.c
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */

/* this file contains the logic that calculates the optimal move for the computer to make in the current gamestate.
 * it is accessed by the main application by way of the fn_CALCULATING_MOVE function: this function signals the move
 * it has decided to make by storing it into the global Move_t move_to_make
 */


#include <include/types.h>
#include "include/connect4algorithm.h"



void fn_CALCULATING_MOVE(void){
    //move_to_make = ...

    current_state=STATE_MAKING_MOVE;
}


