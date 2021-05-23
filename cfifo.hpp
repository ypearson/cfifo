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


// delete copy constructor
