/*
 * connect4algorithm.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */

#include "types.h"
#include "stdbool.h"

#ifndef CONNECT4ALGORITHM_H_
#define CONNECT4ALGORITHM_H_

#define MAX_DEPTH 6
#define SCORE_MIN INT16_MIN
#define SCORE_MAX INT16_MAX


extern State_t current_state;
extern Move_t move_to_make;


typedef struct {
    Col_t move;
    Score_t score;
} Result_t;

static const uint8_t DIRS[4] = {
    0x11,  // dx=1, dy=0: horizontal
    0x02,  // dx=0, dy=1: vertical  
    0x12,  // dx=1, dy=1: up diagonal
    0x10   // dx=1, dy=-1 down diagonal
};

#define GET_DX(dir) ((int8_t)((dir) >> 4))
#define GET_DY(dir) ((int8_t)(((dir) & 0x0F) - 1))

Score_t eval_window(uint8_t p1, uint8_t p2, bool player);

Score_t delta_score(Board_t *b, Col_t col, int row, bool player);

Result_t minimax(Board_t *b, int depth, 
                 bool maximizing, Score_t alpha, Score_t beta);

//function that updates the value of move_to_make with the move calculated by the algorithm, puts the machine in its next state, and then returns control to main
void fn_CALCULATING_MOVE(void);

GameState_t Game_winner(void);

#endif /* CONNECT4ALGORITHM_H_ */
