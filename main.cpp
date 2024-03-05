/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include <cstdint>

typedef struct {
    uint32_t    voltage;   /* AD result of measured voltage */
    uint32_t    current;   /* AD result of measured current */
    uint32_t counter;   /* A counter value               */
} message_t;

MemoryPool<message_t, 16> mpool;
Queue<message_t, 16> queue;
Thread hilo_1;
Thread hilo_2;
Thread hilo_3;


void send_thread_volt(void);
void send_thread_current(void);
void send_thread_counter(void);


int main(void)
{
    hilo_1.start(send_thread_volt);
    hilo_1.start(send_thread_current);
    hilo_1.start(send_thread_counter);


    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t *)evt.value.p;
            printf("\nVoltage: %u V\n\r", message->voltage);
            printf("Current: %u A\n\r", message->current);
            printf("Number of cycles: %u\n\r", message->counter);

            mpool.free(message);
        }
    }
}


/* Send Thread  volt*/
void send_thread_volt(void)
{
   
    uint32_t i = 0;
    while (true) {
        i++;
        message_t *message = mpool.alloc();
        message->voltage = (i * 3);
        queue.put(message);
        ThisThread::sleep_for(1000ms);
    }
}


/* Send Thread  volt*/
void send_thread_current(void)
{
   
    uint32_t i = 0;
    while (true) {
        i++;
        message_t *message = mpool.alloc();
        message->current = (i*33);
        queue.put(message);
        ThisThread::sleep_for(1000ms);
    }
}

/* Send Thread  volt*/
void send_thread_counter(void)
{
    uint32_t i = 0;
    
    while (true) {
        i++;
        message_t *message = mpool.alloc();
        message-> counter = i;
        queue.put(message);
        ThisThread::sleep_for(1000ms);
    }
}
