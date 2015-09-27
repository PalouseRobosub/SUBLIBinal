
#include "UART.h"


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
void (*uart_1_tx_callback) (void);
void (*uart_1_rx_callback) (void);

void (*uart_2_tx_callback) (void);
void (*uart_2_rx_callback) (void);

UART_Data u1;
UART_Data u2;

UART_Data* initialize_UART(UART_Config config) {

    switch (config.which_uart) {
        case UART_CH_1:
            U1BRG = config.pb_clk / (16 * config.speed) - 1; //calculate the proper baud rate

            U1MODEbits.PDSEL = 0; //parity and data size selection bits (no parity, 8bit)

            //setup the rx and tx buffers
            u1.Rx_queue = create_queue(config.rx_buffer_ptr, config.rx_buffer_size);
            u1.Tx_queue = create_queue(config.tx_buffer_ptr, config.tx_buffer_size);

            IEC1bits.U1TXIE = ((config.tx_en) ? 1 : 0); //enable or disable the rx/tx interrupts
            IEC1bits.U1RXIE = ((config.rx_en) ? 1 : 0);
            IPC8bits.U1IP = 7; //set interrupt priority to 7

            U1STAbits.UTXISEL = 2; //set tx interrupt to fire when the tx buffer is empty
            U1STAbits.URXISEL = 0; //set rx interrupt to fire whenever a new byte is received

            U1STAbits.UTXEN = ((config.tx_en) ? 1 : 0); //enable or disable the rx/tx modules
            U1STAbits.URXEN = ((config.rx_en) ? 1 : 0); //enable or disable the rx/tx modules

            U1MODEbits.ON = 1; //enable the UART

            uart_1_tx_callback = config.tx_callback; //link the callback functions
            uart_1_rx_callback = config.rx_callback;

            u1.Tx_is_idle = TRUE;
            
            //set the peripheral pin select
            if (config.rx_en)
            {
                switch (config.rx_pin) {
                    case Pin_RPA2:
                        TRISAbits.TRISA2 = 1; //input
                        U1RXR = 0;
                        break;
                    case Pin_RPB6:
                        TRISB |= 1<<6; //input
                        U1RXR = 1;
                        break;
                    case Pin_RPA4:
                        TRISAbits.TRISA4 = 1; //input
                        U1RXR = 2;
                        break;
                    case Pin_RPB13:
                        TRISBbits.TRISB13 = 1; //input
                        ANSELBbits.ANSB13 = 0; //disable analog
                        U1RXR = 3;
                        break;
                    case Pin_RPB2:
                        TRISBbits.TRISB2 = 1// input
                        ANSELBbits.ANSB2 = 0; //disable analog
                        U1RXR = 4;
                        break;
                    default:
                        //Error
                        break;
                        
                           
                }
            }
            
            if (config.tx_en)
            {
                switch (config.tx_pin) {
                    case Pin_RPA0:
                        RPA0R = 1; //U1TX
                        TRISAbits.TRISA0 = 0; //output
                        break;
                    case Pin_RPB3:
                        RPB3R = 1; //U1TX
                        TRISBbits.TRISB3 = 0; //output
                        break;
                    case Pin_RPB4:
                        RPB4R = 1; //U1TX
                        TRISBbits.TRISB4 = 0; //output
                        break;
                    case Pin_RPB15:
                        RPB15R = 1; //U1TX
                        TRISBbits.TRISB15 = 0; //output
                        break;
                    case Pin_RPB7:
                        RPB7R = 1; //U1TX
                        TRISBbits.TRISB7 = 0; //output
                        break;
                    default:
                        //error
                        break;
                }
            }

            return &u1;
            break;

        case UART_CH_2:
            U2BRG = config.pb_clk / (16 * config.speed) - 1; //calculate the proper baud rate

            U2MODEbits.PDSEL = 0; //parity and data size selection bits (no parity, 8bit)

            //setup the rx and tx buffers
            u2.Rx_queue = create_queue(config.rx_buffer_ptr, config.rx_buffer_size);
            u2.Tx_queue = create_queue(config.tx_buffer_ptr, config.tx_buffer_size);

            IEC1bits.U2TXIE = ((config.tx_en) ? 1 : 0); //enable or disable the rx/tx interrupts
            IEC1bits.U2RXIE = ((config.rx_en) ? 1 : 0);
            IPC9bits.U2IP = 7; //set interrupt priority to 7

            U2STAbits.UTXISEL = 2; //set tx interrupt to fire when the tx buffer is empty
            U2STAbits.URXISEL = 0; //set rx interrupt to fire whenever a new byte is received

            U2STAbits.UTXEN = ((config.tx_en) ? 1 : 0); //enable or disable the rx/tx modules
            U2STAbits.URXEN = ((config.rx_en) ? 1 : 0); //enable or disable the rx/tx modules

            U2MODEbits.ON = 1; //enable the UART

            uart_2_tx_callback = config.tx_callback; //link the callback functions
            uart_2_rx_callback = config.rx_callback;

            u2.Tx_is_idle = TRUE;
            
            //set the peripheral pin select
            if (config.rx_en)
            {
                switch (config.rx_pin) {
                    case Pin_RPA1:
                        TRISAbits.TRISA1 = 1; //input
                        ANSELAbits.ANSA1 = 0; //disable analog
                        U2RXR = 0;
                        break;
                    case Pin_RPB5:
                        TRISB |= 1<<5; //input
                        U2RXR = 1;
                        break;
                    case Pin_RPB1:
                        TRISBbits.TRISB1 = 1; //input
                        ANSELBbits.ANSB1 = 0; //disable analog
                        U2RXR = 2;
                        break;
                    case Pin_RPB11:
                        TRISBbits.TRISB11 = 1; //input
                        U2RXR = 3;
                        break;
                    case Pin_RPB8:
                        TRISBbits.TRISB8 = 1// input
                        U2RXR = 4;
                        break;
                    default:
                        //Error
                        break;
                        
                           
                }
            }
            
            if (config.tx_en)
            {
                switch (config.tx_pin) {
                    case Pin_RPA3:
                        RPA3R = 2; //U1TX
                        TRISAbits.TRISA3 = 0; //output
                        break;
                    case Pin_RPB14:
                        RPB14R = 2; //U1TX
                        TRISBbits.TRISB14 = 0; //output
                        break;
                    case Pin_RPB0:
                        RPB0R = 2; //U1TX
                        TRISBbits.TRISB0 = 0; //output
                        break;
                    case Pin_RPB10:
                        RPB10R = 2; //U1TX
                        TRISBbits.TRISB10 = 0; //output
                        break;
                    case Pin_RPB9:
                        RPB9R = 2; //U1TX
                        TRISBbits.TRISB9 = 0; //output
                        break;
                    default:
                        //error
                        break;
                }
            }
            
            return &u2;
            break;

        default:
            //some sort of error handling?
            break;
    }

    return NULL;
}

