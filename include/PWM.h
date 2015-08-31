/* 
 * File:   OC.h
 * Author: ryan
 *
 * Created on August 15, 2015, 9:20 PM
 */

#ifndef OC_H
#define	OC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "PPS.h"
#include "Timer.h"
    
    typedef enum {
        PWM_CH_1,
        PWM_CH_2,
        PWM_CH_3,
        PWM_CH_4,
        PWM_CH_5
    }PWM_Channel;
    
    
    typedef struct {
        float dutyCycle; // Only used in PWM mode
        Timer_Config timer;
        Pin pin; //our output pin
        uint8 enable;
        PWM_Channel channel;
    }PWM_Config;

    
    //functions
    void initialize_PWM(PWM_Config config);
    void enable_PWM(PWM_Config config);
    void disable_PWM(PWM_Config config);
    
    void update_PWM(PWM_Config config, float dutyCycle);
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* OC_H */

