#include "TaskRunner.hpp"

namespace app::os
{
    TaskRunner::TaskRunner(TimerTask *const timerTasks, uint8_t len) : timerTasks(timerTasks), timerTasksLen(len)
    {
    }
    bool TaskRunner::run(uint8_t *taskId)
    {
        bool executed_task = false;
        app::os::TimerTask *p_tasks = timerTasks;

        for (uint8_t task = 0; task < timerTasksLen; task++)
        {
            if (p_tasks->scheduler() == true)
            {
                if (taskId)
                {
                    *taskId = task;
                }
                executed_task = true;
                break;
            }
            p_tasks++;
        }
        return executed_task;
    }
    uint8_t TaskRunner::start(uint8_t taskId)
    {
        uint8_t err = 1;
        if (taskId < timerTasksLen)
        {
            timerTasks[taskId].start();
            err = 0;
        }
        return err;
    }
    uint8_t TaskRunner::stop(uint8_t taskId)
    {
        uint8_t err = 1;
        if (taskId < timerTasksLen)
        {
            timerTasks[taskId].stop();
            err = 0;
        }
        return err;
    }
    bool TaskRunner::isRunning(uint8_t taskId)
    {
        bool running = false;
        if (taskId < timerTasksLen)
        {
            running = !timerTasks[taskId].isDone() && timerTasks[taskId].isEnabled();
        }
        return running;
    }
    const char *TaskRunner::getDescription(uint8_t taskId)
    {
        if (taskId < timerTasksLen)
        {
            return timerTasks[taskId].getDescription();
        }
        else
        {
            return "No Task";
        }
    }
}