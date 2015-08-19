/*!
 * \file I2C.h
 * Defines the data structures and methods for using the I2C channels
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif


/*Include Statements*/
#include "System.h"
#include "Queue.h"

#define I2C_SPEED (100000)

    /* Enums */
    /// \enum I2C_MODE Reading or writing from the device
    typedef enum
    {
        READ,
        WRITE
    } I2C_MODE;

    /// \enum I2C_STATE All possible states for the I2C bus
    typedef enum
    {
        STARTED,
        DEV_ADDR_W_SENT,
        SUB_ADDR_SENT,
        DATA_SENT,
        RESTARTED,
        STOPPED_TEMP,
        STARTED_TEMP,
        DEV_ADDR_R_SENT,
        DATA_RECEIVED,
        ACK_SENT,
        NOACK_SENT,
        STOPPED
    } I2C_STATE;

    typedef enum //I2C_Channel
    {
        I2C_CH_1,
        I2C_CH_2
    } I2C_Channel;


    /* helpful macros */
#define get_read_addr(x) ((x << 1) | 0x01)
#define get_write_addr(x) ((x << 1))

    /*Object Defintions*/
    typedef struct I2C_DATA {
        Queue Rx_queue; ///< The receive queue
        Queue Tx_queue; ///< The transmit queue
        boolean Tx_is_idle; ///< Is the transmit line idle?
    } I2C_Data; ///< I2C_DATA Used for references to the I2C data queues

    typedef struct I2C_NODE {
        uint8 device_id; ///< A unique identifier for the device
        uint8 device_address; ///< I2C address for device
        uint8 sub_address; ///< Internal device address
        uint8* data_buffer; ///< Data buffer to store the recieved data or write data
        uint8 data_size; ///< How much data to send/read from device (must be <= buffer size)
        I2C_MODE mode; ///< Reading or writing?
        void (*callback) (struct I2C_NODE); ///< Callback function
    } I2C_Node; ///< I2C data node

    typedef struct I2C_CONFIG {
        uint pb_clk; ///< The speed of the peripheral bus clock
        I2C_Channel channel; ///< The I2C channel to use
        uint8 *rx_buffer_ptr; ///< Pointer to the receive buffer queue
        uint rx_buffer_size; ///< The size of the receive buffer
        uint8 *tx_buffer_ptr; ///< Pointer to the transmit buffer queue
        uint tx_buffer_size; ///< The size of the transmit buffer
        void* callback; ///< Callback function
    }I2C_Config; ///< The configuration struct for the I2C busses

    /*Function Prototypes*/
    /**
     * Initializes an I2C bus
     * \param config configuration information for the I2C bus to initialize
     * \see I2C_CONFIG
     * \return A poiner to the receive and transmit queues
     */
    I2C_Data* initialize_I2C(I2C_Config config);

    /**
     * Queues the node data into the Transmit queue
     * \param channel The channel to send the data on
     * \param node struct of the data to send and where to send it
     * \return An Error type
     * \see Error
     * \see I2C_NODE
     */
    Error send_I2C(I2C_Channel channel, I2C_Node node);

    //run this background process in the main while loop to
    //process the results of I2C transactions
    /**
     * Processes the results of I2C transactions.
     * Run in the main while loop.
     */
    void bg_process_I2C(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

