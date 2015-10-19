

/********************************************************
 *   File Name: ext_int_ex.c
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


void interrupt_Callback();

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {
    
    Interrupt_Config interrupt_Config = {0}; //Initialize the structure to zero
    
    //Configure an output pin
    TRISAbits.TRISA0 = 0; //Set an output pin on RA0 
    
    interrupt_Config.callback = &interrupt_Callback; //set the callback function that executes when the interrupt triggers
    interrupt_Config.enable = TRUE; //We will enable the external interrupt
    interrupt_Config.extInt = INT1; //We will use External Interrupt 1
    interrupt_Config.pin = Pin_RPB14; //Set our external interrupt pin to be RB14
    interrupt_Config.resistor = pullup; //set an internal pullup resistor
    interrupt_Config.polarity = rising; //set the interrupt to trigger when the input toggles from low to high, 
                                        //e.g. rising edge
    
    initialize_Interrupt(interrupt_Config); //initialize the interrupt
    
    enable_Interrupts();

    //Begin the embedded system loop
    while (1) {
        
    }
    
    return 0;
}

void interrupt_Callback() {
    PORTAINV = 1; //toggle bit 1 in PORTA
}