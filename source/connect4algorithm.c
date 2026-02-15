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

#include "include/connect4algorithm.h"
#include "include/types.h"
#include "include/board.h"
#include "include/displaycontroller.h"
#include "stdio.h"

Score_t eval_window(uint8_t p1, uint8_t p2, bool player)
{
    // mixed
    if (p1 & p2) return 0;

    // empty 
    if (!(p1 | p2)) {
        return player ? 1 : -1;
    }

    if(player) {
        if(p1) {
            return (p1 == 3) ? 95 : (1 << p1);
        }
        else {
            return (p2 == 3) ? 5 : (1 << (p2 - 1));
        }
    }

    else {
        if(p2) {
            return (p2 == 3) ? -95 : -(1 << p2);
        }
        else {
            return (p1 == 3) ? -5 : -(1 << (p1 - 1));
        }
    }
}

Score_t delta_score(Board_t *b, Col_t col, int row, bool player)
{
    Score_t total_delta = -1;
    Score_t delta;
    int8_t d, i;
    for (d = 0; d < 4; d++) {
        uint8_t dir = DIRS[d];
        int8_t dx = GET_DX(dir);
        int8_t dy = GET_DY(dir);

        int8_t p1_count = 0;
        int8_t p2_count = 0;
        int8_t cells = 0;

        // counted first 3 cells 
        for (i = 3; i > 0; i--) {
            int8_t c = col - i * dx;
            int8_t r = row - i * dy;
            
            if ((c >= 0) & (c < NUM_COLS) & (r >= 0) & (r < NUM_ROWS)) {
                uint8_t m = (1 << r);
                p1_count += (b->p1[c] & m) ? 1 : 0;
                p2_count += (b->p2[c] & m) ? 1 : 0;
                cells++;
            }
            else { continue; }
        }

        // check if we have window 
        if(cells >= 3) {
            delta = eval_window(p1_count, p2_count, player);
            if (delta == 95) return SCORE_MAX;
            if (delta == -95) return SCORE_MIN;
            total_delta += delta;
        }

        // counted last 3 cells
        for (i = 1; i <= 3; i++) {

            int8_t c = col + i * dx;
            int8_t r = row + i * dy;
            
            // check next cell is valid
            if ((c >= 0) & (c < NUM_COLS) & (r >= 0) & (r < NUM_ROWS)) {
                uint8_t m = (1 << r);
                p1_count += (b->p1[c] & m) ? 1 : 0;
                p2_count += (b->p2[c] & m) ? 1 : 0;
                cells++;

                // check if we have window 
                if(cells >= 3) {
                    // Remove leftmost cell if window has 4 cells
                    if(cells >= 4) {
                        int8_t c_del = c - (dx << 2); // multiply by 4 using shift
                        int8_t r_del = r - (dy << 2);
                        uint8_t m_del = (1 << r_del);
                        p1_count -= (b->p1[c_del] & m_del) ? 1 : 0;
                        p2_count -= (b->p2[c_del] & m_del) ? 1 : 0;
                    }

                    delta = eval_window(p1_count, p2_count, player);
                    if (delta == 95) return SCORE_MAX;
                    if (delta == -95) return SCORE_MIN;
                    total_delta += delta;
                }
        
            }
            // if not no need to continue
            else { break; }
        }

    }
    return total_delta;
}

Result_t minimax(Board_t *b, int depth, 
                 bool maximizing, Score_t alpha, Score_t beta)
{
    // board is full or 0 depth
    if (depth == 0 || Board_full(b))
        return (Result_t){0, b->score};

    Result_t best;
    best.move = 0;
    best.score = maximizing ? SCORE_MIN : SCORE_MAX;
    Col_t c;

    for (c = 0; c < NUM_COLS; c++) {

        if (b->height[c] >= NUM_ROWS) continue;

        int8_t row = b->height[c];

        // calculate how score will change 
        Score_t delta = delta_score(b, c, row, maximizing);

        Result_t r;

        // There is a winning move in 1
        if((delta == SCORE_MAX) & maximizing) {
            r = (Result_t){c, SCORE_MAX};
        }
        else if((delta == SCORE_MIN) & !maximizing) {
            r = (Result_t){c, SCORE_MIN};
        }

        else {
            // change board
            Board_make_move(b, c, maximizing, delta);

            // recurse
            r = minimax(b, depth - 1,
                                !maximizing, alpha, beta);

            // revert board
            Board_unmake_move(b, c, maximizing, delta);
        }
        
        if (maximizing) {
            if (r.score > best.score) {
                best.score = r.score;
                best.move = c;
            }
            if (best.score > alpha) alpha = best.score;
        } else {
            if (r.score < best.score) {
                best.score = r.score;
                best.move = c;
            }
            if (best.score < beta) beta = best.score;
        }

        if (alpha >= beta) break;
    }
    return best;
}

// register bot's next move
void fn_CALCULATING_MOVE(void) {

    //write the waiting indicator on the display
    Display_write();

    // check one move win
    Col_t c;
    for (c = 0; c < NUM_COLS; c++) {
        if (game_board.height[c] >= NUM_ROWS) continue;
        int8_t row = game_board.height[c];

        // calculate how score will change 
        Score_t delta = delta_score(&game_board, c, row, true);
        if((delta == SCORE_MAX)) {
            move_to_make = c;
            current_state=STATE_MAKING_MOVE;
            break;
        }
    }

    Result_t r = minimax(&game_board, MAX_DEPTH, true, SCORE_MIN, SCORE_MAX);
    Col_t bot_move = r.move;
    move_to_make = bot_move;
    current_state=STATE_MAKING_MOVE;
}

// winner function
GameState_t Game_winner(Score_t delta) {
    char msg[16];
    snprintf(msg, sizeof(msg), "delta: %d", delta);
    Display_printCentered(msg);

    if (delta >= 95) {
        return GAME_COMPUTER_WON;
    }
    if (delta <= -95) {
        return GAME_PLAYER_WON;
    }
    if (Board_full(&game_board)) {
        return GAME_DRAW;
    }
    return GAME_ONGOING;
}



