/*!
 * \file packetizer.h
 *
 * Defines the data types and methods for creating a packetizer on a specified Data_Channel
 */

#ifndef PACKETIZER_H
#define	PACKETIZER_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"
#include "UART.h"

    /// \enum Data_Channel Set of all packetizable channels
    typedef enum {
        PACKET_UART_CH_1,
        PACKET_UART_CH_2
    } Data_Channel;

    typedef struct PACKETIZER_DATA {
       void (*receive_callback) (uint8* data, uint8 data_size); ///< Receive callback function for the channel
       uint8 control_byte; ///< Control byte for the channel
       boolean sync_lock; ///< Tells if the transmission line synced with the incoming packets
       boolean packet_received; ///< Tells if a packet has been received for further processing
       uint8 received_bytes[255]; ///< Array of the received bytes
       uint16 received_index; ///< ????????
       uint8 packet_length; ///< The size of the packet
    } Packetizer_Data; ///< Contains the data for incoming packets

    typedef struct PACKETIZER_CONFIG {
        Data_Channel which_channel; ///< The data channel to packetize for
        uint8 control_byte; ///< The control byte to use for the packet
        void (*callback); ///< ????????
        UART_Config uart_config; ///< Configuration for a UART channel
    }Packetizer_Config; ///< The configuration struct for the Packetizer

    /**
     * Initializes the Data_Channel and the packetizer
     * \param config The configuration to set for the packetizer
     */
    void initialize_packetizer(Packetizer_Config config);

    /**
     * Sends a packet along the Data_Channel
     * \param which_channel The channel to send data on
     * \param data The data to send
     * \param data_size The size of the data
     */
    void send_packet(Data_Channel which_channel, uint8* data, uint8 data_size);

    /**
     * Checks if the channel has received a packet then triggers the appropriate action
     * \param which_channel The channel to watch in the process
     */
    void packetizer_background_process(Data_Channel which_channel);
#ifdef	__cplusplus
}
#endif

#endif	/* PACKETIZER_H */

