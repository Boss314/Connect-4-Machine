/*
 * types.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */


#include "stdint.h"


#ifndef TYPES_H_
#define TYPES_H_

// typedef to indicate the columns in a connect 4 board
// a column can also be used to identify where a player has made a move on the board, so we also define Move_t to identify moves
// they are defined as sequential integers (rather than an enum type) to be used as indexes to the column field of Board_t
typedef uint8_t Col_t;
typedef Col_t Move_t;
#define NUM_COLS 7
#define COL1 (uint8_t)(0)
#define COL2 (uint8_t)(1)
#define COL3 (uint8_t)(2)
#define COL4 (uint8_t)(3)
#define COL5 (uint8_t)(4)
#define COL6 (uint8_t)(5)
#define COL7 (uint8_t)(6)
#define COL(x) (uint8_t)(x-1)


//typedef to indicate the rows of the connect 4 board
//an actual connect 4 board only has 6 rows
typedef uint8_t Row_t;
#define NUM_ROWS 6


// typedef for the datatype used to memorize the score of a particular connect 4 board
// maximum value means a state of victory for the computer, minimum value means victory for the player
typedef int16_t Score_t;
#define SCORE_MIN INT16_MIN
#define SCORE_MAX INT16_MAX


/* struct type to indicate the state of a connect 4 board
 *  p1 and p2: arrays of uint8_t elements used as bitfields that correspond to the columns of the connect 4 board, a vaule of 1 represents a place with a piece in it
 *             the least significant bit represents the lowest piece in that column and the most significant represents the highest
 *             p1 contains the information about the computer's pieces, p2 about the player's pieces
 *
 *  height:    array that represents the position of the highest piece in a column, all places above the height[i]-th place in the i-th column are blank
 *             and all other pieces are guaranteed to have a piece in them
 *
 *  score:     Score_t variable used to keep track of how favorable the state the board is in is, used by the algorithm
 */
// board struct changed by Temuulen
typedef struct {
    uint8_t p1[NUM_COLS];   // bot
    uint8_t p2[NUM_COLS];   // player
    uint8_t height[NUM_COLS];
    Score_t score;
} Board_t;


/* enum type of the various states the device can be in:
 * -STATE_INIT: the initial state of the machine during which initialisation of the device is done
 * -STATE_WAITING_FOR_MOVE: state the machine is in when it is waiting for the human player to make a move
 * -STATE_CALCULATING_MOVE: state the machine is in while it is calculating the next move to make using the connect 4 algorithm
 * -STATE_MAKING_MOVE: state the machine is in while it is moving the arm to make the move chosen by the connect 4 algorithm
 * -STATE_PLAYER_VICTORY: state the machine is in when the game is over and the human player has won
 * -STATE_COMPUTER_VICTORY: state the machine is in when the game is over and the computer has won
 * -STATE_DRAW: state the machine is in when the game is over and neither player has won
 */
typedef enum {
    STATE_INIT,
    STATE_WAITING_FOR_MOVE,
    STATE_CALCULATING_MOVE,
    STATE_MAKING_MOVE,
    STATE_PLAYER_VICTORY,
    STATE_COMPUTER_VICTORY,
    STATE_DRAW,
    NUM_STATES
}State_t;


// struct type thet defines the state machine used in main.c
typedef struct{
    State_t state;
    void (*state_function)(void);
} StateMachine_t;


/* enum type of the various states the device can be in:
 * -GAME_COMPUTER_WON: the game is over and has been won by the computer
 * -GAME_PLAYER_WON: the game is over and has been won by the player
 * -GAME_DRAW: the game is over because the board is full but neither player has managed to create a line of 4 pieces
 * -GAME_ONGOING: the game is not yet over
 */
typedef enum {
    GAME_COMPUTER_WON,
    GAME_PLAYER_WON, 
    GAME_DRAW,
    GAME_ONGOING
} GameState_t;





#endif /* TYPES_H_ */
