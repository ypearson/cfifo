#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <cstdint>

#define LOOPS 10000000
#define SLEEP2 rand()%5000+500
#define SLEEP1 rand()%10000+500

#include "circularBuffer.hpp"

uint32_t buffer[8];
CircularBuffer cb{buffer,8};
uint32_t g_value = 0;
uint8_t lock = 0;

void *threadA(void *vargp)
{
    (void)vargp;
    for (uint32_t i = 0; i < LOOPS; ++i)
    {
        usleep(SLEEP1);
        if(!cb.write(g_value))
        {
            // printf("\033[0;31m1.Put %u\n", (int)g_value);
            g_value++;
            g_value %= 100;
            if(lock>5)
                for(;;);
        }
        else
        {
            // printf("\033[0;31m1.Full!\n");
        }

        if(i % 100 == 0)
        {
            printf("%s","W");
            fflush(stdout);
        }
    }
    return 0;
}

void *threadB(void *vargp)
{
    (void)vargp;
    for (uint32_t i = 0; i < LOOPS; ++i)
    {
        usleep(SLEEP2);
        uint32_t val = 0;
        static uint32_t cval = 0;
        if(!cb.read(&val))
        {
            // printf("\033[0;32m2.Get %d\n",(int)val);

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
            // printf("\033[0;32m2.Empty!\n");
        }

        if(i % 100 == 0)
        {
            printf("%s","R");
            fflush(stdout);
        }

    }
    return 0;
}

int main() {

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, threadA, NULL);
    pthread_create(&thread_id, NULL, threadB, NULL);
    pthread_join(thread_id, NULL);

    printf("Done!\n");


}

