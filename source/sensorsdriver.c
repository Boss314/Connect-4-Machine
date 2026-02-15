/*
 * sensorsdriver.c
 *
 *  Created on: Nov 26, 2025
 *      Author: enrico
 */


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "include/board.h"
#include "include/sensorsdriver.h"
#include "include/connect4algorithm.h"
#include "include/displaycontroller.h"
#include "stdbool.h"




// type to hold the data of where a proximity sensor is connected
typedef struct{
    uint_fast8_t port; //port the sensor is connected to
    uint_fast16_t pin; //pin the sensor is connected to
    Col_t column; //column the sensor is on
} Sensor_t;

//array of the pins all the sensors are connected to
// next to them are the pins they correspond to on the boosterpack
const Sensor_t sensors[NUM_COLS] = {
                             {GPIO_PORT_P3,GPIO_PIN6,COL1}, //J2.11
                             {GPIO_PORT_P3,GPIO_PIN0,COL2}, //J2.18
                             {GPIO_PORT_P5,GPIO_PIN2,COL3}, //J2.12
                             {GPIO_PORT_P6,GPIO_PIN6,COL4}, //J4.36
                             {GPIO_PORT_P6,GPIO_PIN7,COL5}, //J4.35
                             {GPIO_PORT_P2,GPIO_PIN3,COL6}, //J4.34
                             {GPIO_PORT_P4,GPIO_PIN6,COL7}  //J1.8
};

// global variable used only in this file that stores in what column a move from the player has been detected
volatile Move_t move_detected;


//function to enable the all interrupts on the sensor pins
void enable_interrupts();
//function to disable all the interrupts on the sensor pins
void disable_interrupts();
//function that processes an interrupt triggered by a sensor, it gets called by the ISRs that pass it the port the interrupt came from
void process_interrupt(uint_fast8_t port);

void fn_WAITING_FOR_MOVE(void){
    //write the indicator to let the player move on the display
    Display_write();

    //allow the sensors to trigger an interrupt
    enable_interrupts();

    //go to sleep
    PCM_gotoLPM0();

    //when we get out of sleep it means there's been an interrupt, so let's apply the move we detected
    int8_t row = game_board.height[move_detected];
    Score_t delta = delta_score(&game_board, move_detected, row, false);
    Board_make_move(&game_board, move_detected, false, delta);

    //check if the player won and decide the next state
    GameState_t state = Game_winner(delta);

    char buffer[13] = "You chose: ";
    buffer[11] = '0' + (move_detected + 1);
    buffer[12] = '\0';

    Display_printCentered(buffer);

    switch (state) {
        case GAME_COMPUTER_WON:
            current_state = STATE_COMPUTER_VICTORY;
            break;
        case GAME_PLAYER_WON:
            current_state = STATE_PLAYER_VICTORY;
            break;
        case GAME_DRAW:
            current_state = STATE_DRAW;
            break;
        case GAME_ONGOING:
            current_state = STATE_CALCULATING_MOVE;
            break;
    }
}


void Sensors_init(void){
    int i;
    for(i=0;i<NUM_COLS;i++){
        // the sensors we use (tcrt5000) output low when they detect something, so we set them to have pullup resistors and to trigger an interrupt when the input gets low
        GPIO_setAsInputPinWithPullUpResistor(sensors[i].port, sensors[i].pin);
        GPIO_interruptEdgeSelect(sensors[i].port, sensors[i].pin, GPIO_HIGH_TO_LOW_TRANSITION);
    }

    //enable the interrupts of the gpio ports, the most convenient way to do this is by hardcoding the interrupt numbers
    Interrupt_enableInterrupt(INT_PORT2);
    Interrupt_enableInterrupt(INT_PORT3);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT6);
    Interrupt_enableMaster(); //just in case interrupts arent enabled we enable them
}


void Sensors_request_piece(Col_t column){
    Sensor_t sensor;

    int i;
    for(i=0;i<NUM_COLS;i++){ // find the sensor that corresponds to the requested column
        if(sensors[i].column == column){
            sensor=sensors[i];
            break;
        }
    }

    //enable the interrupt on that sensor
    GPIO_clearInterruptFlag(sensor.port, sensor.pin);
    GPIO_enableInterrupt(sensor.port, sensor.pin);

    //go to sleep
    PCM_gotoLPM0();

    //when we exit from sleep it mean the sensor detected a move on the requested column
    return;
}


void enable_interrupts(){
    int i;
    for(i=0;i<NUM_COLS;i++){ //first clear the flags
        GPIO_clearInterruptFlag(sensors[i].port, sensors[i].pin);
    }

    for(i=0;i<NUM_COLS;i++){ //then enable the interrupts
        GPIO_enableInterrupt(sensors[i].port, sensors[i].pin);
    }
}


void disable_interrupts(){
    int i;
    for(i=0;i<NUM_COLS;i++){
        GPIO_disableInterrupt(sensors[i].port, sensors[i].pin);
    }
}




/*
 * we should detect exactly one move for every time fn_WAITING_FOR_MOVE is called, so all the interrupt routines need mutual exclusion to the move_detected variable and to the Board_t variable
 * to do this we make each interrupt routine disable all the sensor interrupts using disable_interrupts(), so that only one routine can happen at a time
 */
void process_interrupt(uint_fast8_t port){
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(port);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(port, status);

    //go through the list of the sensors to see which triggered the interrupt
    int i;
    for(i=0;i<NUM_COLS;i++){
        if(sensors[i].port == port){ //only consider sensors belonging to this port
            if(status & sensors[i].pin){ //check if that sensor's pin is the one that triggered
                move_detected = sensors[i].column; //set the value of the move we detected
                break;
            }
        }
    }

    //block other sensors from triggering interrupts before returning
    disable_interrupts();
}


// following here are the implementations of the interrupt routines triggered by the sensors
// each of them just calls the interrupt processing function passing it the port it came from

void PORT2_IRQHandler(void){
    process_interrupt(GPIO_PORT_P2);
}

void PORT3_IRQHandler(void){
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);

    if(status & BIT5){
        exited=1;
        GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
        return;
    }

    process_interrupt(GPIO_PORT_P3);
}

void PORT4_IRQHandler(void){
    process_interrupt(GPIO_PORT_P4);
}

void PORT5_IRQHandler(void){
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);

    if(status & BIT1){
        MAX_DEPTH+=2;
        if(MAX_DEPTH==8) MAX_DEPTH=2;

        if(current_state==STATE_CALCULATING_MOVE){
            current_state=STATE_WAITING_FOR_MOVE;
        }else{
            current_state=STATE_CALCULATING_MOVE;
        }

        GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
        return;
    }

    process_interrupt(GPIO_PORT_P5);
}

void PORT6_IRQHandler(void){
    process_interrupt(GPIO_PORT_P6);
}
