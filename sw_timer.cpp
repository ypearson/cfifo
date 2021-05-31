#include "sw_timer.hpp"
#include "circularBuffer_template.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <cstdint>

static volatile uint32_t m_timer;
// static  cfifo_t m_cfifo;
// static  sw_timer_t *m_swte[SW_TIMER_SLOTS];
// static _sw_timer_t  m_swti[SW_TIMER_SLOTS];
// static uint8_t m_slots[SW_TIMER_SLOTS] = {1,2,3,4,5,6,7,8};

TimerTask::TimerTask(fp_t primary, fp_t secondary, const char *description, bool one_shot) : primary(primary), secondary(secondary), one_shot(one_shot),
                                                                                             period_primary(0), period_secondary(0), cycle_counter(0),
                                                                                             cycle_max(0), description(description), secondary_started(false)
{
}

void TimerTask::setPeriod(uint16_t prim, uint16_t sec)
{
    period_primary = prim;
    period_secondary = sec;
}
uint16_t TimerTask::getSecondaryPeriod(void)
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
    if(primary)
    {
        primary();
        cycle_counter++;
    }
}

void TimerTask::execute_secondary(void)
{
    if(secondary)
    {
        secondary();
    }
}


void led_on(void)
{
    printf("LED ON\n");
}
void led_off(void)
{
    printf("LED OFF\n");
}

void sw_timer_init(void)
{
    m_timer = 0;
}

// uint8_t sw_timer_create(sw_timer_t *swt, uint8_t slot, uint8_t one_shot)
// {
//     uint16_t task_id;

//     if(!slot)
//     {
//         slot = sw_timer_acquire_next_slot();
//     }
//     else
//     {
//         slot = sw_timer_acquire_slot(slot);
//     }

//     if(!slot || !swt)
//         return 0;

//     task_id = sw_timer_get_task_id(slot);

//     cfifo_put(&m_cfifo, & task_id);

//     m_swte[task_id]           = swt;
//     m_swte[task_id]->slot     = slot;
//     m_swti[task_id].ref_time  = m_timer;
//     m_swti[task_id].enable    = 0;
//     m_swti[task_id].one_shot = one_shot;

//     return slot;
// }

// void sw_timer_start(uint8_t slot)
// {
//     uint8_t task_id = sw_timer_get_task_id(slot);

//     if(!sw_timer_acquire_slot(slot))
//     {
//         if(!m_swti[task_id].enable)
//         {
//             m_swti[task_id].enable    = 1;
//             m_swti[task_id].ref_time  = m_timer;
//             m_swti[task_id].pri_cnt   = 0;
//             m_swti[task_id].sec_start = 0;
//         }
//     }
// }

// void sw_timer_restart(uint8_t slot)
// {
//     uint8_t task_id = sw_timer_get_task_id(slot);

//     if(!sw_timer_acquire_slot(slot))
//     {
//         m_swti[task_id].enable    = 1;
//         m_swti[task_id].ref_time  = m_timer;
//         m_swti[task_id].pri_cnt   = 0;
//         m_swti[task_id].sec_start = 0;
//     }
// }

// void sw_timer_stop(uint8_t slot)
// {
//     uint8_t task_id = sw_timer_get_task_id(slot);

//     if(!sw_timer_acquire_slot(slot))
//     {
//         m_swti[task_id].enable = 0;
//     }
// }

// uint8_t sw_timer_is_running(uint8_t slot)
// {
//     uint8_t task_id = sw_timer_get_task_id(slot);
//     uint8_t is_enabled = m_swti[task_id].enable;
//     return is_enabled;
// }

// void sw_timer_restart(uint8_t slot)
// {
//     uint8_t task_id = sw_timer_get_task_id(slot);

//     m_swti[task_id].pri_cnt = 0;
// }

// uint8_t sw_timer_last_run(uint8_t slot)
// {
//     uint8_t ret = 0;
//     uint8_t task_id = sw_timer_get_task_id(slot);

//     if(m_swte[task_id]->pri_max)
//     {
//         if( m_swti[task_id].pri_cnt == (m_swte[task_id]->pri_max - 1))
//         {
//             ret = 1;
//         }
//     }

//     return ret;
// }

// void sw_timer_tick(void)
// {
//     m_timer++;
// }

// void sw_timer_fnc(fp_t fp)
// {
//     if(fp)
//     {
//         fp();
//     }
// }

// void sw_timer_run(void)
// {
//     #define PERIOD_ELAPSED           (trig_time > m_swti[task_id].ref_time && m_timer >= trig_time)
//     #define PERIOD_ELAPSED_ROLLOVER  (m_timer   < m_swti[task_id].ref_time && m_timer >= trig_time)
//     #define TASK_MAX_EXECUTIONS      (m_swti[task_id].pri_cnt >= m_swte[task_id]->pri_max && m_swte[task_id]->pri_max && !m_swte[task_id]->sec_period)
//     #define SUBTASK_MAX_EXECUTIONS   (m_swti[task_id].pri_cnt >= m_swte[task_id]->pri_max && m_swte[task_id]->pri_max)

