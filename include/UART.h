/*!
 * \file UART.h
 *
 * The data types and methods for the UART channels
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Inclues*/
#include "System.h"
#include "Queue.h"

    /*Enum Defintions*/
    /// \enum UART_Channel The possible UART channels
    typedef enum {
        UART_CH_1,
        UART_CH_2
    } UART_Channel;

    /*Constants*/
    #define UART_FIFO_SIZE 8

    /*Object Defintions*/
    typedef struct UART_DATA {
        Queue Rx_queue; ///< Rx queue
        Queue Tx_queue; ///< Tx queue
        boolean Tx_is_idle; ///< Idle information
    } UART_Data; ///< Stores the references to the receive and transmit queues

    typedef struct UART_CONFIG {
        uint speed; ///< The baud rate to run UART at
        uint pb_clk; ///< The peripheral bus clock speed
        UART_Channel which_uart; ///< The UART to configure
        uint8 *rx_buffer_ptr; ///< Pointer to the receive buffer queue
        uint rx_buffer_size; ///< The size of the receive buffer
        uint8 *tx_buffer_ptr; ///< Pointer to the transmit buffer queue
        uint tx_buffer_size; ///< The size of the transmit buffer
        void (*rx_callback); ///< Callback function for a received byte
        void (*tx_callback); ///< Callback function for a transmitted byte
        boolean tx_en; ///< Enable the transmit line
        boolean rx_en; ///< Enable the receive line
    } UART_Config; ///< The configuration struct for the UART channels

    /*Function Prototypes*/
    /**
     * Initializes the specified UART channel
     * \param config Configuration information for the UART channel
     * \return A pointer to the UART's Rx and Tx queues
     * \see UART_CONFIG
     */
    UART_Data* initialize_UART(UART_Config config);

    /**
     * Send data through the UART channel
     * \param channel The UART channel to send data to
     * \param data_ptr The data to send the channel
     * \param data_size The number of bytes in the data
     * \return An Error type
     * \see Error
     */
    Error send_UART(UART_Channel channel, uint8 *data_ptr, uint data_size);

    /**
     * Receive data through the UART channel
     * \param channel The UART channel to receive data from
     * \param data_ptr The array to save the data in
     * \param data_size The number of bytes to read
     * \return An Error type
     * \see Error
     */
    Error receive_UART(UART_Channel channel, uint8 *data_ptr, uint data_size);
    
#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

