
#include "I2C.h"


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


//callback functions
void (*I2C_1_callback) (void);
void (*I2C_2_callback) (void);

//channel data structures
I2C_Data i2c1 = {0};
I2C_Data i2c2 = {0};

//i2c state machine variables
I2C_STATE i2c_1_state;
I2C_STATE i2c_2_state;

I2C_Data* initialize_I2C(I2C_Config config) {
                         //comment
    switch (config.channel) {
        case I2C_CH_1:
            I2C1BRG = config.pb_clk / (2 * I2C_SPEED) - 2; //calculate the proper divider

            //setup the rx and tx buffers
            i2c1.Rx_queue = create_queue(config.rx_buffer_ptr, config.rx_buffer_size);
            i2c1.Tx_queue = create_queue(config.tx_buffer_ptr, config.tx_buffer_size);

            I2C_1_callback = config.callback; //link the callback function

            i2c1.Tx_is_idle = TRUE; //set the I2C state machine to idling
            i2c_1_state = STOPPED;

            //WTF MPLAB
            IEC1bits.I2C1MIE = 1; //enable interrupts
            IFS1bits.I2C1MIF = 0; //turn interrupts off
            IPC8bits.I2C1IP = 7;

            I2C1CONbits.ON = 0b1; //enable the I2C module
            return &i2c1;
            break;

        case I2C_CH_2:
            I2C2BRG = config.pb_clk / (2 * I2C_SPEED)  - 2; //calculate the proper divider

            //setup the rx and tx buffers
            i2c2.Rx_queue = create_queue(config.rx_buffer_ptr, config.rx_buffer_size);
            i2c2.Tx_queue = create_queue(config.tx_buffer_ptr, config.tx_buffer_size);

            I2C_2_callback = config.callback; //link the callback function

            i2c2.Tx_is_idle = TRUE; //set the I2C state machine to idling
            i2c_2_state = STOPPED;

            //WTF MPLAB
            IEC1bits.I2C2MIE = 1; //enable interrupts
            IFS1bits.I2C2MIF = 0; //turn interrupts off
            IPC9bits.I2C2IP = 7;

            I2C2CONbits.ON = 0b1; //enable the I2C module
            return &i2c2;
            break;

        default:
            //some sort of error handling?
            break;
    }

    return NULL;
}

Error send_I2C(I2C_Channel channel, I2C_Node node) {
    Error status = ERR_NO_ERR;

    switch (channel) {
        case I2C_CH_1:
            //load the new node
            status = enqueue(&(i2c1.Tx_queue), (uint8*) & node, sizeof (node));

            //if the bus is idling, force-start it
            if (i2c1.Tx_is_idle) {
                IFS1bits.I2C1MIF = 1;
            }
            break;

        case I2C_CH_2:
            //load the new node
            status = enqueue(&(i2c2.Tx_queue), (uint8*) & node, sizeof (node));

            //if the bus is idling, force-start it
            if (i2c2.Tx_is_idle) {
                IFS1bits.I2C2MIF = 1;
            }

            break;

        default:
            //some sort of error handling?
            status = ERR_INVALID_CHANNEL;
    }

    return status;
}

//run this background process in the main while loop to
//process the results of I2C transactions

