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
// they are defined as powers of 2 so they can be used as bitmasks on the elements of the column field of Board_t
// NOTE: an actual connect 4 board only has 6 rows
typedef uint8_t Row_t;
#define NUM_ROWS 6;
#define ROW1 (uint8_t)(0x01)
#define ROW2 (uint8_t)(0x02)
#define ROW3 (uint8_t)(0x04)
#define ROW4 (uint8_t)(0x08)
#define ROW5 (uint8_t)(0x10)
#define ROW6 (uint8_t)(0x20)
#define ROW7 (uint8_t)(0x40)
#define ROW8 (uint8_t)(0x80)


// typedef for the datatype used to memorize the score of a particular connect 4 board
// maximum value means a state of victory for the computer, minimum value means victory for the player
typedef int16_t Score_t;
#define SCORE_MIN INT16_MIN
#define SCORE_MAX INT16_MAX


/* struct type to indicate the state of a connect 4 board
 * column: an array of 7 uint8_t used as bitfields, the j-th lowest bit of column[i] identifies the piece present in the j-th row (from the
 *         bottom) of the i-th column (from the left): 1 for pieces belonging to the computer, 0 for pieces belonging to the player
 *
 * next_row: array of 7 Row_t variables that identify where the next pieces shpuld be placed: next_row[i] is the row where a piece would
 *           fall if it was placed in the i-th column (from the left), it is the lowest row in that column that doesnt have a piece in it,
 *           all lower rows are guaranteed to have pieces in them and all higher rows are guaranteed to be empty
 *
 * score: variable that contain the score of the game board's state, used by the minimax algorithm. if the score is the maximum value
 *        then the board is in a state where the computer has won, if the score is the minimum value then the player has won
 *
 * NOTE: the values of the bits in any column[i] field are only defined up to (and not including) the row indicated in next_row[i], all higher
 *       bits should never be considered because they correspond to places where there are no pieces
 */
typedef struct{
    Col_t column[NUM_COLS];
    Row_t next_row[NUM_COLS];
    Score_t score;
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



// typedef for the format of a digit/letter as displayed on the 7 segment display, each bit corresponds to a different segment, 1 for on and 0 for off
typedef uint8_t SevenSegment_t;
/*  what bits correspond to what segments is determined by the exact wiring of the circuit the display is on, and by the way the bits are inserted in the 74hc595 shift register
 *  the mapping currently looks like this, with 0 being the least significant bit and 7 being the most significant
 *
 *    --5--
 *   |     |
 *   4     6
 *   |     |
 *    --3--
 *   |     |
 *   2     7
 *   |     |
 *    --1--
 */                                                 //76543210
#define SEVEN_SEGMENT_BLANK         (SevenSegment_t)0b00000000
#define SEVEN_SEGMENT_ZERO          (SevenSegment_t)0b11110110
#define SEVEN_SEGMENT_ONE           (SevenSegment_t)0b11000000
#define SEVEN_SEGMENT_TWO           (SevenSegment_t)0b01101110
#define SEVEN_SEGMENT_THREE         (SevenSegment_t)0b11101010
#define SEVEN_SEGMENT_FOUR          (SevenSegment_t)0b11011000
#define SEVEN_SEGMENT_FIVE          (SevenSegment_t)0b10111010
#define SEVEN_SEGMENT_SIX           (SevenSegment_t)0b10111110
#define SEVEN_SEGMENT_SEVEN         (SevenSegment_t)0b11100000
#define SEVEN_SEGMENT_EIGHT         (SevenSegment_t)0b11111110
#define SEVEN_SEGMENT_NINE          (SevenSegment_t)0b11111010



#endif /* TYPES_H_ */
