
/********************************************************
 *   File Name: uart_ex.c
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


//forward declarations
void timer_callback(void);

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //buffer for uart ISRs
    uint8 uart_tx_buffer[128], uart_rx_buffer[128];

    //structures for configuring peripherals
    UART_Config uart_config = {0};
    Timer_Config timer_config = {0};
    
    //temp buffer
    uint8 blah;

    //setup peripherals
    timer_config.frequency = 1000; //Set the timer to operate at 1KHz
    timer_config.which_timer = Timer_1; //Use timer 1
    timer_config.callback = &timer_callback; //Hand a callback function for the ISR functionality
    timer_config.enabled = 1; //Enable the timer
    initialize_Timer(timer_config); //Initialize the timer module

    uart_config.which_uart = UART_CH_1; //Specify UART channel 1 for us
    uart_config.pb_clk = PB_CLK; //Tell the module the speed of our clock
    uart_config.speed = 115200; //Specify the buadrate to be 115.2k
    uart_config.rx_buffer_ptr = uart_rx_buffer; //Hand a pointer to some memory for the UART
    uart_config.rx_buffer_size = sizeof(uart_rx_buffer); //Tell the UART the size of this memory
    uart_config.rx_en = 1; //Enable receiving
    uart_config.tx_buffer_ptr = uart_tx_buffer; //Hand a data pointer to the transmission buffer
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer); //Tell the UART the size of the transmission buffer
    uart_config.tx_en = 1; //Enable transmission
    initialize_UART(uart_config); //Initialize the UART module
 
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        //one byte is put in the 'blah' buffer by calling receive_UART
        if (receive_UART(UART_CH_1, &blah, sizeof(blah)) == ERR_NO_ERR) //if we successfully pulled out some data
        {
            //do something with the data
            send_UART(UART_CH_1, &blah, sizeof(blah));
        }
    }

    return 0;
}

void timer_callback(void)
{
    uint8 data[3];

    data[0] = 'a';
    data[1] = 'b';
    data[2] = 'c';

    //send some uart data
    send_UART(UART_CH_1, data, sizeof(data));

}