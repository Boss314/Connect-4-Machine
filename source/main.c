#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "include/board.h"
#include "include/displaycontroller.h"
#include "include/connect4algorithm.h"
#include "include/sensorsdriver.h"
#include "include/types.h"
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

void fn_INIT(){
    Sensors_init();
    Display_init();

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    Board_init(&game_board);

    exited=0;

    while(!exited){
        char string[]="  max depth";
        string[0]='0'+MAX_DEPTH;

        Display_printCentered(string);
        PCM_gotoLPM0();
    }

    exited=0;
    while(!exited){
        char string[]="    go first";
        if(current_state==STATE_WAITING_FOR_MOVE){
            string[0]='y';
            string[1]='o';
            string[2]='u';
        }else{
            string[0]='c';
            string[1]='o';
            string[2]='m';
        }

        Display_printCentered(string);
        PCM_gotoLPM0();
    }

    //current_state=STATE_WAITING_FOR_MOVE;
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



