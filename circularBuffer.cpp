#include "circularBuffer.hpp"

CircularBuffer::CircularBuffer(uint32_t *buffer, uint16_t size):
buffer(buffer),size(size),_read(0),_write(0)
{
}
CircularBuffer::~CircularBuffer(){
    buffer = 0;
}

uint16_t CircularBuffer::length(void){
    return (_write-_read) & (size - 1);
}

uint8_t CircularBuffer::read(uint32_t *data) {
    if(length() == 0)
        return 1;
    *data = buffer[_read];
    _read.store((_read.load()+1) & (size-1));
    return 0;
}

uint8_t CircularBuffer::write(uint32_t data) {
    if(length() == (size-1))
        return 1;
    buffer[_write] = data;
    _write.store((_write.load()+1) & (size-1));
    return 0;
}

uint8_t CircularBuffer::peek(uint32_t &data) {
    if(length() == 0)
        return 1;
    *data = buffer[_read];
}