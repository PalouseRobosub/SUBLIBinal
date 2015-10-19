
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
#include "sublibinal_config.h"

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
    timer_config.pbclk = PB_CLK;
    timer_config.which_timer = Timer_1; //Use timer 1
    timer_config.callback = &timer_callback; //Hand a callback function for the ISR functionality
    timer_config.enabled = 1; //Enable the timer
    initialize_Timer(timer_config); //Initialize the timer module

    uart_config.which_uart = UART_CH_1; //Specify UART channel 1 for us
    uart_config.pb_clk = PB_CLK; //Tell the module the speed of our clock
    uart_config.speed = 115200; //Specify the buadrate to be 115.2k
    uart_config.rx_buffer_ptr = uart_rx_buffer; //Hand a pointer to some memory for the UART
    uart_config.rx_buffer_size = sizeof(uart_rx_buffer); //Tell the UART the size of this memory
    uart_config.rx_pin = Pin_RPB13; //set RPB13 as the rx pin for UART
	uart_config.rx_en = 1; //Enable receiving
    uart_config.tx_buffer_ptr = uart_tx_buffer; //Hand a data pointer to the transmission buffer
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer); //Tell the UART the size of the transmission buffer
 	uart_config.tx_pin = Pin_RPB15; //specify pin RPB15 for the UART tx pin
 	uart_config.tx_en = 1; //Enable transmission
    initialize_UART(uart_config); //Initialize the UART module
 
    //Global interrupt enable. Do this last!
	enable_Interrupts();

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
