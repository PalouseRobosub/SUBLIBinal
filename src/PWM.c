#include "PWM.h"
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


    void initialize_PWM(PWM_Config config) {
        //We will first initialize the OC channel
        switch (config.channel) {
            case PWM_CH_1:
                switch (config.pin) {
                    case Pin_RPB3:
                        ANSELBbits.ANSB3 = 0;
                        TRISBbits.TRISB3 = 0;
                        RPB3R = 0b0101;
                        break;
                    case Pin_RPB4:
                        TRISBbits.TRISB4 = 0;
                        RPB4R = 0b0101;
                        break;
                    case Pin_RPB15:
                        ANSELBbits.ANSB15 = 0;
                        TRISBbits.TRISB15 = 0;
                        RPB15R = 0b0101;
                        break;
                    case Pin_RPB7:
                        TRISBbits.TRISB7 = 0;
                        RPB7R = 0b0101;
                        break;
                    default: //RPA0
                        ANSELAbits.ANSA0 = 0;
                        TRISAbits.TRISA0 = 0;
                        RPA0R = 0b0101;
                        break;
                }
                OC1CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC1CONbits.OCTSEL = 0;
                    OC1R = config.dutyCycle * PR2;
                    OC1RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC1CONbits.OCTSEL = 1;
                    OC1R = config.dutyCycle * PR3;
                    OC1RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC1CONbits.OCM = 0b110; //PWM no fault pin mode
                
                OC1CONbits.ON = config.enable;
                break;
            case PWM_CH_2:
                switch (config.pin) {
                    case Pin_RPB5:
                        TRISBbits.TRISB5 = 0;
                        RPB5R = 0b0101;
                        break;
                    case Pin_RPB1:
                        ANSELBbits.ANSB1 = 0;
                        TRISBbits.TRISB1 = 0;
                        RPB1R = 0b0101;
                        break;
                    case Pin_RPB11:
                        TRISBbits.TRISB11 = 0;
                        RPB11R = 0b0101;
                        break;
                    case Pin_RPB8:
                        TRISBbits.TRISB8 = 0;
                        RPB8R = 0b0101;
                        break;
                    default: //RPA1
                        ANSELAbits.ANSA1 = 0;
                        TRISAbits.TRISA1 = 0;
                        RPA1R = 0b0101;
                        break;
                }
                OC2CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC2CONbits.OCTSEL = 0;
                    OC2R = config.dutyCycle * PR2;
                    OC2RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC2CONbits.OCTSEL = 1;
                    OC2R = config.dutyCycle * PR3;
                    OC2RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC2CONbits.OCM = 0b110; //PWM no fault pin mode
                
                OC2CONbits.ON = config.enable;
                break;
            case PWM_CH_3:
                switch (config.pin) {
                    case Pin_RPB14:
                        ANSELBbits.ANSB14 = 0;
                        TRISBbits.TRISB14 = 0;
                        RPB14R = 0b0101;
                        break;
                    case Pin_RPB0:
                        ANSELBbits.ANSB0 = 0;
                        TRISBbits.TRISB0 = 0;
                        RPB0R = 0b0101;
                        break;
                    case Pin_RPB10:
                        TRISBbits.TRISB10 = 0;
                        RPB10R = 0b0101;
                        break;
                    case Pin_RPB9:
                        TRISBbits.TRISB9 = 0;
                        TRISBbits.TRISB9 = 0;
                        RPB9R = 0b0101;
                        break;
                    default: //RPA3
                        TRISAbits.TRISA3 = 0;
                        RPA3R = 0b0101;
                        break;
                }
                OC3CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC3CONbits.OCTSEL = 0;
                    OC3R = config.dutyCycle * PR2;
                    OC3RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC3CONbits.OCTSEL = 1;
                    OC3R = config.dutyCycle * PR3;
                    OC3RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC3CONbits.OCM = 0b110; //PWM no fault pin mode
                
                OC3CONbits.ON = config.enable;
                break;
            case PWM_CH_4:
                switch (config.pin) {
                    case Pin_RPB6:
                        //error, the xc32 compiler doesnt support this for some reason
                        break;
                    case Pin_RPA4:
                        TRISAbits.TRISA4 = 0;
                        RPA4R = 0b0101;
                        break;
                    case Pin_RPB13:
                        TRISBbits.TRISB13 = 0;
                        RPB13R = 0b0101;
                        break;
                    case Pin_RPB2:
                        TRISBbits.TRISB2 = 0;
                        TRISBbits.TRISB2 = 0;
                        RPB2R = 0b0101;
                        break;
                    default: //RPA2
                        TRISAbits.TRISA2 = 0;
                        RPA2R = 0b0101;
                        break;
                }
                OC4CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC4CONbits.OCTSEL = 0;
                    OC4R = config.dutyCycle * PR2;
                    OC4RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC4CONbits.OCTSEL = 1;
                    OC4R = config.dutyCycle * PR3;
                    OC4RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC4CONbits.OCM = 0b110; //PWM no fault pin mode
                
                OC4CONbits.ON = config.enable;
                break;
            case PWM_CH_5:
                switch (config.pin) {
                    case Pin_RPB6:
                        //error, the xc32 compiler doesnt support this for some reason
                        break;
                    case Pin_RPA4:
                        TRISAbits.TRISA4 = 0;
                        RPA4R = 0b0110;
                        break;
                    case Pin_RPB13:
                        TRISBbits.TRISB13 = 0;
                        RPB13R = 0b0110;
                        break;
                    case Pin_RPB2:
                        TRISBbits.TRISB2 = 0;
                        TRISBbits.TRISB2 = 0;
                        RPB2R = 0b0110;
                        break;
                    default: //RPA2
                        TRISAbits.TRISA2 = 0;
                        RPA2R = 0b0110;
                        break;
                }
                OC5CON = 0;
                if (config.timer.which_timer == Timer_2) {
                    OC5CONbits.OCTSEL = 0;
                    OC5R = config.dutyCycle * PR2;
                    OC5RS = config.dutyCycle * PR2;
                } else if (config.timer.which_timer == Timer_3){
                    OC5CONbits.OCTSEL = 1;
                    OC5R = config.dutyCycle * PR3;
                    OC5RS = config.dutyCycle * PR3;
                } else {
                    //error
                }
                OC5CONbits.OCM = 0b110; //PWM no fault pin mode
                
                OC5CONbits.ON = config.enable;
                break;
        }
    }
    
    void enable_PWM(PWM_Config config) {
        switch (config.channel) {
            case PWM_CH_1:
                OC1CONbits.ON = 1;
                break;
            case PWM_CH_2:
                OC2CONbits.ON = 1;
                break;
            case PWM_CH_3:
                OC3CONbits.ON = 1;
                break;
            case PWM_CH_4:
                OC4CONbits.ON = 1;
                break;
            case PWM_CH_5:
                OC5CONbits.ON = 1;
                break;
        }
    }
    
    void disable_PWM(PWM_Config config) {
        switch (config.channel) {
            case PWM_CH_1:
                OC1CONbits.ON = 0;
                IEC0bits.OC1IE = 0;
                break;
            case PWM_CH_2:
                OC2CONbits.ON = 0;
                IEC0bits.OC2IE = 0;
                break;
            case PWM_CH_3:
                OC3CONbits.ON = 0;
                IEC0bits.OC3IE = 0;
                break;
            case PWM_CH_4:
                OC4CONbits.ON = 0;
                IEC0bits.OC4IE = 0;
                break;
            case PWM_CH_5:
                OC5CONbits.ON = 0;
                IEC0bits.OC5IE = 0;
                break;
        }
    }
    
    void update_PWM(PWM_Config config, float dutyCycle) {
        //update the duty cycle of the respective PWM
        switch (config.channel) {
            case PWM_CH_1:
                if (config.timer.which_timer == Timer_2) {
                    OC1RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC1RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_2:
                if (config.timer.which_timer == Timer_2) {
                    OC2RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC2RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_3:
                if (config.timer.which_timer == Timer_2) {
                    OC3RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC3RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_4:
                if (config.timer.which_timer == Timer_2) {
                    OC4RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC4RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
            case PWM_CH_5:
                if (config.timer.which_timer == Timer_2) {
                    OC5RS = PR2*dutyCycle;
                } else if (config.timer.which_timer == Timer_3) {
                    OC5RS = PR3*dutyCycle;
                } else {
                    //error
                }
                break;
        }
    }