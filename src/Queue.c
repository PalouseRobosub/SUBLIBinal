#include "Queue.h"


//Copyright 2015 Palouse RoboSub Club

/*
  This file is part of Sublibinal.

    Sublibinal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Sublibinal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Sublibinal.  If not, see <http://www.gnu.org/licenses/>.
 */


Queue create_queue(uint8* buffer, uint buffer_size) {
    Queue queue;

    queue.buffer = buffer;
    queue.buffer_size = buffer_size;

    queue.QueueEnd = 0;
    queue.QueueStart = 0;
    queue.numStored = 0;

    return queue;
}

Error enqueue(Queue* queue, uint8* data, uint data_size) {
    uint i, j;
    uint interrupt_state;

    interrupt_state = __builtin_get_isr_state();
    __builtin_disable_interrupts();

    //check to see if there is room in the queue for the data
    if ((queue->buffer_size - queue->numStored) < data_size) {
        __builtin_set_isr_state(interrupt_state);
        return ERR_INVALID_SEND;
    }

    //copy the memory
    for (i = 0, j = queue->QueueEnd; i < data_size; ++i, ++j) {
        if (j == (queue->buffer_size)) //if we have reached end of buffer
        {
            j = 0; //wrap the buffer back around
        }

        queue->buffer[j] = data[i];
    }

    //update queue information
    queue->QueueEnd = j;
    queue->numStored += data_size;

    __builtin_set_isr_state(interrupt_state);
    return 0;
}

Error dequeue(Queue* queue, uint8* output_data, uint data_size) {
    uint i, j;
    uint interrupt_state;

    interrupt_state = __builtin_get_isr_state();
    __builtin_disable_interrupts();

    //check if there is that much data in queue
    if (data_size > queue->numStored) {
        __builtin_set_isr_state(interrupt_state);
        return ERR_QUEUE_INVALID_READ;
    }

    //copy the memory
    for (i = 0, j = queue->QueueStart; i < data_size; ++i, ++j) {
        if (j == (queue->buffer_size)) //if we have reached end of buffer
        {
            j = 0; //wrap the buffer back around
        }

        output_data[i] = queue->buffer[j];
    }

    queue->QueueStart = j;
    queue->numStored -= data_size;

    __builtin_set_isr_state(interrupt_state);
    return 0;
}