
#include "ADC.h"

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
void (*ADC_callback) (void);

//channel data structures
ADC_Data adc_data;

boolean ADC_startup;

ADC_Data* initialize_ADC(ADC_Config config) {

    //setup the rx and tx buffers
    adc_data.Results_queue = create_queue(config.result_buffer_ptr, config.result_buffer_size);
    adc_data.Work_queue = create_queue(config.work_buffer_ptr, config.work_buffer_size);

    ADC_callback = config.callback; //link the callback function

    adc_data.is_idle = TRUE; //set the ADC module to idling

    //Select the analog inputs to the ADC multiplexers in AD1CHS
    AD1CHSbits.CH0NA = 0; //set the negative input for channel A to internal ground


    //set up pins for analog function
    setup_ADC_pins(config.channels);
    
    //Select the data format with FORM<2:0> (AD1CON1)
    AD1CON1bits.FORM = 0; //set the format to be simple 16 bit unsigned integer

    //Select the sample clock source using SSRC<2:0> (AD1CON1)
    AD1CON1bits.SSRC = 0b111; //set the clock source to be internal counter

    //Select the voltage reference source using VCFG<2:0> (AD1CON2)
    AD1CON2bits.VCFG = 000; //set the positve and negative references to be Vdd and Vss

    //Select the Scan mode using CSCNA (AD1CON2)
    AD1CON2bits.CSCNA = 0; //do not scan inputs

    //Set the number of conversions per interrupt SMP<3:0> (AD1CON2)
    AD1CON2bits.SMPI = 0; //interrupt after every conversion

    //Set Buffer Fill mode using BUFM (AD1CON2)
    AD1CON2bits.BUFM = 0; //use single 16 word buffer (don't really care, we aren't using the buffer)

    //Select the MUX to be connected to the ADC in ALTS AD1CON2
    AD1CON2bits.ALTS = 0; //always use mux A

    //Select the ADC clock source using ADRC (AD1CON3)
    AD1CON3bits.ADRC = 1; //use internal ADC clock

    //Select the sample time using SAMC<4:0> (AD1CON3)
    AD1CON3bits.SAMC = 15; //sample for 15 ADC clock cycles

    //Select the ADC clock prescaler using ADCS<7:0> (AD1CON3)
    //doesn't matter, we are using the internal oscillator

    //Turn the ADC module on using AD1CON1
    AD1CON1bits.ADON = 1;

    //setup interrupts
    IFS0bits.AD1IF = 0; //clear the interrupt flag
    IPC5bits.AD1IP = 7; //set the priority of the interrupt to 7
    IEC0bits.AD1IE = 1; //enable the interrupt

    return &adc_data;
}

int read_ADC(ADC_Node node) {
    int status;
    
    status = enqueue(&(adc_data.Work_queue), (uint8*) & node, sizeof (node));

    //if the bus is idling, force-start it
    if (adc_data.is_idle) {
        IFS0bits.AD1IF = 1;
    }


    return status;
}

void bg_process_ADC(void) {
    static ADC_Node current_node; //static to improve speed

    while(!dequeue(&(adc_data.Results_queue), (uint8*) & current_node, sizeof(current_node)))
    {
        if(current_node.callback != NULL){
            current_node.callback(current_node);
        }

    }
}

void __ISR(_ADC_VECTOR, IPL7AUTO) ADC_Handler(void) {
    static ADC_Node current_node;

    asm volatile ("di"); //disable interrupts


    if (ADC_startup == FALSE) { //if there is data in the ADC buffer
        current_node.data = ADC1BUF0;
        enqueue(&(adc_data.Results_queue), (uint8*) & current_node, sizeof(current_node));
    }

    if (dequeue(&(adc_data.Work_queue), (uint8*) & current_node, sizeof(current_node))) //load next node from the queue
    {
        adc_data.is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
        ADC_startup = TRUE;
    } else {
        adc_data.is_idle = FALSE; //flag that the bus is working now
        AD1CHSbits.CH0SA = current_node.channel; //set the positive input to be channel
        AD1CON1bits.SAMP = 1; //start sampling
        ADC_startup = FALSE; //set that this is not the first run
    }




    if (ADC_callback != NULL) {
        ADC_callback();
    }

    IFS0bits.AD1IF = 0; //clear the interrupt flag

    asm volatile ("ei"); //reenable interrupts
}

//setup the proper pins for analog use
void setup_ADC_pins(uint16 channels) {
    /* Analog pin mapping:
     * AN0 - RA0
     * AN1 - RA1
     * AN2 - RB0
     * AN3 - RB1
     * AN4 - RB2
     * AN5 - RB3
     * AN9 - RB15
     * AN10 - RB14
     * AN11 - RB13
     *
     * Needs to be configured:
     * TRISx
     * ANSELx
     *
     * The method below sucks, if anybody has a better idea that requires
     * less code I welcome, a refactor of this function.
     */

    if (channels & (1 << ADC_CH_0))
    {
        TRISAbits.TRISA0 = 1;
        ANSELAbits.ANSA0 = 1;
    }
    if (channels & (1 << ADC_CH_1))
    {
        TRISAbits.TRISA1 = 1;
        ANSELAbits.ANSA1 = 1;
    }
    if (channels & (1 << ADC_CH_2))
    {
        TRISBbits.TRISB0 = 1;
        ANSELBbits.ANSB0 = 1;
    }
    if (channels & (1 << ADC_CH_3))
    {
        TRISBbits.TRISB1 = 1;
        ANSELBbits.ANSB1 = 1;
    }
    if (channels & (1 << ADC_CH_4))
    {
        TRISBbits.TRISB2 = 1;
        ANSELBbits.ANSB2 = 1;
    }
    if (channels & (1 << ADC_CH_5))
    {
        TRISBbits.TRISB3 = 1;
        ANSELBbits.ANSB3 = 1;
    }
    if (channels & (1 << ADC_CH_9))
    {
        TRISBbits.TRISB15 = 1;
        ANSELBbits.ANSB15 = 1;
    }
    if (channels & (1 << ADC_CH_10))
    {
        TRISBbits.TRISB14 = 1;
        ANSELBbits.ANSB14 = 1;
    }
    if (channels & (1 << ADC_CH_11))
    {
        TRISBbits.TRISB13 = 1;
        ANSELBbits.ANSB13 = 1;
    }

}
