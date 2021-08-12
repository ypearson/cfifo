#include "TimerTask.hpp"

namespace app::os
{

    TimerTask::TimerTask(task_list_t *task_list, uint16_t cycles, const char *description) : task_list(task_list), cycle_max(cycles), description(description), enabled(false)
    {
    }
    void TimerTask::start(void)
    {
        enabled = true;
        cycle_counter = 0;
        subtask_id = 0;
        set_t_equals_zero();
    }
    void TimerTask::stop(void)
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
    uint8_t TimerTask::set_t_equals_zero(void)
    {
        uint8_t err = 1;
        if (timer != nullptr)
        {
            ref_time = *timer;
            err = 0;
        }
        return err;
    }
    uint16_t TimerTask::getCycleCount(void)
    {
        return cycle_counter;
    }
    bool TimerTask::isSequenceComplete(void)
    {
        return (cycle_counter >= cycle_max) && (cycle_max > 0U);
    }
    bool TimerTask::periodElapsed(uint32_t period)
    {
        uint32_t trig_time = ref_time + period;
        uint32_t t = *timer;
        bool period_elapsed = (trig_time > ref_time) && (t >= trig_time);
        bool period_elasped_rollover = (t < ref_time) && (t >= trig_time);
        period_elapsed |= period_elasped_rollover;
        return period_elapsed;
    }
    bool TimerTask::scheduler(void)
    {
        bool executed_task = false;
        if (enabled)
        {
            if (subtask_id < task_list->len)
            {
                if (periodElapsed(task_list->periods[subtask_id]))
                {
                    if (task_list->executables[subtask_id] != nullptr)
                    {
                        task_list->executables[subtask_id]();
                        subtask_id++;
                        set_t_equals_zero();
                        executed_task = true;
                    }
                }
            }
            else
            {
                cycle_counter++;
                subtask_id = 0;
                set_t_equals_zero();
                if (isSequenceComplete() == true)
                {
                    enabled = false;
                }
            }
        }
        return executed_task;
    }
}