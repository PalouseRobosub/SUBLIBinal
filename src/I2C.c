
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

//channel data structures
I2C_Data i2c1 = {0};
I2C_Data i2c2 = {0};

//i2c state machine variables
I2C_STATE i2c_1_state;
I2C_STATE i2c_2_state;

I2C_Data* initialize_I2C(I2C_Config config) {

    switch (config.channel) {
        case I2C_CH_1:
            I2C1BRG = config.pb_clk / (2 * I2C_SPEED) - 2; //calculate the proper divider

            //setup the rx and tx buffers
            i2c1.Result_queue = create_queue(config.result_buffer_ptr, config.result_buffer_size);
            i2c1.Data_queue = create_queue(config.data_buffer_ptr, config.data_buffer_size);
            i2c1.Work_queue = create_queue(config.work_buffer_ptr, config.work_buffer_size);

            i2c1.is_idle = TRUE; //set the I2C state machine to idling
            i2c_1_state = STOPPED;

            //configure interrupts
            IEC1bits.I2C1MIE = 1; //enable interrupts
            IFS1bits.I2C1MIF = 0; //turn interrupts off
            IPC8bits.I2C1IP = 7;

            I2C1CONbits.ON = 0b1; //enable the I2C module
            return &i2c1;
            break;

        case I2C_CH_2:
            I2C2BRG = config.pb_clk / (2 * I2C_SPEED)  - 2; //calculate the proper divider

            //setup the rx and tx buffers
            i2c2.Result_queue = create_queue(config.result_buffer_ptr, config.result_buffer_size);
            i2c2.Data_queue = create_queue(config.data_buffer_ptr, config.data_buffer_size);
            i2c2.Work_queue = create_queue(config.work_buffer_ptr, config.work_buffer_size);

            i2c2.is_idle = TRUE; //set the I2C state machine to idling
            i2c_2_state = STOPPED;

            //Configure interrupts
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
    
    uint interrupt_state;

    interrupt_state = __builtin_get_isr_state();
    __builtin_disable_interrupts();

    switch (channel) {
        case I2C_CH_1:

            //load the new node
            if (getAvailable(&i2c1.Work_queue) >= (sizeof(node) + node.data_size) && node.mode == WRITE)
            {
                status = enqueue(&(i2c1.Work_queue), (uint8*) & node, sizeof (node));
                status = enqueue(&i2c1.Work_queue, (uint8*) node.data_buffer, node.data_size);
            }
            else if(getAvailable(&i2c1.Work_queue) >= sizeof(node))
                status = enqueue(&(i2c1.Work_queue), (uint8*) & node, sizeof (node));
            
            //if the bus is idling, force-start it
            if (i2c1.is_idle) {
                IFS1bits.I2C1MIF = 1;
            }
            break;

        case I2C_CH_2:
            //load the new node
            status = enqueue(&(i2c2.Work_queue), (uint8*) & node, sizeof (node));

            if (node.mode == WRITE) //Enqueue data to write if required
                status = enqueue(&i2c2.Work_queue, (uint8*) node.data_buffer, node.data_size);

            //if the bus is idling, force-start it
            if (i2c2.is_idle) {
                IFS1bits.I2C2MIF = 1;
            }

            break;

        default:
            //some sort of error handling?
            status = ERR_INVALID_CHANNEL;
    }
    
    __builtin_set_isr_state(interrupt_state);

    return status;
}

//run this background process in the main while loop to
//process the results of I2C transactions

void bg_process_I2C(I2C_Channel channel, boolean loop) {
    I2C_Node current_node;
    Queue *result_queue;

    switch(channel)
    {
      case I2C_CH_1:
        result_queue = &i2c1.Result_queue;
      break;

      case I2C_CH_2:
        result_queue = &i2c2.Result_queue;
      break;
    }

    //process channel
    if(loop) //check if user wants to loop through all nodes, or just once
    {
      while (!dequeue(result_queue, (uint8*) & current_node, sizeof (current_node))) //loop until queue is empty
      {
          if (current_node.callback != NULL)
          {
              current_node.callback(current_node);
          }
      }
    }
    else
    {
      if(dequeue(result_queue, (uint8*) & current_node, sizeof (current_node)) == ERR_NO_ERR) //just get one node out
      {
        if (current_node.callback != NULL)
        {
            current_node.callback(current_node);
        }
      }
    }
}

Error get_data_I2C(I2C_Node* node, uint8* data)
{
  Error status;

  switch(node->channel)
  {
    case I2C_CH_1:
        status = dequeue(&i2c1.Data_queue, data, node->data_size);
    break;

    case I2C_CH_2:
        status = dequeue(&i2c2.Data_queue, data, node->data_size);
    break;
  }


  return status;
}

void __ISR(_I2C_1_VECTOR, IPL7AUTO) I2C_1_Handler(void) {
    static I2C_Node current_node;
    static uint8 data_index;
    uint8 byte;

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
                if (current_node.data_size)
                {
                    dequeue(&i2c1.Work_queue, &byte, sizeof(byte));
                    I2C1TRN = byte; //send first data byte
                    i2c_1_state = DATA_SENT; //move on to next state
                }
                else
                {
                    I2C1CONbits.PEN = 1;
                    i2c_1_state = STOPPED;
                }
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
                dequeue(&i2c1.Work_queue, &byte, sizeof(byte));
                I2C1TRN = byte; //start sending next bit
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
            byte = I2C1RCV;
            enqueue(&i2c1.Data_queue, &byte, sizeof(byte)); //read the received data

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
            if (enqueue(&(i2c1.Result_queue), (uint8*) & current_node, sizeof (current_node))) {
                //queue is full, error handling?
            }

            i2c_1_state = STOPPED; //move onto next state
            break;

        case STOPPED: //we have just sent the stop signal
            if (dequeue(&(i2c1.Work_queue), (uint8*) & current_node, sizeof (current_node))) //load next node from the queue
            {
                i2c1.is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
            } else {
                I2C1CONbits.SEN = 1; //send the start signal
                i2c1.is_idle = FALSE; //flag that the bus is working now
                i2c_1_state = STARTED; //move onto next state
            }
            break;
    }

    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_I2C_2_VECTOR, IPL7AUTO) I2C_2_Handler(void) {
    static I2C_Node current_node;
    static uint8 data_index;
    uint8 byte;

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
                if (current_node.data_size)
                {
                    dequeue(&i2c2.Work_queue, &byte, sizeof(byte));
                    I2C2TRN = byte; //send first data byte
                    i2c_2_state = DATA_SENT; //move on to next state
                }
                else
                {
                    I2C2CONbits.PEN = 1;
                    i2c_2_state = STOPPED;
                }
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
                dequeue(&i2c2.Work_queue, &byte, sizeof(byte));
                I2C2TRN = byte; //start sending next bit
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
            byte = I2C2RCV; //read the received data

            enqueue(&i2c2.Data_queue, &byte, sizeof(byte));

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
            if (enqueue(&(i2c2.Result_queue), (uint8*) & current_node, sizeof (current_node))) {
                //queue is full, error handling?
            }

            i2c_2_state = STOPPED; //move onto next state
            break;

        case STOPPED: //we have just sent the stop signal
            if (dequeue(&(i2c2.Work_queue), (uint8*) & current_node, sizeof (current_node))) //load next node from the queue
            {
                i2c2.is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
            } else {
                I2C2CONbits.SEN = 1; //send the start signal
                i2c2.is_idle = FALSE; //flag that the bus is working now
                i2c_2_state = STARTED; //move onto next state
            }
            break;
    }

    asm volatile ("ei"); //reenable interrupts
}