void bg_process_I2C(void) {
    I2C_Node current_node;

    //process channel 1
    while (!dequeue(&(i2c1.Rx_queue), (uint8*) & current_node, sizeof (current_node))) {
        if (current_node.callback != NULL) {
            current_node.callback(current_node);
        }
    }


    //process channel 2
    while (!dequeue(&(i2c2.Rx_queue), (uint8*) & current_node, sizeof (current_node))) {
        if (current_node.callback != NULL) {
            current_node.callback(current_node);
        }
    }
}
void __ISR(_I2C_1_VECTOR, IPL7AUTO) I2C_1_Handler(void) {
    static I2C_Node current_node;
    static uint8 data_index;

    asm volatile ("di"); //disable interrupts

    IFS1bits.I2C1MIF = 0; //clear the interrupt flag


    switch (i2c_1_state) {
        case STARTED: //start signal has been sent, now to send the device addresss + write bit
            I2C1TRN = get_write_addr(current_node.device_address); //send device address + write bit

            //initialize indexes
            data_index = 0;
            //move on to next state
            i2c_1_state = DEV_ADDR_W_SENT;
            break;

        case DEV_ADDR_W_SENT: //device addresss + write bit has been sent, now to send the sub address
            I2C1TRN = current_node.sub_address; //send sub address byte
            i2c_1_state = SUB_ADDR_SENT; //move to next state
            break;

        case SUB_ADDR_SENT: //sub address has been sent, need logic to determine next step
            if (current_node.mode == READ) //if we want to read
            {
                I2C1CONbits.RSEN = 1; //send restart signal
                i2c_1_state = RESTARTED; //move onto next state
            } else //if we want to write
            {
                I2C1TRN = current_node.data_buffer[data_index]; //send first data byte
                i2c_1_state = DATA_SENT; //move on to next state
            }
            break;

        case DATA_SENT: //we have just sent a data byte
            ++data_index;

            if (data_index == current_node.data_size) //if we have sent all data bytes
            {
                I2C1CONbits.PEN = 1; //send the stop signal
                i2c_1_state = STOPPED; //move onto next state
            } else //if we have more bytes to send
            {
                I2C1TRN = current_node.data_buffer[data_index]; //start sending next bit
            }
            break;

        case RESTARTED: //restart signal has been sent, now to send device address + read bit
            I2C1TRN = get_read_addr(current_node.device_address); //send device address + read bit
            i2c_1_state = DEV_ADDR_R_SENT; //move onto next state
            break;

        case DEV_ADDR_R_SENT: //device address + read bit has been sent, get ready to receive byte(s)
            I2C1CONbits.RCEN = 1; //enable receiver mode
            i2c_1_state = DATA_RECEIVED; //move onto next state
            break;

        case DATA_RECEIVED: //we have just received a byte
            current_node.data_buffer[data_index] = I2C1RCV; //read the received data

            ++data_index;
            if (data_index == current_node.data_size) //if we have read all the data we want
            {
                I2C1CONbits.ACKDT = 1; //configure to send NACK signal
                i2c_1_state = NOACK_SENT; //move onto next state
            } else //we want to read another byte
            {
                I2C1CONbits.ACKDT = 0; //configure to send ACK signal
                i2c_1_state = ACK_SENT; //move onto next state
            }

            I2C1CONbits.ACKEN = 1; //send the ACK/NACK signal

            break;

        case ACK_SENT: //we have sent an ACK signal, prepare to receive another byte
            I2C1CONbits.RCEN = 1; //enable the receiver mode
            i2c_1_state = DATA_RECEIVED; //move onto next state
            break;

        case NOACK_SENT: //we have sent an NACK signal, prepare to stop transaction
            I2C1CONbits.PEN = 1; //send the stop signal

            //put node in receive buffer
            if (enqueue(&(i2c1.Rx_queue), (uint8*) & current_node, sizeof (current_node))) {
                //queue is full, error handling?
            }

            i2c_1_state = STOPPED; //move onto next state
            break;

        case STOPPED: //we have just sent the stop signal
            if (dequeue(&(i2c1.Tx_queue), (uint8*) & current_node, sizeof (current_node))) //load next node from the queue
            {
                i2c1.Tx_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
            } else {
                I2C1CONbits.SEN = 1; //send the start signal
                i2c1.Tx_is_idle = FALSE; //flag that the bus is working now
                i2c_1_state = STARTED; //move onto next state
            }
            break;
    }

    if (I2C_1_callback != NULL) {
        I2C_1_callback();
    }

    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_I2C_2_VECTOR, IPL7AUTO) I2C_2_Handler(void) {
    static I2C_Node current_node;
    static uint8 data_index;

    asm volatile ("di"); //disable interrupts

    IFS1bits.I2C2MIF = 0; //clear the interrupt flag


    switch (i2c_2_state) {
        case STARTED: //start signal has been sent, now to send the device addresss + write bit
            I2C2TRN = get_write_addr(current_node.device_address); //send device address + write bit

            //initialize indexes
            data_index = 0;
            //move on to next state
            i2c_2_state = DEV_ADDR_W_SENT;
            break;

        case DEV_ADDR_W_SENT: //device addresss + write bit has been sent, now to send the sub address
            I2C2TRN = current_node.sub_address; //send sub address byte
            i2c_2_state = SUB_ADDR_SENT; //move to next state
            break;

        case SUB_ADDR_SENT: //sub address has been sent, need logic to determine next step
            if (current_node.mode == READ) //if we want to read
            {
                I2C2CONbits.RSEN = 1; //send restart signal
                i2c_2_state = RESTARTED; //move onto next state
            } else //if we want to write
            {
                I2C2TRN = current_node.data_buffer[data_index]; //send first data byte
                i2c_2_state = DATA_SENT; //move on to next state
            }
            break;

        case DATA_SENT: //we have just sent a data byte
            ++data_index;

            if (data_index == current_node.data_size) //if we have sent all data bytes
            {
                I2C2CONbits.PEN = 1; //send the stop signal
                i2c_2_state = STOPPED; //move onto next state
            } else //if we have more bytes to send
            {
                I2C2TRN = current_node.data_buffer[data_index]; //start sending next bit
            }
            break;

        case RESTARTED: //restart signal has been sent, now to send device address + read bit
            I2C2TRN = get_read_addr(current_node.device_address); //send device address + read bit
            i2c_2_state = DEV_ADDR_R_SENT; //move onto next state
            break;

        case DEV_ADDR_R_SENT: //device address + read bit has been sent, get ready to receive byte(s)
            I2C2CONbits.RCEN = 1; //enable receiver mode
            i2c_2_state = DATA_RECEIVED; //move onto next state
            break;

        case DATA_RECEIVED: //we have just received a byte
            current_node.data_buffer[data_index] = I2C2RCV; //read the received data

            ++data_index;
            if (data_index == current_node.data_size) //if we have read all the data we want
            {
                I2C2CONbits.ACKDT = 1; //configure to send NACK signal
                i2c_2_state = NOACK_SENT; //move onto next state
            } else //we want to read another byte
            {
                I2C2CONbits.ACKDT = 0; //configure to send ACK signal
                i2c_2_state = ACK_SENT; //move onto next state
            }

            I2C2CONbits.ACKEN = 1; //send the ACK/NACK signal

            break;

        case ACK_SENT: //we have sent an ACK signal, prepare to receive another byte
            I2C2CONbits.RCEN = 1; //enable the receiver mode
            i2c_2_state = DATA_RECEIVED; //move onto next state
            break;

        case NOACK_SENT: //we have sent an NACK signal, prepare to stop transaction
            I2C2CONbits.PEN = 1; //send the stop signal

            //put node in receive buffer
            if (enqueue(&(i2c2.Rx_queue), (uint8*) & current_node, sizeof (current_node))) {
                //queue is full, error handling?
            }

            i2c_2_state = STOPPED; //move onto next state
            break;

        case STOPPED: //we have just sent the stop signal
            if (dequeue(&(i2c2.Tx_queue), (uint8*) & current_node, sizeof (current_node))) //load next node from the queue
            {
                i2c2.Tx_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
            } else {
                I2C2CONbits.SEN = 1; //send the start signal
                i2c2.Tx_is_idle = FALSE; //flag that the bus is working now
                i2c_2_state = STARTED; //move onto next state
            }
            break;
    }

    if (I2C_2_callback != NULL) {
        I2C_2_callback();
    }

    asm volatile ("ei"); //reenable interrupts
}
