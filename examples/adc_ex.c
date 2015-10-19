
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
#include "sublibinal_config.h"
#include "peripheral/ports.h"

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
    
    //disable interrupts
    disable_Interrupts();

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
    uart_config.tx_pin = Pin_RPB15;
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
    enable_Interrupts();
    
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