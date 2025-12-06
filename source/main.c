#include "include/connect4algorithm.h"
#include "include/servocontroller.h"
#include "include/sensorsdriver.h"
#include "include/types.h"
#include "msp.h"






void fn_INIT(void);
void fn_PLAYER_VICTORY(void);
void fn_COMPUTER_VICTORY(void);



StateMachine_t fsm[] = {
                        {STATE_INIT,fn_INIT},
                        {STATE_WAITING_FOR_MOVE,fn_WAITING_FOR_MOVE},
                        {STATE_CALCULATING_MOVE,fn_CALCULATING_MOVE},
                        {STATE_MAKING_MOVE,fn_MAKING_MOVE},
                        {STATE_PLAYER_VICTORY,fn_PLAYER_VICTORY},
                        {STATE_COMPUTER_VICTORY,fn_COMPUTER_VICTORY}

};

void fn_INIT(){

}





void fn_PLAYER_VICTORY(){

}

void fn_COMPUTER_VICTORY(){

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



