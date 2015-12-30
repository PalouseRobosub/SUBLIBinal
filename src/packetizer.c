#include "packetizer.h"
#include "UART.h"
#include "Timer.h"


//Copyright 2015 Palouse RoboSub Club

/*
  This file is part of Sublibinal.

    Sublibinal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Sublibinal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Sublibinal.  If not, see <http://www.gnu.org/licenses/>.
 */


Packetizer_Data UART1_channel;
Packetizer_Data UART2_channel;

void initialize_packetizer(Packetizer_Config config) {

    switch (config.which_channel) {
        case PACKET_UART_CH_1:
            UART1_channel.control_byte = config.control_byte;
            UART1_channel.receive_callback = config.callback;
            initialize_UART(config.uart_config);
            break;
        case PACKET_UART_CH_2:
            UART2_channel.control_byte = config.control_byte;
            UART2_channel.receive_callback = config.callback;
            initialize_UART(config.uart_config);
            break;
    }
}

void send_packet(Data_Channel which_channel, uint8* data, uint8 data_size) {
    uint interrupt_state;
    Error ret;
    interrupt_state = __builtin_get_isr_state();
    __builtin_disable_interrupts();

    switch (which_channel) {
        case PACKET_UART_CH_1:
            send_UART(UART_CH_1, &(UART1_channel.control_byte), 1);
            send_UART(UART_CH_1, &(data_size), 1);
            ret = send_UART(UART_CH_1, data, data_size);
            break;
        case PACKET_UART_CH_2:
            send_UART(UART_CH_2, &(UART2_channel.control_byte), 1);
            send_UART(UART_CH_2, &(data_size), 1);
            ret = send_UART(UART_CH_2, data, data_size);
            break;
    }

    if (ret) {
            //do stuff
            //this means the queue is full and the packet is broken.
            //We will lose sync
    }

    __builtin_set_isr_state(interrupt_state);
}

void bg_process_packetizer(Data_Channel which_channel) {
    uint8 current_byte;
    uint8 status;

    void (*receive_callback) (uint8* data, uint8 data_size); //receive callback function for the channel
    uint8* control_byte; //control byte for the channel
    boolean* sync_lock;
    boolean* packet_received;
    uint8* received_bytes;
    uint16* received_index;
    uint8* packet_length;

    //load data from queues
    //form packets by getting data from the queue
    //if a packet is sucessfully formed, do the callback
    //packetizer_callback(packet);



    //get one byte and setup the variables
    //this is the only thing that is device-specific
    switch (which_channel) {
        case PACKET_UART_CH_1:
            status = receive_UART(UART_CH_1, &current_byte, 1);
            receive_callback = UART1_channel.receive_callback;
            control_byte = &UART1_channel.control_byte;
            sync_lock = &UART1_channel.sync_lock;
            packet_received = &UART1_channel.packet_received;
            received_bytes = UART1_channel.received_bytes; //array, so already pointer
            received_index = &UART1_channel.received_index;
            packet_length = &UART1_channel.packet_length;
            break;
        case PACKET_UART_CH_2:
            status = receive_UART(UART_CH_2, &current_byte, 1);
            receive_callback = UART2_channel.receive_callback;
            control_byte = &UART2_channel.control_byte;
            sync_lock = &UART2_channel.sync_lock;
            packet_received = &UART2_channel.packet_received;
            received_bytes = UART2_channel.received_bytes; //array, so already pointer
            received_index = &UART2_channel.received_index;
            packet_length = &UART2_channel.packet_length;
            break;
    }

    if (status == 0) //if there were no errors we got a byte out of the buffer
    {
        if (*sync_lock) {
            switch (*received_index) {
                case 0: //first byte, should be control byte
                    *packet_received = FALSE;
                    ++(*received_index);
                    if (current_byte != *control_byte) { //if we lost sync
                        *sync_lock = FALSE; //we fucked up
                        *received_index = 0; //restart
                    }
                    break;
                case 1: //second byte, length byte
                    *packet_length = current_byte;
                    if (*packet_length == 0)
                    {
                        *packet_received = TRUE;
                        *received_index = 0;
                    }
                    else
                        ++(*received_index);
                    break;

                default: //any other bytes are data bytes
                    received_bytes[(*received_index) - 2] = current_byte; //store the received data
                    ++(*received_index);
                    if ((*received_index)-2 == *packet_length) //we've received the entire packet
                    {
                        *packet_received = TRUE;
                        *received_index = 0;
                    }
                    break;
            }

        } else { //we're out of sync, need to regain sync
            switch (*received_index) {
                case 0:
                    if (current_byte == *control_byte) {
                        *received_index = 1;
                    }
                    break;
                case 1:
                    *packet_length = current_byte;
                    ++(*received_index);
                    break;
                default:
                    //absorb bytes until we have received the entire packet
                    //receive one more, check for control byte
                    //if control byte, then say we are in sync and prep to receive rest of packet
                    ++(*received_index);
                    if (*received_index == *packet_length+3) //maybe length+3?
                    {
                        if (current_byte == *control_byte) //we appear to be in sync
                        {
                            *sync_lock = TRUE;
                            *received_index = 1;
                        }
                    }
                    break;
            }
        }

        if (*packet_received) //if we've receive a packet, process it!
        {
            receive_callback(received_bytes, *packet_length);
        }


    } //else, some error, don't do anything

}

