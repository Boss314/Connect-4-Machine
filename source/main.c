#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "include/board.h"
#include "include/displaycontroller.h"
#include "include/connect4algorithm.h"
#include "include/sensorsdriver.h"
#include "include/types.h"
#include<stdbool.h>
#include "msp.h"






void fn_INIT(void);
void fn_PLAYER_VICTORY(void);
void fn_COMPUTER_VICTORY(void);
void fn_DRAW(void);


StateMachine_t fsm[] = {
                        {STATE_INIT,fn_INIT},
//                        {STATE_START, fn_START},
                        {STATE_WAITING_FOR_MOVE,fn_WAITING_FOR_MOVE},
                        {STATE_CALCULATING_MOVE,fn_CALCULATING_MOVE},
                        {STATE_MAKING_MOVE,fn_MAKING_MOVE},
                        {STATE_PLAYER_VICTORY,fn_PLAYER_VICTORY},
                        {STATE_COMPUTER_VICTORY,fn_COMPUTER_VICTORY},
                        {STATE_DRAW, fn_DRAW}

};


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    while(1){
        if(current_state < NUM_STATES){
            (*fsm[current_state].state_function)();
        }
        else{
            /* serious error */
        }

    }
}


extern volatile bool button1_pressed; // button 1 used to change selection in the menu
extern volatile bool button2_pressed; // button 2 used to get out of the menu once a selection is made
extern volatile uint8_t MAX_DEPTH;

// the fn_INIT function handles the game's start menu, during which the difficulty and the first to play are determined
// to do this we use the boosterpack's pushbutton 1 and 2
void fn_INIT(){
    Sensors_init();
    Display_init();
    Board_init(&game_board);


    //enable the pins connected to the pushbuttons as input capable of triggering interrupts
    //button 1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);

    //button 2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);


    //enable interrputs on ports 3 and 5 in case they aren't enabled already
    Interrupt_enableInterrupt(INT_PORT3);
    Interrupt_enableInterrupt(INT_PORT5);


    // first selection: search depth used by the algorithm, button 1 makes it cycle between 2 4 6
    button2_pressed=false;
    MAX_DEPTH=2;
    while(!button2_pressed){
        //update the writing on the display
        switch(MAX_DEPTH){
            case 2: Display_printCentered("Difficulty: Easy"); break;
            case 4: Display_printCentered("Difficulty: Medium"); break;
            case 6: Display_printCentered("Difficulty: Hard"); break;
        }


        button1_pressed=false;
        //enable the button interrupts and go to sleep
        GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
        GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);
        GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
        PCM_gotoLPM0();

        // check if the interrupt what woke us up was button 1 being pressed
        if(button1_pressed){
            MAX_DEPTH+=2;
            if(MAX_DEPTH==8) MAX_DEPTH=2;
        }

        //disable the button interrupts so they dont trigger while the display is writing
        GPIO_disableInterrupt(GPIO_PORT_P5, GPIO_PIN1);
        GPIO_disableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
    }

    // second selection: determine who goes first, button 1 changes the value of the next state between STATE_WAINTING_FOR_MOVE and STATE_CALCULATING_MOVE
    State_t next_state=STATE_WAITING_FOR_MOVE;
    button2_pressed=false;
    while(!button2_pressed){
        //update the writing on the display
        if(next_state==STATE_WAITING_FOR_MOVE){
            Display_printCentered("Player goes first");
        }else{
            Display_printCentered("Computer goes first");
        }


        button1_pressed=false;
        //enable the button interrupts and go to sleep
        GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
        GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);
        GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
        PCM_gotoLPM0();

        //  check if the interrupt what woke us up was button 1 being pressed
        if(button1_pressed){
            next_state=(next_state==STATE_WAITING_FOR_MOVE) ? STATE_CALCULATING_MOVE : STATE_WAITING_FOR_MOVE;
        }

        //disable the button interrupts so they dont trigger while the display is writing
        //also to make sure they dont interfere with the sensors during the rest of the program
        GPIO_disableInterrupt(GPIO_PORT_P5, GPIO_PIN1);
        GPIO_disableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
    }


    current_state=next_state;
}

void fn_PLAYER_VICTORY(){
    Display_write();
    PCM_gotoLPM0();

}

void fn_COMPUTER_VICTORY(){
    Display_write();
    PCM_gotoLPM0();

}

void fn_DRAW(){
    Display_write();
    PCM_gotoLPM0();

}


