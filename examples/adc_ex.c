
/********************************************************
 *   File Name: adc_ex.c
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
void adc_callback(ADC_Node node);

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //buffers
    uint8 uart_tx_buffer[128];
    uint8 adc_work_queue[10*sizeof(ADC_Data)], adc_results_queue[10*sizeof(ADC_Data)];

    //structures for configuring peripherals
    UART_Config uart_config = {0};
    Timer_Config timer_config = {0};
    ADC_Config adc_config = {0};
    Packetizer_Config packetizer_config = {0};
    
    //peripheral pin select for UART
    ANSELA = ANSELB = 0;
    RPB15R = 1; //RPB15 is U1TX
    TRISBbits.TRISB13 = 1;
    U1RXR = 0b11; //RPB13 is U1RX

    //setup peripherals
    timer_config.frequency = 10; //Set the timer to operate at 10Hz
    timer_config.pbclk = PB_CLK;
    timer_config.which_timer = Timer_1; //Use timer 1
    timer_config.callback = &timer_callback; //Hand a callback function for the ISR functionality
    timer_config.enabled = 1; //Enable the timer
    initialize_Timer(timer_config); //Initialize the timer module

    uart_config.which_uart = UART_CH_1; //Specify UART channel 1 for us
    uart_config.pb_clk = PB_CLK; //Tell the module the speed of our clock
    uart_config.speed = 115200; //Specify the buadrate to be 115.2k
    uart_config.tx_buffer_ptr = uart_tx_buffer; //Hand a data pointer to the transmission buffer
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer); //Tell the UART the size of the transmission buffer
    uart_config.tx_en = 1; //Enable transmission
    initialize_UART(uart_config); //Initialize the UART module
    
    packetizer_config.callback = NULL; //Specify that there is no callback function for the packetizer
    packetizer_config.control_byte = '!'; //Specify that the ! symbol is our control byte for the packet
    packetizer_config.uart_config = uart_config; //Hand the packetizer our UART configuration that the packetizer will use
    packetizer_config.which_channel = PACKET_UART_CH_1; //Specify that this will use UART Channel 1 of rhte packetizer
    initialize_packetizer(packetizer_config); //Initialize the packetizer moudule
    
    adc_config.channels = (1 << ADC_CH_0) | (1 << ADC_CH_1); //Specify that we are using ADC channels 0 and 1
    adc_config.work_buffer_ptr = adc_work_queue; //Hand a pointer for the work data
    adc_config.work_buffer_size = sizeof(adc_work_queue); //Tell the ADC the size of the work data
    adc_config.result_buffer_ptr = adc_results_queue; //Hand a data pointer for the results data 
    adc_config.result_buffer_size = sizeof(adc_results_queue); //Tell the ADC the size of the results buffer
    initialize_ADC(adc_config); //Initialize the ADC module
    
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        bg_process_ADC(); //This process must be in the embedded loop for processing of completed data
    }






    return 0;
}



void timer_callback(void)
{
    //Conduct a read of an ADC node
    ADC_Node node = {0};
    node.device_id = 0x01; //Specify an arbitrary ID for this device. We will use this for identification purposes later
    node.channel = ADC_CH_0; //Specify which channel this device is on
    node.callback = &adc_callback; //Specify a callback for when the data is completed

    read_ADC(node); //Read the ADC node


	node.device_id = 0x02; //Specify another arbitrary ID for this other device
	node.channel = ADC_CH_1; //Specify that this will be read from channel 1
	node.callback = &adc_callback; //Hand a callback function for when the data is completed

	read_ADC(node); //Conduct a read of this different ADC node
}

//This function is called during the bg_process_ADC() function
//The purpose of this function is to do something meaningful with the data
void adc_callback(ADC_Node node)
{
    uint8 send_data[3];
    uint8 data_LB, data_HB;

    //We will break apart the data in the node into a series of bytes
    data_LB = node.data & 0xFF;
    data_HB = node.data >> 8;

    //We will populate an array with data for transmission
    send_data[0] = node.device_id;
    send_data[1] = data_LB;
    send_data[2] = data_HB;

    //Send a packet of information up the Packetizer
    send_packet(PACKET_UART_CH_1, send_data, sizeof(send_data));
}