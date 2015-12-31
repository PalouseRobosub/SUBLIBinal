#include "sublibinal.h"
#include "sublibinal_config.h"

void comparator_callback();

int main()
{
	ANSELA = 0;
	ANSELB = 0;

	Comparator_Config comparatorConfig = {0};			//Initialize configuration structure to 0
	comparatorConfig.callback = &comparator_callback;	//Link the callback function
	comparatorConfig.channel = COMP_CH_1;				//Utilize comparator 1
	comparatorConfig.edge = rising;						//Search for rising edges
	comparatorConfig.enable = TRUE;						//Start the comparator on initialization
	comparatorConfig.input = Pin_RPB2;					//Utilize Pin RPB2 as our time-varying input
	comparatorConfig.voltage_reference = 0.5; 			//Use a voltage reference of 0.5V

	initialize_Comparator(comparatorConfig);			//Initialize the comparator

	TRISB &= ~(1<<13); 									//Enable output on RPB13
	PORTB &= ~(1<<13);									//Initialize output to 0

	enable_Interrupts();								//Global interrupt enable

	while (1);											//Embedded System loop
}

void comparator_callback()
{
	PORTBINV |= 1<<13;									//Toggle the status of Pin RPB13
}
