#ifndef _CFIFO_H
#define _CFIFO_H

#include <stdint.h>

typedef uint16_t ds_t;

typedef struct
{
    #define   CFIFO_SIZE (24-1)
    // void   *buf;
    uint16_t  buf[CFIFO_SIZE];
    uint16_t *head;
    uint16_t *tail;
    uint16_t  sz;
    uint16_t  cnt;
} cfifo_t;

void    cfifo_init(cfifo_t *cf);
void    cfifo_init_(cfifo_t *cf, uint8_t sz);
uint8_t cfifo_put (cfifo_t *cf, ds_t *val);
uint8_t cfifo_get (cfifo_t *cf, ds_t *val);
ds_t    cfifo_peek(cfifo_t *cf, ds_t *val);
ds_t    cfifo_cnt (cfifo_t *cf);
uint8_t cfifo_copy(cfifo_t *cf_src, cfifo_t *cf_dst);

#endif
