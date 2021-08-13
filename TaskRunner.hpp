#pragma once

#include "TimerTask.hpp"

namespace app::os
{
    class TaskRunner
    {
    private:
        TimerTask *const timerTasks;
        uint8_t timerTasksLen;

    public:
        TaskRunner(TimerTask *const timerTasks, uint8_t len);
        bool run(uint8_t *taskId);
        uint8_t start(uint8_t taskId);
        uint8_t stop(uint8_t taskId);
        bool isRunning(uint8_t taskId);
        const char *getDescription(uint8_t taskId);
    };
}