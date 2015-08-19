#include "ExtInterrupts.h"
#include "System.h"
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



void (*callback_int0) (void);
void (*callback_int1) (void);
void (*callback_int2) (void);
void (*callback_int3) (void);
void (*callback_int4) (void);


void initialize_Interrupt(Interrupt_Config config) 
{
    
    //for initialization, we must first set the directionality of the pin and disable any analog values on the pin
    switch (config.extInt) {
    //set the tristate and the PPS
    //disable analog on this pin
        case INT0:
            switch (config.resistor) {
                case pullup:
                    CNPUBbits.CNPUB7 = 1;
                    break;
                case pulldown:
                    CNPDBbits.CNPDB7 = 1;
                    break;
            }
            TRISBbits.TRISB7 = 1;
            INTCONbits.INT0EP = config.polarity; //set polarity
            IPC0bits.INT0IP = 7; //set priority
            IFS0bits.INT0IF = 0;//clear the flag
            IEC0bits.INT0IE = config.enable; //enable the interrupt?
            callback_int0 = config.callback;//set the callback
            break;
        case INT1:
            switch (config.pin) {
                case Pin_RPB14:
                    INT1R = 1;
                    ANSELBbits.ANSB14 = 0;
                    TRISBbits.TRISB14 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB14 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB14 = 1;
                            break;
                    }
                    break;
                case Pin_RPB0:
                    INT1R = 2;
                    ANSELBbits.ANSB0 = 0;
                    TRISBbits.TRISB0 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB0 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB0 = 1;
                            break;
                    }
                    break;
                case Pin_RPB10:
                    INT1R = 3;
                    TRISBbits.TRISB10 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB10 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB10 = 1;
                            break;
                    }
                    break;
                case Pin_RPB9:
                    INT1R = 4;
                    TRISBbits.TRISB9 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB9 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB9 = 1;
                            break;
                    }
                    break;
                default: //RPA3 will also default to here
                    TRISAbits.TRISA3 = 1;
                    INT1R = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUAbits.CNPUA3 = 1;
                            break;
                        case pulldown:
                            CNPDAbits.CNPDA3 = 1;
                            break;
                    }
                    break;
            }
            
            INTCONbits.INT1EP = config.polarity;
            IPC1bits.INT1IP = 7;//set priority
            IFS0bits.INT1IF = 0;//clear flag
            IEC0bits.INT1IE = config.enable;//enable-disable
            callback_int1 = config.callback;//set callback
            break;
        case INT2:
            switch (config.pin) {
                //Pin RPB6 is not available on the PIC32MX250F128B
                case Pin_RPA4:
                    INT2R = 2; 
                    TRISAbits.TRISA4 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUAbits.CNPUA4 = 1;
                            break;
                        case pulldown:
                            CNPDAbits.CNPDA4 = 1;
                            break;
                    }
                    break;
                case Pin_RPB13:
                    INT2R = 3;
                    TRISBbits.TRISB13 = 1;
                    ANSELBbits.ANSB13 = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB13 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB13 = 1;
                            break;
                    }
                    break;
                case Pin_RPB2:
                    INT2R = 4;
                    TRISBbits.TRISB2;
                    ANSELBbits.ANSB2 = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB2 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB2 = 1;
                            break;
                    }
                    break;
                default: // RPA2 will be used
                    INT2R = 0;
                    TRISAbits.TRISA2 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUAbits.CNPUA2 = 1;
                            break;
                        case pulldown:
                            CNPDAbits.CNPDA2 = 1;
                            break;
                    }
                    break;
            }
            INTCONbits.INT2EP = config.polarity;
            IPC2bits.INT2IP = 7;//priority
            IFS0bits.INT2IF = 0;//clear flag
            IEC0bits.INT2IE = config.enable;//enable disable
            callback_int2 = config.callback;//clalback
            break;
        case INT3:
            switch (config.pin) {
                case Pin_RPB5:
                    INT3R = 1;
                    TRISBbits.TRISB5 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB5 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB5 = 1;
                            break;
                    }
                    break;
                case Pin_RPB1:
                    INT3R = 2;
                    TRISBbits.TRISB1 = 1;
                    ANSELBbits.ANSB1 = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB1 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB1 = 1;
                            break;
                    }
                    break;
                case Pin_RPB11:
                    INT3R = 3;
                    TRISBbits.TRISB11 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB11 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB11 = 1;
                            break;
                    }
                    break;
                case Pin_RPB8:
                    INT3R = 4;
                    TRISBbits.TRISB8 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB8 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB8 = 1;
                            break;
                    }
                    break;
                default: //RPA1
                    INT3R = 0;
                    TRISAbits.TRISA1 = 1;
                    ANSELAbits.ANSA1 = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUAbits.CNPUA1 = 1;
                            break;
                        case pulldown:
                            CNPDAbits.CNPDA1 = 1;
                            break;
                    }
                    break;
            }
            INTCONbits.INT3EP = config.polarity;
            IPC3bits.INT3IP = 7;
            IFS0bits.INT3IF = 0;
            IEC0bits.INT3IE = config.enable;
            callback_int3 = config.callback;
            break;
        case INT4:
            switch (config.pin) {
                case Pin_RPB3:
                    INT4R = 1;
                    TRISBbits.TRISB3 = 1;
                    ANSELBbits.ANSB3 = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB3 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB3 = 1;
                            break;
                    }
                    break;
                case Pin_RPB4:
                    INT4R = 2;
                    TRISBbits.TRISB4 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB4 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB4 = 1;
                            break;
                    }
                    break;
                case Pin_RPB15:
                    INT4R = 3;
                    TRISBbits.TRISB15 = 1;
                    ANSELBbits.ANSB15 = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB15 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB15 = 1;
                            break;
                    }
                    break;
                case Pin_RPB7:
                    INT4R = 4;
                    TRISBbits.TRISB7 = 1;
                    switch (config.resistor) {
                        case pullup:
                            CNPUBbits.CNPUB7 = 1;
                            break;
                        case pulldown:
                            CNPDBbits.CNPDB7 = 1;
                            break;
                    }
                    break;
                default://RPA0
                    INT4R = 0;
                    TRISAbits.TRISA0 = 1;
                    ANSELAbits.ANSA0 = 0;
                    switch (config.resistor) {
                        case pullup:
                            CNPUAbits.CNPUA0 = 1;
                            break;
                        case pulldown:
                            CNPDAbits.CNPDA0 = 1;
                            break;
                    }
                    break;
            }
            INTCONbits.INT4EP = config.polarity;
            IPC4bits.INT4IP = 7;
            IFS0bits.INT4IF = 0;
            IEC0bits.INT4IE = 0;
            callback_int4 = config.callback;
            break;
        default:
            //error
            break;
    }

}

