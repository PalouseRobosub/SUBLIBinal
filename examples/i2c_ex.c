
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
#include "sublibinal_config.h"

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
    timer_config.pbclk = PB_CLK; //Specify the speed of the peripheral bus clock
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
    uart_config.tx_pin = Pin_RPB15; //select the TX pin

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
	enable_Interrupts();

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
