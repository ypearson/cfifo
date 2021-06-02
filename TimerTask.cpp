#include "TimerTask.hpp"

TimerTask::TimerTask(fp_t primary, fp_t secondary, volatile uint32_t *timer, const char *description, bool one_shot) : primary(primary), secondary(secondary), one_shot(false),
                                                                                                              period_primary(0), period_secondary(0), cycle_counter(0),
                                                                                                              cycle_max(0), description(description), secondary_started(false),
                                                                                                              timer(timer)
{
}

void TimerTask::setPeriod(uint16_t prim, uint16_t sec)
{
    period_primary = prim;
    period_secondary = sec;
}
uint16_t TimerTask::getPrimaryPeriod(void)
{
    return period_primary;
}

uint16_t TimerTask::getSecondaryPeriod(void)
{
    return period_secondary;
}

void TimerTask::enable_task(void)
{
    enabled = true;
}
void TimerTask::disable_task(void)
{
    enabled = false;
}
bool TimerTask::is_enabled(void)
{
    return enabled;
}
void TimerTask::setReferenceTime(uint32_t time)
{
    ref_time = time;
}
uint32_t TimerTask::getReferenceTime(void)
{
    return ref_time;
}
const char *TimerTask::getDescription(void)
{
    return description;
}
void TimerTask::setSecondary(bool sec)
{
    secondary_started = sec;
}
bool TimerTask::didSecondaryStart(void)
{
    return secondary_started;
}
void TimerTask::execute_primary(void)
{
    if (primary)
    {
        primary();
        cycle_counter++;
    }
}

void TimerTask::execute_secondary(void)
{
    if (secondary)
    {
        secondary();
    }
}
uint16_t TimerTask::getCycleCount(void)
{
    return cycle_counter;
}
bool TimerTask::isCycleComplete(void)
{
    return (cycle_counter >= cycle_max) && (cycle_max > 0U);
}
void TimerTask::cycleReset(void)
{
    cycle_counter = 0U;
}
bool TimerTask::mainLoop(void)
{
    if (enabled)
    {
        if (!secondary_started)
        {
            uint32_t trig_time = ref_time + period_primary;
            bool period_elapsed = (trig_time > ref_time) && (*timer >= trig_time);
            bool period_elasped_rollover = (*timer < ref_time) && (*timer >= trig_time);
            period_elapsed |= period_elasped_rollover;

            if (period_elapsed)
            {
                execute_primary();
                ref_time = *timer;

                if (cycle_counter >= cycle_max && cycle_max && !period_secondary)
                {
                    cycle_counter = 0;
                    enabled = 0;
                }

                if (period_secondary)
                {
                    secondary_started = 1;
                }
            }
        }
        else
        {
            uint32_t trig_time = ref_time + period_secondary;
            bool period_elapsed = (trig_time > ref_time) && (*timer >= trig_time);
            bool period_elasped_rollover = (*timer < ref_time) && (*timer >= trig_time);
            period_elapsed |= period_elasped_rollover;

            if (period_elapsed)
            {
                execute_secondary();
                ref_time = *timer;
                secondary_started = 0;

                if (cycle_counter >= cycle_max && cycle_max)
                {
                    cycle_counter = 0;
                    enabled = 0;
                }
            }
        }
    }
    else
    {
        ref_time = *timer;
    }
    return true;
}
