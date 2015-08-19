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
#define _SUPPRESS_PLIB_WARNING
#include <peripheral/ports.h>

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

    /// \enum Error Defines the possible communcation errors
    typedef enum {
        ERR_NO_ERR, //not an error
        ERR_INVALID_SEND,
        ERR_QUEUE_FULL, //queue is full
        ERR_QUEUE_INVALID_READ,
        ERR_INVALID_CHANNEL //invalid channel
    }Error;

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

