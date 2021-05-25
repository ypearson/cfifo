#pragma once
#include <cstdint>


template <class T>
class Cfifo {
private:
    T *buffer;
    T *head;
    T *tail;
    uint16_t  size;
    uint16_t  cnt;
public:
    Cfifo(T *buffer, uint16_t size); 
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
    head = array;
    size = sz;
    tail = 0;
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
        *head = *val;
        T *phead = head;

        if(head + 1 < buffer + size)
        {
            if(head + 1 == tail) // full
                head = 0;
            else
                head++;
        }
        else if(tail == buffer)
            head = 0;
        else
            head = buffer;
        if(!tail)
            tail = phead;

        cnt++;
    }
    else
    {
        head = 0;
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
        *val = *tail;
        T *ptail = tail;

        if(tail + 1 < buffer + size)
        {
            if(tail + 1 == head) // empty
                tail = 0;
            else
                tail++;
        }
        else if (head == buffer)
            tail = 0;
        else
            tail = buffer;
        if(!head)
            head = ptail;

        cnt--;
    }
    else
    {
        tail = 0;
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
        *val = *tail;
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



