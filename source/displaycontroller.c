/*
 * displaycontroller.c
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */



/* this file contains the implementation of the functions that allow the device to write on the 7 segment display, particularly to make a move on the game board.
 * it is accessed by the main application by way of the fn_MAKING_MOVE function
 */


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "include/displaycontroller.h"
#include "include/types.h"
#include "include/sensorsdriver.h"
#include "include/utils.h"

// array of the display elements so they can be accessed by indexing with move_to_make
SevenSegment_t digits[NUM_COLS]={
                    SEVEN_SEGMENT_ONE,
                    SEVEN_SEGMENT_TWO,
                    SEVEN_SEGMENT_THREE,
                    SEVEN_SEGMENT_FOUR,
                    SEVEN_SEGMENT_FIVE,
                    SEVEN_SEGMENT_SIX,
                    SEVEN_SEGMENT_SEVEN,
};

// constants for the pins connected to the the clock, latch, and serial data pins of the shift register
const uint_fast8_t CLOCK_PORT=GPIO_PORT_P3;    const uint_fast16_t CLOCK_PIN=GPIO_PIN5;
const uint_fast8_t SDATA_PORT=GPIO_PORT_P3;    const uint_fast16_t SDATA_PIN=GPIO_PIN6;
const uint_fast8_t LATCH_PORT=GPIO_PORT_P3;    const uint_fast16_t LATCH_PIN=GPIO_PIN7;


void fn_MAKING_MOVE(void){
    //write the move we want to make on the display
    Display_write(digits[move_to_make]);

    //wait for the sensor in that column to detect a move
    Sensors_request_piece(move_to_make);

    //apply the move we made to the board
    Board_make_move(&game_board, move_to_make, true);

    //check if the computer won and decide the next state
    bool winner;
    bool isfinished=Board_has_won(&game_board, &winner);
    if(isfinished){
        if(winner){
            current_state=STATE_COMPUTER_VICTORY;
        }else{
            //some kind of error, the computer just made a move but the player won
            //Default_Handler();
        }
    }else{
        current_state=STATE_WAITING_FOR_MOVE;
    }

    //clear the display
    Display_write(SEVEN_SEGMENT_BLANK);
}


void Display_write(SevenSegment_t a){
    int i;
    for(i=0;i<8;i++){//loop 8 times to transmit 8 bits
        if(a & (1<<i)){//transmit the value of the bit through the serial data pin
            GPIO_setOutputHighOnPin(SDATA_PORT, SDATA_PIN);
        }else{
            GPIO_setOutputLowOnPin(SDATA_PORT, SDATA_PIN);
        }

        //cycle the clock of the shift register
        GPIO_setOutputHighOnPin(CLOCK_PORT, CLOCK_PIN);
        GPIO_setOutputLowOnPin(CLOCK_PORT, CLOCK_PIN);
    }

    //when were done activate the latch to send the data from the shift register to the output
    GPIO_setOutputHighOnPin(LATCH_PORT, LATCH_PIN);
    GPIO_setOutputLowOnPin(LATCH_PORT, LATCH_PIN);
}


void Display_init(void){
    //set up the clock, latch, and serial data pins as output and set them all to low
    GPIO_setAsOutputPin(CLOCK_PORT, CLOCK_PIN);
    GPIO_setAsOutputPin(SDATA_PORT, SDATA_PIN);
    GPIO_setAsOutputPin(LATCH_PORT, LATCH_PIN);

    GPIO_setOutputLowOnPin(CLOCK_PORT, CLOCK_PIN);
    GPIO_setOutputLowOnPin(SDATA_PORT, SDATA_PIN);
    GPIO_setOutputLowOnPin(LATCH_PORT, LATCH_PIN);

    //clear the display
    Display_write(SEVEN_SEGMENT_BLANK);
}
