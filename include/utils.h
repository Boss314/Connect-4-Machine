/*
 * utils.h
 *
 *  Created on: Dec 2, 2025
 *      Author: enrico
 */

#include "types.h"
#include "stdbool.h"


#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

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
bool Board_make_move(Board_t * board,Move_t move,bool is_computer);

/* function to roll back a move made on a board and bring it back to a previous state
 *      Board_t * board:     board to update with the modified state, passed by reference
 *      Move_t move:         move to roll back on the board
 *
 *      returns bool:        true if it was possible to undo the given move and the state of the board was succesfully modified, false otherwise
 */
bool Board_unmake_move(Board_t * board,Move_t move);

/* function to check the game has been won, that is, to check if the board contains four pieces in a line all belonging to either the player or the computer
 *      Board_t * board:     board to check for victory, passed by reference
 *      bool * is_computer   true if the game has been won by the computer, false if the game has been won by the player
 *
 *      returns bool:        true if the game has been won either by the player or the computer, false otherwise
 */
bool Board_has_won(const Board_t * board, bool * winner);


#endif /* INCLUDE_UTILS_H_ */
