/*!
 * \file SPI.h
 *
 * Defines the data structures and methods for using the SPI channels
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"
#include "Queue.h"

    /* Enums */
    ///\enum SPI_Channel All possible SPI Channels
    typedef enum {
        SPI_CH_1,
        SPI_CH_2
    } SPI_Channel;


    /*Object Defintions*/
    typedef struct SPI_DATA {
        Queue Rx_queue;///< Received data
        Queue Tx_queue;///< Data that needs to be sent
        boolean is_idle;///< Idle information
    } SPI_Data;///< Object for references to the data queues

    typedef struct SPI_CONFIG {
        uint speed; ///< The desired baud rate
        uint pb_clk; ///< The speed of the peripheral bus clock
        SPI_Channel which_spi; ///< The channel to configure
        uint8 clk_edge; ///< ?????
        uint8 *tx_buffer_ptr; ///< Pointer to the transmit buffer queue
        uint tx_buffer_size; ///< Size of the transmit buffer
        void (*tx_callback); ///< Function to call when transmission is made
        boolean tx_en; ///< Enable the Tx line?
        uint8 *rx_buffer_ptr; ///< Pointer to the receive buffer queue
        uint rx_buffer_size; ///< Size of the receive buffer
        void (*rx_callback); ///< Function to call when data is received
        boolean rx_en; ///< Enable the Rx line?
    }SPI_Config; ///< Configuration struct for initializing an SPI channel

    /**
     * Initialize an SPI channel
     * \param config The struct containing the configuration to apply to the channel
     * \see SPI_CONFIG
     * \return Pointer to the transmit and receive queue
     */
    SPI_Data* initialize_SPI(SPI_Config config);

    /**
     * Send data on the SPI bus
     * \param channel The channel to send on
     * \param data_ptr The data to send
     * \param data_size The size of the data
     * \return Error type
     * \see Error
     */
    Error send_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size);

    //int receive_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size);




#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

