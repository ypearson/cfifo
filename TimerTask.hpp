#pragma once

#include <cstdint>
namespace app::os
{
    static constexpr uint8_t task_list_len = 8;
    typedef void (*fp_t)(void);
    typedef struct
    {
        fp_t executables[task_list_len];
        uint32_t periods[task_list_len];
        uint8_t len = task_list_len;
    } task_list_t;

    class TimerTask
    {
    private:
        task_list_t *const task_list;
        uint8_t subtask_id;
        uint16_t cycle_counter;
        uint32_t ref_time;
        const uint16_t cycle_max;
        const char *description;
        bool enabled;
        bool periodElapsed(uint32_t period);
        uint8_t set_t_equals_zero(void);

    public:
        TimerTask(task_list_t *task_list, uint16_t cycles, const char *description);
        static volatile uint32_t *const timer;
        void start(void);
        void stop(void);
        bool is_enabled(void);
        bool isSequenceComplete(void);
        const char *getDescription(void);
        uint16_t getCycleCount(void);
        bool scheduler(void);
    };
}