//     uint16_t trig_time, task_id;
//     uint8_t  put_flag, task, slot;
//     uint8_t  active_tasks_num = cfifo_cnt(&m_cfifo);

//     for(task = 0; task < active_tasks_num; task++)
//     {
//         put_flag = 1;
//         cfifo_get(&m_cfifo, &task_id);

//         if(m_swti[task_id].enable)
//         {
//             trig_time = m_swti[task_id].ref_time + m_swte[task_id]->pri_period;

//             if( (PERIOD_ELAPSED||PERIOD_ELAPSED_ROLLOVER) && !m_swti[task_id].sec_start)
//             {
//                 sw_timer_fnc(m_swte[task_id]->task_pri);
//                 m_swti[task_id].ref_time = m_timer;
//                 m_swti[task_id].pri_cnt++;

//                 if(TASK_MAX_EXECUTIONS)
//                 {
//                     put_flag = 0;
//                     m_swti[task_id].pri_cnt = 0;
//                     m_swti[task_id].enable = 0;
//                 }

//                 if(m_swte[task_id]->sec_period)
//                 {
//                     m_swti[task_id].sec_start = 1;
//                 }
//             }
//             else if(m_swti[task_id].sec_start)
//             {
//                 trig_time = m_swti[task_id].ref_time + m_swte[task_id]->sec_period;

//                 if(PERIOD_ELAPSED||PERIOD_ELAPSED_ROLLOVER)
//                 {
//                     sw_timer_fnc(m_swte[task_id]->task_sec);
//                     m_swti[task_id].ref_time = m_timer;
//                     m_swti[task_id].sec_start = 0;

//                     if(SUBTASK_MAX_EXECUTIONS)
//                     {
//                         put_flag = 0;
//                         m_swti[task_id].pri_cnt = 0;
//                         m_swti[task_id].enable = 0;
//                     }
//                 }
//             }
//         }
//         else
//         {
//             m_swti[task_id].ref_time = m_timer;
//         }

//         if(put_flag || !m_swti[task_id].one_shot)
//         {
//             cfifo_put(&m_cfifo, &task_id);
//         }
//         else
//         {
//             slot = sw_timer_get_slot(task_id);
//             sw_timer_release_slot(slot);
//         }
//     }
// }

TimerTask led_timer1{led_on, led_off, "LED task1!"};
TimerTask led_timer2{led_on, led_off, "LED task2!"};
TimerTask led_timer3{led_on, led_off, "LED task3!"};
TimerTask led_timer4{led_on, led_off, "LED task4!"};
TimerTask tt[] = {led_timer1, led_timer2, led_timer3, led_timer4};
void sw_timer_run(void)
{
#define PERIOD_ELAPSED (trig_time > tt[task].getReferenceTime() && m_timer >= trig_time)
#define PERIOD_ELAPSED_ROLLOVER (m_timer < tt[task].getReferenceTime() && m_timer >= trig_time)
#define TASK_MAX_EXECUTIONS (m_swti[task_id].pri_cnt >= m_swte[task_id]->pri_max && m_swte[task_id]->pri_max && !m_swte[task_id]->sec_period)
#define SUBTASK_MAX_EXECUTIONS (m_swti[task_id].pri_cnt >= m_swte[task_id]->pri_max && m_swte[task_id]->pri_max)

    uint16_t trig_time, task_id, task;

    uint8_t active_tasks_num = 4;
    printf("len=%u\n", active_tasks_num);

    for (task = 0; task < active_tasks_num; task++)
    {
        if (tt[task].is_enabled())
        {
            trig_time = tt[task].getReferenceTime() + tt[task].getPrimaryPeriod();

            if ((PERIOD_ELAPSED || PERIOD_ELAPSED_ROLLOVER) && !tt[task].didSecondaryStart())
            {
                tt[task].execute_primary();
                tt[task].setReferenceTime(m_timer);

                if(TASK_MAX_EXECUTIONS)// START from here

            }
        }
    }
}

bool done = true;

void *threadA(void *vargp)
{
    (void)vargp;
    while (!done)
    {
        m_timer++;
        usleep(1000);
    }
    return 0;
}

void *threadB(void *vargp)
{
    (void)vargp;
    while (!done)
    {
        sw_timer_run();
    }
    return 0;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, threadA, NULL);
    pthread_create(&thread_id, NULL, threadB, NULL);

    // cb.write()

    sw_timer_run();

    printf("%s\n", led_timer1.getDescription());
    printf("%s\n", led_timer2.getDescription());
    printf("%s\n", led_timer3.getDescription());
    printf("%s\n", led_timer4.getDescription());
}