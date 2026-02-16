/* this file contains the declarations
 * and initializations of the various
 * global variables used throughout the
 * project
 */


#include "msp.h"
#include "include/types.h"
#include <stdbool.h>

// variable that holds the column that the algorithm has decided to make its move into
// this variable's value gets set by the function fn_CALCULATING_MOVE during the state STATE_CALCULATING_MOVE
// its value is later used by the fn_MAKING_MOVE function during the STATE_MAKING_MOVE state
Move_t move_to_make;

// variable that holds the current state of the state machine
// initialized to the init state
State_t current_state = STATE_INIT;

//variable that holds the current state of the game board, must be initialized with Board_init() before use
Board_t game_board;

// variables used as flags for when the pushbutton 1 and 2 on the boosterpack have been pressed
volatile bool button1_pressed=false;
volatile bool button2_pressed=false;
