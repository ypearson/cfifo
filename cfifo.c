#include "cfifo.h"

void cfifo_init(cfifo_t *cf)
{
    cf->sz   = CFIFO_SIZE;
    cf->head = cf->buf;
    cf->tail = 0;
    cf->cnt  = 0;

    for(ds_t i = 0; i < CFIFO_SIZE; i++)
    {
        cf->buf[i] = 0;
    }
}

uint8_t cfifo_put(cfifo_t * cf, ds_t *val)
{
    uint8_t ret = 0;
    ds_t   sz   = cf->sz;
    ds_t   cnt  = cf->cnt;
    ds_t * head = cf->head;
    ds_t * tail = cf->tail;
    ds_t * buf  = cf->buf;

    if(cnt < sz)
    {
        if(!head)
            cf->head = cf->buf;

        *cf->head = *val;

        if(head + 1 < buf + sz)
        {
            if(head + 1 == tail) // full case
                cf->head = 0;
            else
                cf->head++;
        }
        else if(tail == buf) // full wrap
            cf->head = 0;
        else
            cf->head = cf->buf; // wrap back to buffer

        if(!tail)
            cf->tail = head; // bug??

        cf->cnt++;
    }
    else
    {
        cf->head = 0;
        ret = 1;
    }

    return ret;
}

uint8_t cfifo_get(cfifo_t *cf, ds_t *val)
{
    uint8_t ret = 1;
    ds_t   sz   = cf->sz;
    ds_t   cnt  = cf->cnt;
    ds_t * head = cf->head;
    ds_t * tail = cf->tail;
    ds_t * buf  = cf->buf;

    if(cnt)
    {
        *val = *cf->tail;
        if(tail + 1 < buf + sz)
        {
            if(tail + 1 == head)
                cf->tail = 0;
            else
                cf->tail++;
        }
        else if (head == buf)
            cf->tail = 0;
        else
            cf->tail = buf;
        if(!head)
            cf->head = tail;

        cf->cnt--;
        ret = 0;
    }
    else
    {
        cf->tail = 0;
        ret = 1;
    }

    return ret;
}

// ds_t cfifo_peek(cfifo_t *cf, ds_t *val)
// {
//     uint8_t ret = 0;
//     ds_t cnt = cf->cnt;

//     if(cnt)
//     {
//         *val = *cf->tail;
//     }
//     else
//     {
//         ret = 1;
//     }

//     return ret;
// }

ds_t cfifo_cnt(cfifo_t *cf)
{
    return cf->cnt;
}

// uint8_t cfifo_copy(cfifo_t *cf_src, cfifo_t *cf_dst)
// {
//     ds_t head, tail;

//     for (uint8_t i = 0; i < CFIFO_SIZE; i++)
//     {
//         cf_dst->buf[i] = cf_src->buf[i];
//     }

//     cf_dst->cnt = cf_src->cnt;

//     if(!cf_src->head)
//     {
//         cf_dst->head = 0;
//     }
//     else
//     {
//         head = cf_src->head - cf_src->buf;
//         cf_dst->head = cf_dst->buf + head;
//     }
//     if(!cf_src->tail)
//     {
//         cf_dst->tail= 0;
//     }
//     else
//     {
//         tail = cf_src->tail - cf_src->buf;
//         cf_dst->tail = cf_dst->buf + tail;
//     }

//     return 0;
// }
