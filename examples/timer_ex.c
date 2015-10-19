
/********************************************************
 *   File Name: timer_ex.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "sublibinal.h"
#include "sublibinal_config.h"

//forward declarations
void timer_callback();

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //structures for configuring peripherals
    Timer_Config timer_config = {0};
    
    //Configure our output pin, PB3
    ANSELBbits.ANSB3 = 0; //digital pin
    TRISBbits.TRISB3 = 0; //output

    //setup peripherals
    timer_config.frequency = 1; //Have the timer trigger at a rate of 1Hz
    timer_config.pbclk = 15000000; //The peripheral bus clock is configured to operate at 15MHz
    timer_config.which_timer = Timer_1; //Use Timer 1
    timer_config.callback = &timer_callback; //Hand a callback function for the ISR
    timer_config.enabled = 1; //Enable the Timer
    initialize_Timer(timer_config); //Initialize the timer module
 
    //Global interrupt enable. Do this last!
	enable_Interrupts();

    while (1) {
        //background tasks here, if necessary
    }

    return 0;
}

void timer_callback(void)
{
    //toggle a pin
    PORTBINV = 1<<3;

    
}
