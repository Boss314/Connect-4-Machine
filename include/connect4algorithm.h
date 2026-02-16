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


/// @brief Current state of the game state machine.
/// 
/// Used by the main application to coordinate gameplay phases
/// such as waiting for input, calculating a move, or executing
/// a move on the board.
extern State_t current_state;

/// @brief Column selected by the AI as the next move to execute.
/// 
/// This value is written by the decision-making algorithm and
/// read by the game controller to perform the actual move.

extern Move_t move_to_make;
/// @brief Global game board instance.
/// 
/// Stores the current game state including piece positions,
/// column heights, and heuristic evaluation score.
/// 
/// Shared between gameplay logic and the AI search algorithm.
extern Board_t game_board;

/// @brief Maximum search depth for the Minimax algorithm.
/// 
/// Controls how many future moves the AI explores when evaluating
/// possible game states.
///
/// Larger values:
/// - stronger gameplay
/// - higher computational cost
///
/// Marked volatile because it may be modified at runtime,
/// for example through user settings or hardware input.
volatile uint8_t MAX_DEPTH;

/// @brief Result of a Minimax search step.
/// 
/// Stores both:
/// - the best move found
/// - the heuristic score of that move
///
/// Used to propagate optimal decisions up the search tree.
typedef struct {
    Col_t move;
    Score_t score;
} Result_t;

/// @brief Direction encoding for scanning board alignments.
/// 
/// Each value encodes a direction vector (dx, dy) used when
/// evaluating potential 4-in-a-row windows.
///
/// Encoding format (1 byte):
/// - high nibble  = dx (column step)
/// - low nibble   = dy + 1 (row step offset to allow -1, 0, +1)
///
/// Directions included:
/// - horizontal
/// - vertical
/// - diagonal rising
/// - diagonal falling
static const uint8_t DIRS[4] = {
    0x11,  // dx=1, dy=0: horizontal
    0x02,  // dx=0, dy=1: vertical  
    0x12,  // dx=1, dy=1: up diagonal
    0x10   // dx=1, dy=-1 down diagonal
};

#define GET_DX(dir) ((int8_t)((dir) >> 4))
#define GET_DY(dir) ((int8_t)(((dir) & 0x0F) - 1))

/// @brief Evaluates a 4-cell window of the board and assigns a heuristic score.
/// 
/// This function analyzes how many pieces from each player are present
/// in a group 3 cells (expect cell that next piece is coming) from 4 
/// consecutive cells (a potential Connect-4 line).
/// It returns a score indicating how favorable that window is for the
/// specified player perspective.
/// 
/// Scoring principles:
/// - Mixed window (both players present) → neutral (0)
/// - Empty window -> small positional value
/// - 1 or 2 pieces -> increasing score
/// - 3 enemy pieces -> higher score
/// - 3 my pieces → very high priority (win)
/// 
/// @param p1 Number of computer pieces in the window.
/// @param p2 Number of human player pieces in the window.
/// @param player Perspective of evaluation:
///        true  = evaluate for computer (maximizing)
///        false = evaluate for human (minimizing)
/// 
/// @return Heuristic score of the window from the given player's perspective.
Score_t eval_window(uint8_t p1, uint8_t p2, bool player);

/// @brief Computes the change in board score caused by placing a piece.
/// 
/// This function evaluates how the total board heuristic would change
/// if a piece is placed at the specified column and row.
/// It scans all directions (horizontal, vertical, diagonal) and evaluates
/// every 4-cell window affected by the move.
/// 
/// The function performs incremental evaluation, meaning it calculates
/// only the score difference instead of recomputing the entire board.
/// This significantly improves minimax performance.
/// 
/// If the move creates an immediate winning condition, a maximum or minimum
/// score is returned to signal a terminal state.
/// 
/// @param b Pointer to the current board state.
/// @param col Column where the piece will be placed.
/// @param row Row where the piece will land.
/// @param player Perspective of evaluation:
///        true  = computer move
///        false = human move
/// 
/// @return Score difference caused by the move.
///         May return SCORE_MAX or SCORE_MIN for immediate wins.
Score_t delta_score(Board_t *b, Col_t col, int row, bool player);

/// @brief Performs Minimax search with alpha–beta pruning to select the best move.
/// 
/// This recursive function explores possible future game states by simulating
/// alternating moves between the computer (maximizing player) and the human
/// opponent (minimizing player).
/// 
/// For each valid move:
/// - the move is applied to the board
/// - the resulting position is evaluated recursively
/// - the move is undone
/// - the best score is retained
/// 
/// Alpha–beta pruning is used to eliminate branches that cannot influence
/// the final decision, improving efficiency and allowing deeper search.
/// 
/// The recursion stops when:
/// - the search depth reaches zero
/// - the board is full
/// - a winning move is detected
/// 
/// @param b Pointer to the current board state.
/// @param depth Remaining search depth.
/// @param maximizing true if computer turn, false if human turn.
/// @param alpha Best score the maximizing player can guarantee so far.
/// @param beta  Best score the minimizing player can guarantee so far.
/// 
/// @return Result_t structure containing:
///         - best move (column index)
///         - evaluated score of that move
Result_t minimax(Board_t *b, int depth,
                 bool maximizing, Score_t alpha, Score_t beta);

/// @brief Determines and registers the computer's next move.
/// 
/// This function is the main entry point for the AI decision process.
/// It evaluates the current game board and selects the best column
/// for the computer to play.
///
/// Execution steps:
/// - Displays a waiting indicator
/// - Checks for any immediate winning move (1-ply search)
/// - If no immediate win exists, runs the Minimax algorithm
///   with alpha–beta pruning to evaluate future game states
/// - Stores the selected move in the global variable move_to_make
/// - Signals the system to execute the move
///
/// This function connects the decision-making algorithm to the
/// main game control logic.
void fn_CALCULATING_MOVE(void);

/// @brief Determines the current game state based on score change.
/// 
/// This function evaluates whether the game has reached a terminal state
/// after a move has been played. It checks for:
/// - computer victory
/// - human player victory
/// - draw (board full)
/// - ongoing game
///
/// The decision is based on score thresholds that represent a winning
/// alignment and whether the board is completely filled.
///
/// @param delta Score change produced by the most recent move.
/// 
/// @return GameState_t indicating:
///         - GAME_COMPUTER_WON
///         - GAME_PLAYER_WON
///         - GAME_DRAW
///         - GAME_ONGOING
GameState_t Game_winner(Score_t delta);

#endif /* CONNECT4ALGORITHM_H_ */
