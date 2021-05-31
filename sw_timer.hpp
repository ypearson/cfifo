#pragma once

#include <stdint.h>

typedef void (*fp_t)(void);

class TimerTask
{
private:
    fp_t primary;
    fp_t secondary;
    uint16_t period_primary;
    uint16_t period_secondary;
    uint16_t cycle_counter;
    uint16_t cycle_max;
    uint32_t ref_time;
    const char *description;
    bool enabled;
    bool one_shot;
    bool secondary_started;

public:
    TimerTask(fp_t, fp_t, const char *, bool one_shot = false);
    void setPeriod(uint16_t, uint16_t);
    uint16_t getPrimaryPeriod(void);
    uint16_t getSecondaryPeriod(void);
    void enable_task(void);
    void disable_task(void);
    bool is_enabled(void);
    void setReferenceTime(uint32_t);
    uint32_t getReferenceTime(void);
    const char *getDescription(void);
    void setSecondary(bool);
    bool didSecondaryStart(void);
    void execute_primary(void);
    void execute_secondary(void);

        // typedef struct
        // {
        //     fp_t     task_pri;
        //     fp_t     task_sec;
        //     uint16_t pri_max;
        //     uint16_t pri_period;
        //     uint16_t sec_period;
        //     uint8_t  slot;
        // } sw_timer_t;

        // typedef struct
        // {
        //     uint16_t ref_time;
        //     uint16_t pri_cnt;
        //     uint8_t  sec_start;
        //     uint8_t  enable;
        //     uint8_t  one_shot;
        //     // sw_timer_t **
        // } _sw_timer_t;

        // uint8_t sw_timer_get_task_id(uint8_t slot);
        // uint8_t sw_timer_get_slot(uint8_t task_id);
        // uint8_t sw_timer_acquire_next_slot(void);
        // uint8_t sw_timer_acquire_slot(uint8_t slot);
        // void    sw_timer_release_slot(uint8_t slot);
        // void    sw_timer_init(void);
        // uint8_t sw_timer_create(sw_timer_t *swt, uint8_t slot, uint8_t one_shot);
        // void    sw_timer_start(uint8_t slot);
        // void    sw_timer_restart(uint8_t slot);
        // void    sw_timer_stop(uint8_t slot);
        // uint8_t sw_timer_is_running(uint8_t slot);
        // uint8_t sw_timer_last_run(uint8_t slot);
        // void    sw_timer_restart(uint8_t slot);
        // void    sw_timer_tick(void);
        // void    sw_timer_fnc(fp_t fp);
        // void    sw_timer_run(void);

        // #endif