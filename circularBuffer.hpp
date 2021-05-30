#pragma once
#include <cstdint>
#include <atomic>
class CircularBuffer
{
private:
    uint32_t *buffer;
    uint16_t size; //size = 2^n
    volatile std::atomic<std::uint32_t> _read;
    volatile std::atomic<std::uint32_t> _write;
public:
    CircularBuffer(uint32_t *buffer, uint16_t size);
    ~CircularBuffer();
    uint16_t length(void);
    uint8_t write(uint32_t data);
    uint8_t read(uint32_t *data);
    uint8_t CircularBuffer::peek(uint32_t &data);
};
