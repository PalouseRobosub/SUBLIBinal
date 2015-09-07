

/********************************************************
 *   File Name: packetizer.c
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
void packetizer_callback(uint8* data, uint8 size);

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //buffer for uart ISRs
    uint8 uart_tx_buffer[128], uart_rx_buffer[128];

    //structures for configuring peripherals
    
    //Note, it is important to initialize these structures to 0 before use. If not, the variables can be populated with random numbers
    //if the callback is not set and left as a random number, the program will crash as it will be dereferencing a pointer that does not
    //point to a valid function.
    
    UART_Config uart_config = {0};
    Timer_Config timer_config = {0};
    Packetizer_Config packetizer_config = {0};
    
    //temp buffer
    uint8 blah;

    //setup timer peripheral
    timer_config.frequency = 1000; //We will set our timer to fire at 1 KHz
    timer_config.which_timer = Timer_1; //We will use timer_1, a type A timer
    timer_config.callback = &timer_callback; //we will set the callback to the timer_callback function
                                             //Any time the timer triggers an interrupt, this function will be called.
    timer_config.enabled = 1; //Enable the timer on initialization
    initialize_Timer(timer_config); //initialize the timer and begin its execution

    //Configure the UART peripheral
    uart_config.which_uart = UART_CH_1; //select the first UART channel for us
    uart_config.pb_clk = PB_CLK; //tell the program how fast our peripheral bush clock is running
    uart_config.speed = 115200; //provide a baud rate that you wish the microcontroller to transmit and receive at
    uart_config.rx_buffer_ptr = uart_rx_buffer; //hand the uart a buffer of data to use for a queue
    uart_config.rx_buffer_size = sizeof(uart_rx_buffer); //tell the program the size of our data buffer
    uart_config.rx_en = 1; //enable the receive buffer
    uart_config.tx_buffer_ptr = uart_tx_buffer; //hand the uart configuration a buffer for transmission data
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer); //tell the program the size of the buffer
    uart_config.tx_en = 1; //enable transmission on the UART
    initialize_UART(uart_config);//initialize all of our UART parameters and begin functionality of the UART as per our specifications
    
    
    //Configure the packetizer
    packetizer_config.callback = &packetizer_callback; //There will be no callback associated with the packetizer. It is unnecessary
    packetizer_config.control_byte = '!'; //We will use the exclamation point character as our control byte.
    packetizer_config.uart_config = uart_config; //we must hand the packetizer the uart configuration for proper set up
    packetizer_config.which_channel = PACKET_UART_CH_1; //our packetizer is running on uart channel 1
    initialize_packetizer(packetizer_config); //initialize the packetizer
    
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        bg_process_packetizer();
    }

    return 0;
}


void packetizer_callback(uint8 * data, uint8 size) {
    //packetizer callback
    
    //simply echo the packet back across the UART communication channel as a heartbeat
    send_packet(PACKET_UART_CH_1, data, size);
    
}
void timer_callback(void)
{
    uint8 data[3];

    data[0] = 'a';
    data[1] = 'b';
    data[2] = 'c';

    //send a data packet
    //The first byte will be the control byte
    //The second byte will be the size of the data being sent in the packet
    //The remaining bytes will be the rest of the data that you are sending; in this case, 'a','b','c'
    send_packet(PACKET_UART_CH_1, data, sizeof(data));
}