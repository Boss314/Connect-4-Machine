/*
 * board.c
 *
 *  Created on: Dec 2, 2025
 *      Author: enrico
 */


#include "include/board.h"
#include "include/types.h"
#include "include/connect4algorithm.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

// changed by Temuulen
void Board_init(Board_t* board){
    memset(board, 0, sizeof(Board_t));
}

// changed by Temuulen
void Board_make_move(Board_t* board, const Move_t move, const bool is_computer, Score_t delta){
    uint8_t r = board->height[move];
    uint8_t mask = (1 << r);
    if (is_computer) 
        board->p1[move] |= mask;
    else 
        board->p2[move] |= mask;
    board->height[move]++;
    board->score += delta;
}

// changed by Temuulen
void Board_unmake_move(Board_t* board, const Move_t move, const bool is_computer, Score_t delta){
    board->height[move]--;
    uint8_t r = board->height[move];
    uint8_t mask = (1 << r);
    if (is_computer) 
        board->p1[move] &= ~mask;
    else 
        board->p2[move] &= ~mask;
    board->score -= delta;
}

// added by Temuulen
bool Board_full(const Board_t *board)
{
    Col_t i;
    for (i = 0; i < NUM_COLS; i++) {
        if (board->height[i] < NUM_ROWS) 
        return false;
    }
    return true;
}




