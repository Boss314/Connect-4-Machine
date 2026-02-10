/*
 * board.h
 *
 *  Created on: Dec 2, 2025
 *      Author: enrico
 */

#include <include/types.h>
#include "stdbool.h"


#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_


// board struct changed by Temuulen
typedef struct {
    uint8_t p1[NUM_COLS];   // bot
    uint8_t p2[NUM_COLS];   // player
    uint8_t height[NUM_COLS];
    Score_t score;
} Board_t;

/* function that initialises a blank game of connect 4
 *      Board_t * board: board passed by reference to initialize
 */
void Board_init(Board_t * board);

/* function to modify the state of a connect 4 game to make a move on the board
 *      Board_t * board:     board to update with the modified state, passed by reference
 *      Move_t move:         move to make on the board
 *      bool is_computer:    true if the move is being made by the computer, false if the move is being made by the player
 *
 *      returns bool:        true if the given move was legal and the state of the board was succesfully modified, false otherwise
 */
bool Board_make_move(Board_t * board, Move_t move, bool is_computer, Score_t delta);

/* function to roll back a move made on a board and bring it back to a previous state
 *      Board_t * board:     board to update with the modified state, passed by reference
 *      Move_t move:         move to roll back on the board
 *
 *      returns bool:        true if it was possible to undo the given move and the state of the board was succesfully modified, false otherwise
 */
bool Board_unmake_move(Board_t* board, const Move_t move, const bool is_computer, Score_t delta);

bool Board_full(const Board_t *board);
#endif /* INCLUDE_UTILS_H_ */
