
/********************************************************
 *   File Name: spi_ex.c
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
    uint8 spi_tx_buffer[128];

    //structures for configuring peripherals
    SPI_Config spi_config = {0};
    Timer_Config timer_config = {0};

    //setup peripherals
    timer_config.which_timer = Timer_1; //Specify that we will be using timer 1
    timer_config.frequency = 1000; //Specify that the timer operates at 1KHz
    timer_config.pbclk = PB_CLK; //specify the speed of the peripheral bus clock
    timer_config.callback = &timer_callback; //Link a callback function for the ISR
    timer_config.enabled = 1; //Enable the Timer
    initialize_Timer(timer_config); //Initialize the Timer

    spi_config.which_spi = SPI_CH_1; //Specify that we will use SPI Channel 1
    spi_config.pb_clk = PB_CLK; //Tell the module the speed of our clock, 15MHz
    spi_config.speed = 100000; //Tell SPI module the frequency that we are talking at, 100KHz
    spi_config.tx_en = 1; //Enable the SPI module
    spi_config.clk_edge = falling; //Specify that SPI transition from a high to low transition
    spi_config.tx_buffer_ptr = spi_tx_buffer; //Hand a pointer to the data buffer
    spi_config.tx_buffer_size = sizeof(spi_tx_buffer); //tell it the size of the data buffer
    initialize_SPI(spi_config); //Initialize the SPI module
 
    //Global interrupt enable. Do this last!
	enable_Interrupts();

    while (1) {
        //put background tasks here
    }

    return 0;
}

void timer_callback(void)
{
    uint8 data[3];

    data[0] = 'a';
    data[1] = 'b';
    data[2] = 'c';

    //send some SPI data
    send_SPI(SPI_CH_1, data, sizeof(data));
    
    
}
