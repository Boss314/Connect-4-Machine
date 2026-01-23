/*
 * connect4algorithm.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */

#include "types.h"

#ifndef CONNECT4ALGORITHM_H_
#define CONNECT4ALGORITHM_H_


extern State_t current_state;
extern Move_t move_to_make;


//function that updates the value of move_to_make with the move calculated by the algorithm, puts the machine in its next state, and then returns control to main
void fn_CALCULATING_MOVE(void);

//
Score_t score_contribution(Board_t *board, Col_t column, Row_t row);

#endif /* CONNECT4ALGORITHM_H_ */
