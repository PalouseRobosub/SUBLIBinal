
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

/*************************************************************************
 System Includes
 ************************************************************************/
#define PB_CLK 15000000 //15 MHz

/*************************************************************************
 Processor Configuration
 ************************************************************************/
//insert configuration for new microcontrollers
#include <xc.h>

//select programming pins
#pragma config ICESEL = ICS_PGx1// ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = OFF             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = OFF            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = OFF           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_15         // PLL Multiplier (15x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
//programming pins are selected above
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

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
    pwm_configuration.timer = timer_configuration; //Assign the timer to the PWM configuration
    
    initialize_PWM(pwm_configuration); //initialize the PWM module
    
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    //Begin the embedded system loop
    while (1) {
        
    }
    
    return 0;
}