void disable_Interrupt(Interrupt extInt) {
    //clear the bit
    switch (extInt) {
        case INT0:
            IEC0bits.INT0IE = 0;
            break;
        case INT1:
            IEC0bits.INT1IE = 0;
            break;
        case INT2:
            IEC0bits.INT2IE = 0;
            break;
        case INT3:
            IEC0bits.INT3IE = 0;
            break;
        case INT4:
            IEC0bits.INT4IE = 0;
            break;
        default:
            //error
            break;
    }
}
void enable_Interrupt(Interrupt extInt) {
    //set the bit
    switch (extInt) {
        case INT0:
            IEC0bits.INT0IE = 1;
            break;
        case INT1:
            IEC0bits.INT1IE = 1;
            break;
        case INT2:
            IEC0bits.INT2IE = 1;
            break;
        case INT3:
            IEC0bits.INT3IE = 1;
            break;
        case INT4:
            IEC0bits.INT4IE = 1;
            break;
        default:
            //error
            break;
    }
}

void __ISR(_EXTERNAL_0_VECTOR, IPL7AUTO) Ext_Int0_Handler (void) {
    asm volatile ("di"); //disable interrupts
    
    //call the function pointer
    if (callback_int0 != NULL) {
        (*callback_int0)();
    }
    IFS0bits.INT0IF = 0;
    asm volatile ("ei"); //reenable interrupts
}
void __ISR(_EXTERNAL_1_VECTOR, IPL7AUTO) Ext_Int1_Handler (void) {
    asm volatile ("di"); //disable interrupts
    
    //call the function pointer
    if (callback_int1 != NULL) {
        (*callback_int1)();
    }
    IFS0bits.INT1IF = 0;
    asm volatile ("ei"); //reenable interrupts
}
void __ISR(_EXTERNAL_2_VECTOR, IPL7AUTO) Ext_Int2_Handler (void) {
    asm volatile ("di"); //disable interrupts
    
    //call the function pointer
    if (callback_int2 != NULL) {
        (*callback_int2)();
    }
    IFS0bits.INT2IF = 0;
    asm volatile ("ei"); //reenable interrupts
}
void __ISR(_EXTERNAL_3_VECTOR, IPL7AUTO) Ext_Int3_Handler (void) {
    asm volatile ("di"); //disable interrupts
    
    //call the function pointer
    if (callback_int3 != NULL) {
        (*callback_int3)();
    }
    IFS0bits.INT3IF = 0;
    asm volatile ("ei"); //reenable interrupts
}
void __ISR(_EXTERNAL_4_VECTOR, IPL7AUTO) Ext_Int4_Handler (void) {
    asm volatile ("di"); //disable interrupts
    
    //call the function pointer
    if (callback_int4 != NULL) {
        (*callback_int4)();
    }
    IFS0bits.INT4IF = 0;
    asm volatile ("ei"); //reenable interrupts
}