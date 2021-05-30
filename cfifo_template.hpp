#pragma once
#include <cstdint>
#include <cstdio>
#include <atomic>

template <class T>
class Cfifo {
private:
    T *buffer;
    T *write;
    T *read;
    uint16_t  size;
    uint16_t  cnt;
public:
    Cfifo(T *array, uint16_t sz);
    ~Cfifo();
    uint8_t put(T *val);
    uint8_t put(T val);
    uint8_t get(T *val);
    uint8_t peek(T *val);
    uint16_t count(void);
    uint8_t is_empty(void);
};

template <class T>
Cfifo<T>::Cfifo(T *array, uint16_t sz) {
    buffer = array;
    write = array;
    size = sz;
    read = 0;
    cnt  = 0;

    for(uint16_t i = 0; i < size; i++)
    {
        buffer[i] = 0;
    }
}

template <class T>
Cfifo<T>::~Cfifo() {}

template <class T>
uint8_t Cfifo<T>::put(T *val)
{
    uint8_t err = 0;

    if(cnt < size)
    {
        *write = *val;
        T *pwrite = write;

        if(write + 1 < buffer + size)
        {
            if(write + 1 == read) // full
                {write = 0;
                 printf("\033[0;31m1.W[%lx]\n", 0);}

            else{
                write++;
                printf("\033[0;31m1.W[%lx]\n", write-buffer);
            }
        }
        else if(read == buffer)
            write = 0;
        else
            write = buffer;
        if(!read)
            read = pwrite;

        cnt++;
        // printf("\033[0;31m1.W cnt = %d\n", cnt);
    }
    else
    {
        write = 0;
        err = 1;
    }
    return err;
}

template <class T>
uint8_t Cfifo<T>::get(T *val)
{
    uint8_t err = 0;

    if(cnt)
    {
        *val = *read;
        T *pread = read;

        if(read + 1 < buffer + size)
        {
            if(read + 1 == write) // empty
               { read = 0;
                printf("\033[0;32m2.R[%lx]\n", 0);
            }
            else{
                read++;
                printf("\033[0;32m2.R[%lx]\n", read-buffer);

            }
        }
        else if (write == buffer)
            read = 0;
        else
            read = buffer;
        if(!write)
            write = pread;

        cnt--;
        // printf("\033[0;32m2.R cnt = %d\n", cnt);

    }
    else
    {
        read = 0;
        err = 1;
    }

    return err;
}

template <class T>
uint8_t Cfifo<T>::peek(T *val)
{
    uint8_t err = 0;

    if(cnt)
    {
        *val = *read;
    }
    else
    {
        err = 1;
    }

    return err;
}

template <class T>
uint16_t Cfifo<T>::count(void)
{
    return cnt;
}



