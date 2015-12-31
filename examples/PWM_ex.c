
/********************************************************
 *   File Name: PWM_ex.c
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

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {
    
    Timer_Config timer_configuration = {0};
    PWM_Config pwm_configuration = {0};
    
    //configure our timer for use by the PWM module
    timer_configuration.callback = NULL; //We do not need a timer callback function
    timer_configuration.enabled = TRUE; //enable the timer when we initialize it
    timer_configuration.frequency = 1000; //Run the timer at 1 KHz frequency
    timer_configuration.pbclk = PB_CLK; //Tell the function our PB_CLK is 15MHz
    timer_configuration.which_timer = Timer_2; //Use timer 1 for this timer
    initialize_Timer(timer_configuration); //initialize and start the timer
    
    //specify parameters of the PWM module
    pwm_configuration.channel = PWM_CH_1; //Use PWM_CH_1 for this waveform
    pwm_configuration.dutyCycle = .75; //Set the duty cycle to 75%
    pwm_configuration.enable = TRUE; //enable the PWM
    pwm_configuration.pin = Pin_RPB3; //Set our PWM to output on RB3
    pwm_configuration.timer = Timer_2; //Assign the timer to the PWM configuration
    
    initialize_PWM(pwm_configuration); //initialize the PWM module
    
    //Global interrupt enable. Do this last!
	enable_Interrupts();

    //Begin the embedded system loop
    while (1) {
        
    }
    
    return 0;
}
