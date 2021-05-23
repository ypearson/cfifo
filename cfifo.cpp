#include <cstdio>
#include "cfifo.hpp"

template <class T>
Cfifo<T>::Cfifo(T *array, uint16_t sz) {

    buffer = array;
    head = array;
    size = sz;
    tail = 0;
    cnt  = 0;

    for(uint16_t i = 0; i < size; i++)
    {
        buffer[i] = 0;
    }
}

// head == 0 -> FULL
// tail == 0 -> EMPTY
// ASSERTS

template <class T>
Cfifo<T>::~Cfifo() {}

template <class T>
uint8_t Cfifo<T>::put(T *val)
{
    uint8_t ret = 0;

    if(cnt < size)
    {
        if(!head)
            head = buffer;

        *head = *val;

        if(head + 1 < buffer + size)
        {
            if(head + 1 == tail)
                head = 0;
            else
                head++;
        }
        else if(tail == buffer)
            head = 0;
        else
            head = buffer;

        if(!tail)
            tail = buffer;

        cnt++;
    }
    else
    {
        head = 0;
        ret = 1;
    }

    return ret;
}

template <class T>
uint8_t Cfifo<T>::get(T *val)
{
    uint8_t ret = 1;

    if(cnt)
    {
        *val = *tail;
        if(tail + 1 < buffer + size)
        {
            if(tail + 1 == head)
                tail = 0;
            else
                tail++;
        }
        else if (head == buffer)
            tail = 0;
        else
            tail = buffer;
        if(!head)
            head = tail;

        cnt--;
        ret = 0;
    }
    else
    {
        tail = 0;
        ret = 1;
    }

    return ret;
}

template <class T>
uint8_t Cfifo<T>::peek(T *val)
{
    uint8_t ret = 0;

    if(cnt)
    {
        *val = *tail;
    }
    else
    {
        ret = 1;
    }

    return ret;
}

template <class T>
uint16_t Cfifo<T>::count(void)
{
    return cnt;
}


