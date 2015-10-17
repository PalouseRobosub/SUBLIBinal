/*!
 * \file System.h
 * The main include header for the library
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif


/* Includes */
#include "attribs.h"
#include "pic32mx250f128b.h"
    
#ifndef NULL
    #define NULL 0
#endif

/* Type Defintions*/
    typedef unsigned int uint; ///< An unsigned 32-bit integer
    typedef unsigned short int uint16; ///< An unsigned 16-bit integer
    typedef unsigned char uint8; ///< An unsigned 8-bit integer
    typedef signed char sint8; ///< A signed 8-bit integer

    typedef uint u32;
    typedef uint16 u16;
    typedef uint8 u8;

    typedef enum {
        FALSE,
        TRUE
    } boolean;
    
        typedef enum {
        Pin_RPA0,
        Pin_RPA1,
        Pin_RPA2,
        Pin_RPA3,
        Pin_RPA4,
        Pin_RPB0,
        Pin_RPB1,
        Pin_RPB2,
        Pin_RPB3,
        Pin_RPB4,
        Pin_RPB5,
        Pin_RPB6,
        Pin_RPB7,
        Pin_RPB8,
        Pin_RPB9,
        Pin_RPB10,
        Pin_RPB11,
        Pin_RPB13,
        Pin_RPB14,
        Pin_RPB15
    }Pin;

    /// \enum Error Defines the possible communcation errors
    typedef enum {
        ERR_NO_ERR, //not an error
                ERR_INVALID_ENUM, //invalid enumeration supplied to function
        //Timer Errors
                ERR_TIMER_FREQ_UNATTAINABLE,
                ERR_INVALID_DIVIDER,
                ERR_INVALID_PERIOD,
                
                
        ERR_INVALID_SEND,
        ERR_QUEUE_FULL, //queue is full
        ERR_QUEUE_INVALID_READ,
        ERR_INVALID_CHANNEL //invalid channel
    }Error;

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

