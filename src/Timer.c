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


void (*timer_1_callback) (void);
void (*timer_2_callback) (void);
void (*timer_3_callback) (void);
void (*timer_4_callback) (void);
void (*timer_5_callback) (void);

/*Timer_Setup function*/
//this function initializes a specific timer on the microcontroller with the specified input
//divide is the prescaler for the clock divider
//period is the max value that the timer will count to
//timer is the specific timer that is being worked with
//function_ptr is the ISR function pointer
//enable specifies whether to enable the interrupt or not

void initialize_Timer(Timer_Config config) {
    
    //determine the best divider based upon desired frequency
    Clock_Divider div;
    int period;
    //first,we will determine the best clock divider to use based upon max PR size, this will give us highest accuracy
    if (config.frequency  > (float)config.pbclk/65535) {
        //we can use Div_1
        div = Div_1;
        period = config.pbclk/(config.frequency*1);
    } else if (config.frequency > (float)config.pbclk/(65535*2)){
        //div 2 is best
        div = Div_2;
        period = config.pbclk/(config.frequency*2);
    } else if (config.frequency > (float)config.pbclk/(65535*4)) {
        //div 4
        div = Div_4;
        period = config.pbclk/(config.frequency*4);
    } else if(config.frequency > (float)config.pbclk/(65535*8)) {
        //div 8
        div = Div_8;
        period = config.pbclk/(config.frequency*8);
    } else if (config.frequency > (float)config.pbclk/(65535*16)) {
        //div 16
        div = Div_16;
        period = config.pbclk/(config.frequency*16);
    } else if (config.frequency > (float)config.pbclk/(65535*32)) {
        //div 32
        div = Div_32;
        period = config.pbclk/(config.frequency*32);
    } else if (config.frequency > (float)config.pbclk/(65535*64)) {
        //div 64
        div = Div_64;
        period = config.pbclk/(config.frequency*64);
    } else {
        //div 256
        div = Div_256;
        period = config.pbclk/(config.frequency*256);
    }

    //switch case to determine which timer we are working with
    switch (config.which_timer) {
        case Timer_1:
            //determine the divider for a type A timer
            switch (div) {
                case Div_1:
                    T1CONbits.TCKPS = 0; //1:8 is closest scaler
                    PR1 = period;
                    break;
                case Div_2:
                    
                    T1CONbits.TCKPS = 0b1; //1:8 is closest scaler
                    PR1 = period/4;
                    break;
                case Div_4:
                    T1CONbits.TCKPS = 0b1; //1:8 is closest scaler
                    PR1 = period/2;
                    break;
                case Div_8:
                    T1CONbits.TCKPS = 0b1; //1:8 
                    PR1 = period;
                    break;
                case Div_16:
                    T1CONbits.TCKPS = 0b10; //1:64 is closest scaler
                    PR1 = period/4;
                    break;
                case Div_32:
                    T1CONbits.TCKPS = 0b10; //1:64 is closest scaler
                    PR1 = period/2;
                    break;
                case Div_64:
                    T1CONbits.TCKPS = 0b10; //1:64
                    PR1 = period;
                    break;
                case Div_256:
                    T1CONbits.TCKPS = 0b11; //1:256
                    PR1 = period;
                    break;
            }
            
            IPC1bits.T1IP = 7; //set the interrupt to priority level 7
            if (config.callback != NULL) {
                IEC0bits.T1IE = config.enabled; //enable the interrupt
            }
            timer_1_callback = config.callback; //set the ISR function pointer
            T1CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_2:
            T2CONbits.TCKPS = div; //set the clock divider
            PR2 = period; //set the period for the timer
            IPC2bits.T2IP = 7; //set the interrupt priority
            if (config.callback != NULL) {
                IEC0bits.T2IE = config.enabled; //set the interrupt enable
            }
            timer_2_callback = config.callback; //set the ISR function pointer
            T2CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_3:
            T3CONbits.TCKPS = div; //set the clock divider
            PR3 = period; //set the period of the timer
            IPC3bits.T3IP = 7; //set the interrupt priotity
            if (config.callback != NULL) {
                IEC0bits.T3IE = config.enabled; //enable the interrupt
            }
            timer_3_callback = config.callback; //set the ISR function pointer
            T3CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_4:
            T4CONbits.TCKPS = div;
            PR4 = period;
            IPC4bits.T4IP = 7;
            if (config.callback != NULL) {
                IEC0bits.T4IE = config.enabled;
            }
            timer_4_callback = config.callback;
            T4CONbits.ON = config.enabled; //actually turn the timer on
            break;

        case Timer_5:
            T5CONbits.TCKPS = div;
            PR5 = period;
            IPC5bits.T5IP = 7;
            if (config.callback != NULL) {
                IEC0bits.T5IE = config.enabled;
            }
            timer_5_callback = config.callback;
            T5CONbits.ON = config.enabled; //actually turn the timer on
            break;
    }
}

