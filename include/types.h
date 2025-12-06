/*
 * types.h
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */


#include "stdint.h"


#ifndef TYPES_H_
#define TYPES_H_

// typedef to indicate the 7 possible moves that can be made in connect 4, corresponding to the 7 columns where one can place a piece
// they are defined as sequential integers (rather than an enum type) to be used as indexes to the column field of Board_t
typedef uint8_t Move_t;
#define MOVE_COL1 (uint8_t)(0)
#define MOVE_COL2 (uint8_t)(1)
#define MOVE_COL3 (uint8_t)(2)
#define MOVE_COL4 (uint8_t)(3)
#define MOVE_COL5 (uint8_t)(4)
#define MOVE_COL6 (uint8_t)(5)
#define MOVE_COL7 (uint8_t)(6)


//typedef to indicate the 6 rows of the connect 4 board
// they are defined as powers of 2 so they can be used as bitmasks on the elements of the column field of Board_t
typedef uint8_t Row_t;
#define ROW1 (uint8_t)(0x0001)
#define ROW2 (uint8_t)(0x0002)
#define ROW3 (uint8_t)(0x0004)
#define ROW4 (uint8_t)(0x0008)
#define ROW5 (uint8_t)(0x0010)
#define ROW6 (uint8_t)(0x0020)
#define ROW7 (uint8_t)(0x0040)


/* struct type to indicate the state of a connect 4 board
 * column: an array of 7 uint8_t used as bitfields, the j-th lowest bit of column[i] identifies the piece present in the j-th row (from the
 *         bottom) of the i-th column (from the left): 1 for pieces belonging to the computer, 0 for pieces belonging to the player
 *
 * next_row: array of 7 Row_t variables that identify where the next pieces shpuld be placed: pieces_placed[i] is the row where a piece would
 *           fall if it was placed in the i-th column (from the left), it is the lowest row in that column that doesnt have a piece in it,
 *           all lower rows are guaranteed to have pieces in them and all higher rows are guaranteed to be empty
 *
 * NOTE: the values of the bits in any column[i] field are only defined up to (and not including) the row indicated in pieces_placed[i], all higher
 *       bits should never be considered because they correspond to places where there are no pieces
 */
typedef struct{
    uint8_t column[7];
    Row_t next_row[7];
} Board_t;




/* enum type of the various states the device can be in:
 * -STATE_INIT: the initial state of the machine during which initialisation of the device is done
 * -STATE_WAITING_FOR_MOVE: state the machine is in when it is waiting for the human player to make a move
 * -STATE_CALCULATING_MOVE: state the machine is in while it is calculating the next move to make using the connect 4 algorithm
 * -STATE_MAKING_MOVE: state the machine is in while it is moving the arm to make the move chosen by the connect 4 algorithm
 * -STATE_PLAYER_VICTORY: state the machine is in when the game is over and the human player has won
 * -STATE_COMPUTER_VICTORY: state the machine is in when the game is over and the computer has won
 */
typedef enum {
    STATE_INIT,
    STATE_WAITING_FOR_MOVE,
    STATE_CALCULATING_MOVE,
    STATE_MAKING_MOVE,
    STATE_PLAYER_VICTORY,
    STATE_COMPUTER_VICTORY,
    NUM_STATES
}State_t;


// struct type thet defines the state machine used in main.c
typedef struct{
    State_t state;
    void (*state_function)(void);
} StateMachine_t;


#endif /* TYPES_H_ */
