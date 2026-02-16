/*
 * board.h
 *
 *  Created on: Dec 2, 2025
 *      Author: enrico
 */

#include "include/types.h"
#include "stdbool.h"


#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_


/// @brief Initializes a Connect 4 board to an empty starting state.
/// 
/// Resets all internal board data structures, including:
/// - piece positions for both players
/// - column heights
/// - accumulated heuristic score
///
/// This function must be called before starting a new game or
/// before reusing a board structure.
///
/// @param board Pointer to the board structure to initialize.
void Board_init(Board_t * board);

/// @brief Applies a move to the board and updates the game state.
/// 
/// Places a piece in the specified column and updates:
/// - the player bitboard (computer or human)
/// - the column height
/// - the incremental heuristic score
///
/// This function is used both for real gameplay and for simulated
/// moves during Minimax search.
///
/// @param board Pointer to the board to modify.
/// @param move Column index where the piece will be placed.
/// @param is_computer true  → computer places a piece (O)
///                    false → human player places a piece (X)
/// @param delta Precomputed score change produced by this move.
///
/// @note The move is assumed to be legal when called.
///       No legality check is performed inside this function.
void Board_make_move(Board_t * board, Move_t move, bool is_computer, Score_t delta);

/// @brief Reverts a previously applied move and restores the board state.
/// 
/// Removes the most recent piece placed in the specified column and
/// restores:
/// - the player bitboard
/// - the column height
/// - the accumulated heuristic score
///
/// This function enables efficient backtracking during Minimax search
/// without copying the board.
///
/// @param board Pointer to the board to modify.
/// @param move Column index from which the last piece will be removed.
/// @param is_computer true  → remove computer piece (O)
///                    false → remove human piece (X)
/// @param delta Score change that was applied when the move was made.
///              This value is subtracted to restore the previous score.
///
/// @note The move must correspond to the most recently placed piece
///       in that column.
void Board_unmake_move(Board_t* board, const Move_t move, const bool is_computer, Score_t delta);

/// @brief Checks whether the board is completely full.
/// 
/// A board is considered full when every column has reached
/// the maximum number of rows and no further moves are possible.
///
/// This condition indicates a draw if no winning alignment exists.
///
/// @param board Pointer to the board to check.
///
/// @return true if no legal moves remain, false otherwise.
bool Board_full(const Board_t *board);
#endif /* INCLUDE_UTILS_H_ */