Error send_UART(UART_Channel channel, uint8 *data_ptr, uint data_size) {
    Error status = ERR_NO_ERR;
    //we need to place the provided data onto the Tx queue
    switch (channel) {
        case UART_CH_1:
            status = enqueue(&(u1.Tx_queue), data_ptr, data_size);
            if (u1.Tx_is_idle) { //if the tx is idle, force-start it
                IEC1bits.U1TXIE = 1;
                IFS1bits.U1TXIF = 1;
            }
            break;
        case UART_CH_2:
            status = enqueue(&(u2.Tx_queue), data_ptr, data_size);
            if (u2.Tx_is_idle) { ////if the tx is idle, force-start it
                IEC1bits.U1TXIE = 1;
                IFS1bits.U2TXIF = 1;
            }
            break;
        default:
            status = ERR_INVALID_SEND; //return failure
            break;

    }
    return status;
}

Error receive_UART(UART_Channel channel, uint8 *data_ptr, uint data_size) {
    Error status;
    //we need to read the specified data from the rx queue
    switch (channel) {
        case UART_CH_1:
            status = dequeue(&(u1.Rx_queue), data_ptr, data_size);
            break;
        case UART_CH_2:
            status = dequeue(&(u2.Rx_queue), data_ptr, data_size);
            break;
        default:
            status = ERR_INVALID_CHANNEL; //return failure
            break;
    }
    return status; //return success
}

