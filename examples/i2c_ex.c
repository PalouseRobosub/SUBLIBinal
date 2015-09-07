
/********************************************************
 *   File Name: i2c_ex.c
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
void i2c_callback(I2C_Node node);

//global variables
uint8 sensor_data[6]; //buffer for sensor data to be stored

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //buffer for uart ISRs
    uint8 uart_tx_buffer[128], uart_rx_buffer[128];
    uint8 i2c_tx_buffer[128], i2c_rx_buffer[128];

    //structures for configuring peripherals
    UART_Config uart_config = {0};
    Timer_Config timer_config = {0};
    Packetizer_Config packet_config = {0};
    I2C_Config i2c_config = {0};
    
    //temp buffer
    uint8 blah;

    //setup peripherals
    timer_config.frequency = 1000; //Set the timer to 1KHz
    timer_config.which_timer = Timer_1; //Specify timer 1 as our selected device
    timer_config.callback = &timer_callback; //Specify the timer callback function
    timer_config.enabled = 1; //enable the timer
    initialize_Timer(timer_config); //Initialize the timer with our specifications

    uart_config.which_uart = UART_CH_1; //Specify uart channel 1 for our use
    uart_config.pb_clk = PB_CLK; //tell the module the clock speed
    uart_config.speed = 115200; //inform the module of the desired baud rate
    uart_config.tx_buffer_ptr = uart_tx_buffer; //Provide a pointer to a data buffer
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer); //provide the size of the data buffer
    uart_config.tx_en = 1; //enable the uart
    initialize_UART(uart_config); //initialize the uart module

    packet_config.control_byte = 0x0A; //set the control byte for the packetizer
    packet_config.which_channel = PACKET_UART_CH_1; //specify use of UART channel 1 for the packetizer
    packet_config.uart_config = uart_config; //specify the UART configuration to reference
    packet_config.callback = NULL; //Set there to be no callback
    initialize_packetizer(packet_config); //Initialize the packetizer module
    
    i2c_config.callback = NULL; //No I2C callback function
    i2c_config.channel = I2C_CH_1; //Using I2C channel 1
    i2c_config.pb_clk = PB_CLK; //specify the peripheral bus clock
    i2c_config.rx_buffer_ptr = i2c_rx_buffer; //provide a pointer to rx buffer
    i2c_config.rx_buffer_size = sizeof(i2c_rx_buffer); //provide size of rx buffer
    i2c_config.tx_buffer_ptr = i2c_tx_buffer; //provide a pointer to the tx buffer
    i2c_config.tx_buffer_size = sizeof(i2c_tx_buffer); //provide the size of the tx buffer
    initialize_I2C(i2c_config); //initialize the I2C
 
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        //put background processes here
        bg_process_I2C();
    }

    return 0;
}

void timer_callback(void)
{
    I2C_Node node = {0};

    node.device_id = 0x01; //arbitrary-ish device ID for clarification of data results
    node.device_address = 0x45; //address of the sensor that we are talking to
    node.sub_address = 0x12; //sub address on sensor to read in the sensors memory
    node.data_buffer = sensor_data; //provide a buffer to some data for the sensor, data must remain in scope!!!
    node.data_size = 6; //reading 6 bytes from the sensor
    node.mode = READ; //we are reading and not writing
    node.callback = &i2c_callback; //provide a callback for the data

    send_I2C(I2C_CH_1, node); //enqueue a read of the device on the I2C
}

void i2c_callback(I2C_Node node)
{
    uint8 send_data[7];
    uint8 i;

    //form data to go in the packet
    send_data[0] = node.device_id;
    for(i = 1; i < 7; ++i)
    {
        send_data[i] = node.data_buffer[i-1];
    }    

    //send the packet
    send_packet(PACKET_UART_CH_1, send_data, sizeof(send_data));
}