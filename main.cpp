#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <cstdint>

#include "cfifo.hpp"

#define SLEEP rand()%500000+50000

pthread_mutex_t lock;

uint8_t buffer[7];
Cfifo<uint8_t> cfifo(buffer, 7);

uint8_t g_value = 0;

void *myThread1(void *vargp)
{
    for (int i = 0; i < 25; ++i)
    {
        usleep(SLEEP);
        if(!cfifo.put(&g_value))
        {
            printf("\033[0;31m1.Put %u\n", (int)g_value);
            pthread_mutex_lock(&lock);
            g_value++;
            g_value %= 10;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            printf("\033[0;31m1.Full!\n");
        }
    }
    return 0;
}


void *myThread2(void *vargp)
{
    for (int i = 0; i < 25; ++i)
    {
        usleep(SLEEP);
        uint8_t val = 0;
        if(!cfifo.get(&val))
        {
            printf("\033[0;32m2.Get %d\n",(int)val);
        }
        else
        {
            printf("\033[0;32m2.Empty!\n");
        }
    }
    return 0;
}

void *myThread3(void *vargp)
{
    for (int i = 0; i < 25; ++i)
    {
        usleep(SLEEP);
        if(!cfifo.put(&g_value))
        {
            printf("\033[0;31m3.Put %u\n", (int)g_value);
            pthread_mutex_lock(&lock);
            g_value++;
            g_value %= 10;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            printf("\033[0;31m3.Full!\n");
        }
    }
    return 0;
}


void *myThread4(void *vargp)
{
    for (int i = 0; i < 25; ++i)
    {
        usleep(SLEEP);
        uint8_t val = 0;
        if(!cfifo.get(&val))
        {
            printf("\033[0;32m4.Get %d\n",(int)val);
        }
        else
        {
            printf("\033[0;32m4.Empty!\n");
        }
    }
    return 0;
}


int main() {


    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }



    pthread_t thread_id;
    pthread_create(&thread_id, NULL, myThread1, NULL);
    pthread_create(&thread_id, NULL, myThread2, NULL);
    pthread_create(&thread_id, NULL, myThread3, NULL);
    pthread_create(&thread_id, NULL, myThread4, NULL);


    pthread_join(thread_id, NULL);
    printf("After Thread\n");

}

