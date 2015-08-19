/*!
 * \file Queue.h
 *
 * A Queue pseudo-class
 */

#ifndef QUEUE_H
#define	QUEUE_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"

    /*Structure Defintions*/
    typedef struct QUEUE {
        uint8 *buffer; ///< pointer to the queue memory
        uint buffer_size; ///< size of the supplied buffer
        uint QueueStart; ///< location of first data point (start of queue)
        uint QueueEnd; ///< location of the last data point (end of queue)
        uint numStored; ///< amount of data within the queue
    } Queue; ///< Stores the info required to implement a queue


    /*Queue Functions*/
    /**
     * Creates a Queue from an array and the array's size
     * \param buffer The array to store the Queue in
     * \param buffer_size The size of the Queue to create
     * \return A complete Queue
     */
    Queue create_queue(uint8* buffer, uint buffer_size);

    /**
     * Adds data to the specified Queue
     * \param queue The Queue struct to enqueue to
     * \param[in] data the data to enqueue
     * \param data_size The size of the data to enqueue
     * \return An Error type
     * \see Error
     */
    Error enqueue(Queue* queue, uint8* data, uint data_size);

    /**
     * Removes data from the specified Queue
     * \param queue The Queue to dequeue from
     * \param[out] output_data The location to store the dequeued data
     * \param data_size The amount of data to dequeue
     * \return An Error type
     * \see Error
     */
    Error dequeue(Queue* queue, uint8* output_data, uint data_size);



#ifdef	__cplusplus
}
#endif

#endif	/* QUEUE_H */

