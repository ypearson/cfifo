#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <cstdint>

#include "cfifo.hpp"

uint8_t buffer[5];
Cfifo<uint8_t> cfifo(buffer, 5);

void *myThread1(void *vargp)
{
    for (int i = 0; i < 25; ++i)
    {
        usleep(rand()%1000000+500);
        static uint8_t val = 0;
        val %=10;
        if(!cfifo.put(&val))
        {
            printf("\033[0;31mPut %u\n", (int)val);
            val++;
        }
        else
        {
            printf("\033[0;31mFull!\n");
        }
    }
    return 0;
}


void *myThread2(void *vargp)
{
    for (int i = 0; i < 25; ++i)
    {
        usleep(rand()%1000000+50000);
        uint8_t val = 0;
        if(!cfifo.get(&val))
        {
            printf("\033[0;32mGet %d\n",(int)val);
        }
        else
        {
            printf("\033[0;32mEmpty!\n");
        }
    }
    return 0;
}

int main() {



    pthread_t thread_id;
    pthread_create(&thread_id, NULL, myThread1, NULL);
    pthread_create(&thread_id, NULL, myThread2, NULL);
    pthread_create(&thread_id, NULL, myThread1, NULL);
    pthread_create(&thread_id, NULL, myThread2, NULL);


    pthread_join(thread_id, NULL);
    printf("After Thread\n");

}

