
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
#include "../System.h"
#include "../Timer.h"
#include "../UART.h"
#include "../ADC.h"
#include "../packetizer.h"

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
    UART_Config uart_config;
    Timer_Config timer_config;
    ADC_Config adc_config;

    //setup peripherals
    timer_config.divide = Div_256;
    timer_config.period = 50000;
    timer_config.which_timer = Timer_1;
    timer_config.callback = &timer_callback;
    timer_config.enabled = 1;
    initialize_TIMER(timer_config);

    uart_config.which_uart = UART_CH_1;
    uart_config.pb_clk = PB_CLK;
    uart_config.speed = 115200;
    uart_config.tx_buffer_ptr = uart_tx_buffer;
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer);
    uart_config.tx_en = 1;
    initialize_UART(uart_config);

    adc_config.channels = (1 << ADC_CH_0) | (1 << ADC_CH_1);
    adc_config.work_buffer_ptr = adc_work_queue;
    adc_config.work_buffer_size = sizeof(adc_work_queue);
    adc_config.result_buffer_ptr = adc_results_queue;
    adc_config.result_buffer_size = sizeof(adc_results_queue);
    initialize_ADC(adc_config);
    
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        bg_process_ADC();
    }






    return 0;
}



void timer_callback(void)
{
    ADC_Node node;
    node.device_id = 0x01;
    node.channel = ADC_CH_0;
    node.callback = &adc_callback;

    read_ADC(node);


	node.device_id = 0x02;
	node.channel = ADC_CH_1;
	node.callback = &adc_callback;

	read_ADC(node);
}

void adc_callback(ADC_Node node)
{
    uint8 send_data[3];
    uint8 data_LB, data_HB;

    data_LB = node.data & 0xFF;
    data_HB = node.data >> 8;

    send_data[0] = node.device_id;
    send_data[1] = data_LB;
    send_data[2] = data_HB;

    send_packet(PACKET_UART1, send_data, sizeof(send_data));
}