/////////////////////////////////
//Packetizer AutoSync Functions//
/////////////////////////////////

volatile char sync = 0;
volatile char null_received = 0;
uint pb_clk = 0;
Data_Channel pchannel = 0;

void syncCallback()
{
	if (null_received) 
	{
		null_received = 0;
		update_frequency_Timer(Timer_1, pb_clk, 50);
	}
	send_packet(pchannel, NULL, 0);
}

void packetizerCallback(uint8 *data, uint8 size)
{
	char tmp = 1;
	//check to see if we received the NULL packet
	if (size == 0 && !null_received)
	{
		//NULL packet received
		TMR1 = 0;
		null_received = 1;
		send_packet(pchannel, &tmp, 1);
	}
    else if (size == 1 && null_received)    
    {
        if (data[0] == 1)
            sync = 1;
    }
}

void acquireSync(Data_Channel channel, uint pbclk)
{
    pb_clk = pbclk;
	//This function will temporarily commandeer a timer for its use
	//backup Timer_1 configurations
	int tmpT1CON = T1CON, tmpPR1 = PR1;
	void *tmpCallback = getSyncCallback();
	void *tmpPacketizerCallback;
	//Disable all other interrupts
	int tmpIEC0 = IEC0, tmpIEC1 = IEC1;

	IEC0 = 0;
	IEC1 = 0;

	//Configure Timer_1
	Timer_Config t1 = {0};
	t1.callback = &syncCallback;
    t1.which_timer = Timer_1;
	t1.enabled = TRUE;
	t1.frequency = 5;
	t1.pbclk = pbclk;
	initialize_Timer(t1);

	//Commandeer the packetizer as well
	if (channel == PACKET_UART_CH_1)
	{
		tmpPacketizerCallback = UART1_channel.receive_callback;
		UART1_channel.receive_callback = &packetizerCallback;
		IEC1bits.U1TXIE = 1;
		IEC1bits.U1RXIE = 1;
	}
	else
	{
		tmpPacketizerCallback = UART2_channel.receive_callback;
		UART2_channel.receive_callback = &packetizerCallback;
		IEC1bits.U2TXIE = 1;
		IEC1bits.U2RXIE = 1;
	}

	pchannel = channel;

	//Force enable interrupts if they are not enabled
	enable_Interrupts();

	//This function will fire a NULL packet at 50Hz until it receives a responding NULL packet. 
	//When it receives a responding NULL, it will send a packet with the only data as a char 1. 
	//It will then wait for 1 second to receive the response. 
	//If it does not receive the response, it will go back to transmitting NULL packets and repeat the cycle until sync is acquired


	//Wait to acquire synchronization	
	while (!sync)
		bg_process_packetizer(channel);

	//Reset all commandeered modules
	disable_Interrupts();
	
	//Reset interrupts
	IEC0 = tmpIEC0;
	IEC1 = tmpIEC1;

	//Reset the timer parameters
	T1CON = tmpT1CON;
	PR1 = tmpPR1;
	setSyncCallback(tmpCallback);
	
	//Reset the packetizer parameters
	if (channel == PACKET_UART_CH_1)
		UART1_channel.receive_callback = tmpPacketizerCallback;
	else
		UART2_channel.receive_callback = tmpPacketizerCallback;

	//reenable interrupts
	enable_Interrupts();
	
}
