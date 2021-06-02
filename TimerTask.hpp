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
public:
    TimerTask(fp_t, fp_t , volatile uint32_t *, const char *, bool);
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
    uint16_t getCycleCount(void);
    bool isCycleComplete(void);
    void cycleReset(void);
    bool mainLoop(void);
};