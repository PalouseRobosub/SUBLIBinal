/*! \file ADC.h
 * Defines the data structures and methods for using the ADC
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"
#include "Queue.h"


    /* Enums */
    /** \enum ADC_Channel Set of all possible ADC channels */
    typedef enum {
        ADC_CH_0,
        ADC_CH_1,
        ADC_CH_2,
        ADC_CH_3,
        ADC_CH_4,
        ADC_CH_5,
        ADC_CH_9 = 9,
        ADC_CH_10,
        ADC_CH_11
    } ADC_Channel;


    /*Object Defintions*/
    typedef struct ADC_DATA {
        Queue Results_queue;///< Queue of resulting ADC reads
        Queue Work_queue;///< Queued ADC reads to be done
        boolean is_idle;///< Idle information
    }ADC_Data; ///< Object for references to the data queues


    typedef struct ADC_NODE {
        uint8 device_id;///< a unique identifier for the device
        ADC_Channel channel;///< ADC channel
        uint16 data;///< location to store data
        void (*callback) (struct ADC_NODE);///< callback function
    } ADC_Node;///< ADC data node


    typedef struct ADC_CONFIG {
        uint16 channels;///< The bit field of the channels to configure
        uint8 *work_buffer_ptr;///< Pointer to the ADC's work buffer
        uint work_buffer_size;///< The size of the ADC's work buffer
        uint8 *result_buffer_ptr;///< Pointer to the ADC's result buffer
        uint result_buffer_size;///< The size of the ADC's result buffer
        void* callback;///< The function to call when done sampling \note <strong>default:</strong> NULL
    } ADC_Config;///< The configuration struct for the ADC


    /*Function Prototypes*/
    /**
     * Initializes the ADC
     * \param config Configuration information for the ADC
     * \return A pointer to the results and the work queue for the ADC
     * \see ADC_CONFIG
     */
    ADC_Data* initialize_ADC(ADC_Config config);

    /**
     * Sets up the ADC to read
     * \param node Specifies parameters required for an ADC read
     * \return An Error type
     * \see Error
     * \see ADC_NODE
     */
    int read_ADC(ADC_Node node);

    /**
     * Run this background process in the main while loop to
     * process the results of ADC samples
     */
    void bg_process_ADC(void);
    
    /**
     * Configures the necessary pins as analog pins
     * \param channels Bit mask to define which ADC channels to setup
     * \note channels = (1 << ADC_CH_1) | (1 << ADC_CH_4);
     */
    void setup_ADC_pins(uint16 channels);



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

