#include "TimerTask.hpp"

namespace app::os
{
    TimerTask::TimerTask(task_list_t *task_list, uint16_t cycles, const char *description) :
        task_list(task_list), cycle_max(cycles), description(description), enabled(false)
    {
    }
    void TimerTask::start(void)
    {
        enabled = true;
        ready_to_executed_task = true;
        cycle_counter = 0;
        subtask_id = 0;
        setTEqualsZero();
    }
    void TimerTask::stop(void)
    {
        enabled = false;
    }
    bool TimerTask::isEnabled(void)
    {
        return enabled;
    }
    const char *TimerTask::getDescription(void)
    {
        return description;
    }
    uint8_t TimerTask::setTEqualsZero(void)
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
    bool TimerTask::isDone(void)
    {
        return (cycle_counter >= cycle_max) && (cycle_max > 0U);
    }
    bool TimerTask::periodElapsed(uint32_t period)
    {
        uint32_t trig_time = ref_time + period;
        uint32_t now = *timer;
        bool period_elapsed = (trig_time > ref_time) && (now >= trig_time);
        bool period_elasped_rollover = (now < ref_time) && (now >= trig_time);
        period_elapsed |= period_elasped_rollover;
        return period_elapsed;
    }
    bool TimerTask::scheduler(void)
    {
        bool executed = false;
        if (enabled)
        {
            if (subtask_id < task_list->len)
            {
                if (ready_to_executed_task == true)
                {
                    task_list->executables[subtask_id]();
                    setTEqualsZero();
                    ready_to_executed_task = false;
                    executed = true;
                }
                else if (periodElapsed(task_list->periods[subtask_id]))
                {
                    ready_to_executed_task = true;
                    subtask_id++;
                }
            }
            else
            {
                cycle_counter++;
                subtask_id = 0;
                setTEqualsZero();
                if (isDone() == true)
                {
                    enabled = false;
                }
            }
        }
        return executed;
    }
}