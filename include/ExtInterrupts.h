/* 
 * File:   ExtInterrupts.h
 * Author: Ryan
 *
 * Created on August 6, 2015, 5:42 PM
 */

#ifndef EXTINTERRUPTS_H
#define	EXTINTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    #include "System.h"

    typedef enum {
        Interrupt_0,
        Interrupt_1,
        Interrupt_2,
        Interrupt_3,
        Interrupt_4
    }Interrupt; 
    
    typedef enum {
        none,
        pullup,
        pulldown
    } Resistor;
    
    typedef struct INTERRUPT_CONFIG{
        Interrupt extInt;
        Pin pin;
        Polarity polarity;
        boolean enable;
        Resistor resistor;
        void *callback;
    } Interrupt_Config;
    
    Error initialize_Interrupt(Interrupt_Config config);
    void disable_Interrupt(Interrupt extInt);
    void enable_Interrupt(Interrupt extInt);
    

#ifdef	__cplusplus
}
#endif

#endif	/* EXTINTERRUPTS_H */

