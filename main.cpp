#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include "cfifo.hpp"


void *myThreadFun(void *vargp)
{
    sleep(1);
    printf("I'm a thread!\n");

    return 0;
}


int main() {

    uint8_t val = 10;
    uint8_t buffer[8];

    pthread_t thread_id;
    printf("Before Thread\n");
    pthread_create(&thread_id, NULL, myThreadFun, NULL);
    pthread_join(thread_id, NULL);
    printf("After Thread\n");


    template class Cfifo<uint8_t> cfifo(buffer, 8);
    cfifo.put(&val);

    val = 0;
    cfifo.get(&val);
    printf("%d\n", val);
}