void __ISR(_UART_1_VECTOR, IPL7AUTO) Uart_1_Handler(void) {
    uint8 received, transmit[UART_FIFO_SIZE], i, count;
    asm volatile ("di"); //disable interrupt

    if (IFS1bits.U1RXIF) { //if the interrupt flag of RX is set


        //we have received information - pop that information off the channel
        //push that data onto our received queue
        received = U1RXREG;
        enqueue(&(u1.Rx_queue), &received, 1);

        if (uart_1_rx_callback != NULL) {
            uart_1_rx_callback(); //call additional ISR functionality
        }

        //now clear the interrupt flag
        IFS1bits.U1RXIF = 0;
    }
    if (IFS1bits.U1TXIF) { //if the interrupt flag of TX is set
        u1.Tx_is_idle = 0; //tx is not idle
        //if the transmit queue is empty
        if (u1.Tx_queue.numStored == 0) {
            //our buffer is empty
            //we need to disable the interrupts
            IEC1bits.U1TXIE = 0;
            //and set the queue to idle
            u1.Tx_is_idle = 1;

        } else {
            //there is in the transmit queue, let's send it
            //assume the hardware FIFO is empty
            //it should, that's what triggers the tx interrupt

            //try to pull out enough data out of queue to
            //fill up the hardware FIFO
            if(u1.Tx_queue.numStored >= UART_FIFO_SIZE)
            {
                count = UART_FIFO_SIZE;
            }
            else //pull as much data as we can
            {
                count = u1.Tx_queue.numStored;
            }
            dequeue(&(u1.Tx_queue), transmit, count);

            //load the data into the hardware FIFO
            for(i=0; i < count; ++i)
            {
                U1TXREG = transmit[i];
            }

            if (uart_1_tx_callback != NULL) {
                uart_1_tx_callback(); //call additional ISR functionality
            }
            //now clear the interrupt flag
            IFS1bits.U1TXIF = 0;
        }
    }

    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_UART_2_VECTOR, IPL7AUTO) Uart_2_Handler(void) {
    uint8 received, transmit[UART_FIFO_SIZE], i, count;
    asm volatile ("di"); //disable interrupt

    if (IFS1bits.U2RXIF) { //if the interrupt flag of RX is set


        //we have received information - pop that information off the channel
        //push that data onto our received queue
        received = U2RXREG;
        enqueue(&(u2.Rx_queue), &received, 1);

        if (uart_2_rx_callback != NULL) {
            uart_2_rx_callback(); //call additional ISR functionality
        }

        //now clear the interrupt flag
        IFS1bits.U2RXIF = 0;
    }
    if (IFS1bits.U2TXIF) { //if the interrupt flag of TX is set
        u2.Tx_is_idle = 0; //tx is not idle

        //if the transmit queue is empty
        if (u2.Tx_queue.numStored == 0) {
            //our buffer is empty
            //we need to disable the interrupts
            IEC1bits.U2TXIE = 0;
            //and set the queue to idle
            u2.Tx_is_idle = 1;

        } else {
            //there is in the transmit queue, let's send it
            //assume the hardware FIFO is empty
            //it should, that's what triggers the tx interrupt

            //try to pull out enough data out of queue to
            //fill up the hardware FIFO
            if(u2.Tx_queue.numStored >= UART_FIFO_SIZE)
            {
                count = UART_FIFO_SIZE;
            }
            else //pull as much data as we can
            {
                count = u2.Tx_queue.numStored;
            }
            dequeue(&(u2.Tx_queue), transmit, count);

            //load the data into the hardware FIFO
            for(i=0; i < count; ++i)
            {
                U2TXREG = transmit[i];
            }
            
            if (uart_2_tx_callback != NULL) {
                uart_2_tx_callback(); //call additional ISR functionality
            }
            //now clear the interrupt flag
            IFS1bits.U2TXIF = 0;
        }
    }

    asm volatile ("ei"); //reenable interrupts
}
