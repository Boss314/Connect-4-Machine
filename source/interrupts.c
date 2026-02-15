#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "include/sensorsdriver.h"
#include<stdbool.h>

extern volatile bool button1_pressed;
extern volatile bool button2_pressed;

// this file contains the interrupt service routines used throughout the project
// most of these ISRs can only be activated by the sensors on the columns, so they directly call the Sensors_process_interrupt function that deals with them
// there is an exception for ports 3 and 5, which also contain the boosterpack's pushbuttons that are used in the menu, in their case we check for them and set an extra flag

void PORT2_IRQHandler(void){
    Sensors_process_interrupt(GPIO_PORT_P2);
}


void PORT3_IRQHandler(void){
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    if(status & BIT5){
        //the interrupt came from the button, we set the flag and clear the flags
        button2_pressed=true;
        GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
    }else{
        //the interrupt came from a sensor, we process it
        Sensors_process_interrupt(GPIO_PORT_P3);
    }

    Sensors_process_interrupt(GPIO_PORT_P3);
}


void PORT4_IRQHandler(void){
    Sensors_process_interrupt(GPIO_PORT_P4);
}


void PORT5_IRQHandler(void){
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    if(status & BIT1){
        //the interrupt came from the button, we set the flag and clear the flags
        button1_pressed=true;
        GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
    }else{
        //the interrupt came from a sensor, we process it
        Sensors_process_interrupt(GPIO_PORT_P5);
    }
}

void PORT6_IRQHandler(void){
    Sensors_process_interrupt(GPIO_PORT_P6);
}
