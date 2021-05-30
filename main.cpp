#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <cstdint>

#include "cfifo_template.hpp"

#define SLEEP2 rand()%5000+500
#define SLEEP1 rand()%5000+500

uint8_t lock = 0;
uint8_t buffer[8];
Cfifo<uint8_t> cfifo(buffer, 7);

uint8_t g_value = 0;

#define LOOPS 1000000

void *thread1(void *vargp)
{
    for (uint32_t i = 0; i < LOOPS; ++i)
    {
        usleep(SLEEP1);
        if(!cfifo.put(&g_value))
        {
            printf("\033[0;31m1.Put %u\n", (int)g_value);
            g_value++;
            g_value %= 100;
            if(lock>5)
                for(;;);
        }
        else
        {
            printf("\033[0;31m1.Full!\n");
        }
    }
    return 0;
}


void *thread2(void *vargp)
{
    for (uint32_t i = 0; i < LOOPS; ++i)
    {
        usleep(SLEEP2);
        uint8_t val = 0;
        static uint8_t cval = 0;
        if(!cfifo.get(&val))
        {
            printf("\033[0;32m2.Get %d\n",(int)val);

            if(val != cval)
            {
                printf("****val = %d, cval = %d\n", val, cval);
                lock++;
                if(lock >5)
                    for(;;);
            }
            cval++;
            cval %= 100;

        }
        else
        {
            printf("\033[0;32m2.Empty!\n");
        }

    }
    return 0;
}

void *thread1(void *vargp)
{
    for (uint32_t i = 0; i < LOOPS; ++i)
    {
        usleep(SLEEP1);
        if(!cfifo.put(&g_value))
        {
            printf("\033[0;31m1.Put %u\n", (int)g_value);
            g_value++;
            g_value %= 100;
            if(lock>5)
                for(;;);
        }
        else
        {
            printf("\033[0;31m1.Full!\n");
        }
    }
    return 0;
}


void *thread2(void *vargp)
{
    for (uint32_t i = 0; i < LOOPS; ++i)
    {
        usleep(SLEEP2);
        uint8_t val = 0;
        static uint8_t cval = 0;
        if(!cfifo.get(&val))
        {
            printf("\033[0;32m2.Get %d\n",(int)val);

            if(val != cval)
            {
                printf("****val = %d, cval = %d\n", val, cval);
                lock++;
                if(lock >5)
                    for(;;);
            }
            cval++;
            cval %= 100;

        }
        else
        {
            printf("\033[0;32m2.Empty!\n");
        }

    }
    return 0;
}

struct
{
    uint8_t *buffer;
    uint16_t size; //size = 2^n
    uint16_t read;
    uint16_t write;
}cbuffer_t;

uint16_t cb_length(cbuffer_t *cb)
{
    return (cb->write-cb->read) & (cb->size - 1);
}

uint8_t cb_write(cbuffer_t *cb, uint8_t data)
{
    if(cb_length(&cb) == (cb->size-1))
        return 1;
    cb->buffer[cb->write] = data;
    cb->write = (cb->write+1) & (cb->size-1);
}
uint8_t cb_read(cbuffer_t *cb, uint8_t *data)
{
    if(cb_length(&cb) == (cb->size-1))
        return 1;
    *data = cb->buffer[cb->read];
    cb->read = (cb->read+1) & (cb->size-1);

}



int main() {

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread1, NULL);
    pthread_create(&thread_id, NULL, thread2, NULL);
    pthread_join(thread_id, NULL);

}

