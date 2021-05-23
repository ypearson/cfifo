#ifndef _SW_TIMER_H_
#define _SW_TIMER_H_

#include <stdint.h>

#define SW_TIMER_SLOTS (8)

typedef void (*fp_t)(void);

typedef struct
{
    fp_t     task_pri;
    fp_t     task_sec;
    uint16_t pri_max;
    uint16_t pri_period;
    uint16_t sec_period;
    uint8_t  slot;
} sw_timer_t;

typedef struct
{
    uint16_t ref_time;
    uint16_t pri_cnt;
    uint8_t  sec_start;
    uint8_t  enable;
    uint8_t  only_once;
    // sw_timer_t **
} _sw_timer_t;

uint8_t sw_timer_get_task_id(uint8_t slot);
uint8_t sw_timer_get_slot(uint8_t task_id);
uint8_t sw_timer_acquire_next_slot(void);
uint8_t sw_timer_acquire_slot(uint8_t slot);
void    sw_timer_release_slot(uint8_t slot);
void    sw_timer_init(void);
uint8_t sw_timer_create(sw_timer_t *swt, uint8_t slot, uint8_t only_once);
void    sw_timer_start(uint8_t slot);
void    sw_timer_restart(uint8_t slot);
void    sw_timer_stop(uint8_t slot);
uint8_t sw_timer_is_running(uint8_t slot);
uint8_t sw_timer_last_run(uint8_t slot);
void    sw_timer_restart(uint8_t slot);
void    sw_timer_tick(void);
void    sw_timer_fnc(fp_t fp);
void    sw_timer_run(void);

#endif