void enable_timer(Timer_Type which_timer)
{
    switch(which_timer)
    {
        case Timer_1:
            T1CONbits.ON = 1;
            if (timer_1_callback != NULL) {
                IEC0bits.T1IE = 1;
            }
            break;
        case Timer_2:
            T2CONbits.ON = 1;
            if (timer_2_callback != NULL) {
                IEC0bits.T2IE = 1;
            }
            break;
        case Timer_3:
            T3CONbits.ON = 1;
            if (timer_3_callback != NULL) {
                IEC0bits.T3IE = 1;
            }
            break;
        case Timer_4:
            T4CONbits.ON = 1;
            if (timer_4_callback != NULL) {
                IEC0bits.T4IE = 1;
            }
            break;
        case Timer_5:
            T5CONbits.ON = 1;
            if (timer_5_callback != NULL) {
                IEC0bits.T5IE = 1;
            }
            break;
    }
}

void disable_timer(Timer_Type which_timer)
{
    switch(which_timer)
    {
        case Timer_1:
            T1CONbits.ON = 0;
            IEC0bits.T1IE = 0;
            break;
        case Timer_2:
            T2CONbits.ON = 0;
            IEC0bits.T2IE = 0;
            break;
        case Timer_3:
            T3CONbits.ON = 0;
            IEC0bits.T3IE = 0;
            break;
        case Timer_4:
            T4CONbits.ON = 0;
            IEC0bits.T4IE = 0;
            break;
        case Timer_5:
            T5CONbits.ON = 0;
            IEC0bits.T5IE = 0;
            break;
    }
}

void __ISR(_TIMER_1_VECTOR, IPL7AUTO) Timer_Handler_1(void) {
    asm volatile ("di"); //disable interrupt


    if (timer_1_callback != NULL) {
        timer_1_callback();
    }

    IFS0bits.T1IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_2_VECTOR, IPL7AUTO) Timer_Handler_2(void) {
    asm volatile ("di"); //display interrupts


    if (timer_2_callback != NULL) {
        timer_2_callback();
    }

    IFS0bits.T2IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_3_VECTOR, IPL7AUTO) Timer_Handler_3(void) {
    asm volatile ("di"); //disable interrupts


    if (timer_3_callback != NULL) {
        timer_3_callback();
    }

    IFS0bits.T3IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_4_VECTOR, IPL7AUTO) Timer_Handler_4(void) {
    asm volatile ("di"); //disable interrupts


    if (timer_4_callback != NULL) {
        timer_4_callback();
    }

    IFS0bits.T4IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_TIMER_5_VECTOR, IPL7AUTO) Timer_Handler_5(void) {
    asm volatile ("di"); //disable interrupts


    if (timer_5_callback != NULL) {
        timer_5_callback();
    }

    IFS0bits.T5IF = 0; //clear the interrupt flag
    asm volatile ("ei"); //reenable interrupts
}
