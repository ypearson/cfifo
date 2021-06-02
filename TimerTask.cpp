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
const char *TimerTask::getDescription(void)
{
    return description;
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
bool TimerTask::periodElapsed(uint32_t period)
{
    uint32_t trig_time = ref_time + period;
    bool period_elapsed = (trig_time > ref_time) && (*timer >= trig_time);
    bool period_elasped_rollover = (*timer < ref_time) && (*timer >= trig_time);
    period_elapsed |= period_elasped_rollover;

    return period_elapsed;
}
bool TimerTask::mainLoop(void)
{
    if (enabled)
    {
        if (!secondary_started)
        {
            if (periodElapsed(period_primary))
            {
                execute_primary();
                ref_time = *timer;

                if (isCycleComplete() && !period_secondary)
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
            if (periodElapsed(period_secondary))
            {
                execute_secondary();
                ref_time = *timer;
                secondary_started = 0;

                if (isCycleComplete())
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
