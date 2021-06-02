#pragma once

#include <cstdint>

typedef void (*fp_t)(void);

class TimerTask
{
private:
    fp_t primary;
    fp_t secondary;
    volatile uint32_t *timer;
    uint16_t period_primary;
    uint16_t period_secondary;
    uint16_t cycle_counter;
    uint16_t cycle_max;
    uint32_t ref_time;
    const char *description;
    bool enabled;
    bool one_shot;
    bool secondary_started;
    void execute_primary(void);
    void execute_secondary(void);
    bool isCycleComplete(void);
    bool periodElapsed(uint32_t period);

public:
    TimerTask(fp_t, fp_t, volatile uint32_t *, const char *, bool);
    void setPeriod(uint16_t prim, uint16_t sec);
    void enable_task(void);
    void disable_task(void);
    bool is_enabled(void);
    const char *getDescription(void);
    uint16_t getCycleCount(void);
    bool mainLoop(void);
};