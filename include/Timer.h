/*!
 * \file Timer.h
 *
 * A Timer pseudo-class
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "System.h"

    ///\enum Clock_Divider All possible clock dividers for a timer
    typedef enum {
        Div_1,
        Div_2,
        Div_4,
        Div_8,
        Div_16,
        Div_32,
        Div_64,
        Div_256
    } Clock_Divider;

    ///\enum Timer_Type All possible timers
    typedef enum {
        Timer_1 =2,
        Timer_2 =3,
        Timer_3 =4,
        Timer_4 =5,
        Timer_5 =6
    } Timer_Type;

    typedef struct TIMER_CONFIG {
        Timer_Type which_timer; ///< The timer to configure
        uint frequency;
        uint pbclk;
        void (*callback); ///< The function to call when the timer is triggered
        boolean enabled; ///< Tells if the timer is enabled
    }Timer_Config; ///< Configuration struct for any timer


    /* #Define Functions / Inline Functions*/
    //This function is the ISR #define function template
    /* #define Timer_ISR_(x) void __ISR(_Timer_x_Vector, IPL7AUTO) Timer_Handler_x (void) {\
         INTDisableInterrupts();\
         if (timer_x_callback != NULL) {\
             timer_x_callback();\
         }\
         IFS0bits.TxIF = 0;\
         INTENableInterrupts();\
     }*/


    /* Function Prototypes*/

    /**
     * Initialize a timer
     * \param config Configuration struct for the desired timer
     * \see TIMER_CONFIG
     */
    Error initialize_Timer(Timer_Config config);

    /**
     * Enables a timer
     * \param which_timer The timer to enable
     */
    Error enable_Timer(Timer_Type which_timer);

    /**
     * Disables a timer
     * \param which_timer The timer to disable
     */
    Error disable_Timer(Timer_Type which_timer);

    
    Error update_period_Timer(Timer_Type which_timer, int period);
    Error update_frequency_Timer(Timer_Type which_timer, float frequency);
    Error update_divider_Timer(Timer_Type which_timer, Clock_Divider div);



#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

