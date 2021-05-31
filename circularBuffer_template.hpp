#pragma once
#include <cstdint>
#include <atomic>

template <class T>
class CircularBuffer
{
private:
    T *buffer;
    uint16_t size; //size = 2^n
    volatile std::atomic<std::uint32_t> _read;
    volatile std::atomic<std::uint32_t> _write;
public:
    CircularBuffer(T *buf, uint16_t sz) {
        buffer = buf;
        size = sz;
        _read = 0;
        _write = 0;
    }
    ~CircularBuffer() {}
    uint16_t length(void) {
        return (_write-_read) & (size - 1);
    }
    uint8_t write(T data) {
        if(length() == (size-1))
            return 1;
        buffer[_write] = data;
        _write.store((_write.load()+1) & (size-1));
        return 0;
    }
    uint8_t read(T *data) {
        if(length() == 0)
            return 1;
        *data = buffer[_read];
        _read.store((_read.load()+1) & (size-1));
        return 0;
    }
    uint8_t peek(T *data) {
        if(length() == 0)
            return 1;
        *data = buffer[_read];
        return 0;
    }
};