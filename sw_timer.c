#include "sw_timer.h"
#include "cfifo.h"

static volatile uint16_t m_timer;
static  cfifo_t m_cfifo;
static  sw_timer_t *m_swte[SW_TIMER_SLOTS];
static _sw_timer_t  m_swti[SW_TIMER_SLOTS];
static uint8_t m_slots[SW_TIMER_SLOTS] = {1,2,3,4,5,6,7,8};

uint8_t sw_timer_get_task_id(uint8_t slot)
{
    uint8_t task_id;

    if(!slot || slot > SW_TIMER_SLOTS)
    {
        task_id = 0;
    }
    else
    {
        task_id = slot - 1U;
    }

    return task_id;
}

uint8_t sw_timer_get_slot(uint8_t task_id)
{
    return (task_id + 1U);
}

uint8_t sw_timer_acquire_next_slot(void)
{
    uint8_t slot = 0;

    for(uint8_t i = 0; i < SW_TIMER_SLOTS; i++)
    {
        if(m_slots[i])
        {
            slot = m_slots[i];
            m_slots[i] = 0;
            break;
        }
    }
    return slot;
}

uint8_t sw_timer_acquire_slot(uint8_t slot)
{
    uint8_t task_id = 0;

    if (slot > SW_TIMER_SLOTS)
    {
        return 0;
    }

    task_id = sw_timer_get_task_id(slot);

    if(m_slots[task_id])
    {
        slot = m_slots[task_id];
        m_slots[task_id] = 0;
    }
    else
    {
        slot = 0;
    }

    return slot;
}

void sw_timer_release_slot(uint8_t slot)
{
    uint8_t task_id = sw_timer_get_task_id(slot);
    m_slots[task_id] = slot;
}

void sw_timer_init(void)
{
    m_timer = 0;

    cfifo_init(&m_cfifo);

    for(uint8_t i = 0; i < SW_TIMER_SLOTS; i++)
    {
        m_swte[i]           = 0;
        m_swti[i].ref_time  = 0;
        m_swti[i].pri_cnt   = 0;
        m_swti[i].sec_start = 0;
        m_swti[i].enable    = 0;
        m_swti[i].one_shot = 0;
    }
}

uint8_t sw_timer_create(sw_timer_t *swt, uint8_t slot, uint8_t one_shot)
{
    uint16_t task_id;

    if(!slot)
    {
        slot = sw_timer_acquire_next_slot();
    }
    else
    {
        slot = sw_timer_acquire_slot(slot);
    }

    if(!slot || !swt)
        return 0;

    task_id = sw_timer_get_task_id(slot);

    cfifo_put(&m_cfifo, & task_id);

    m_swte[task_id]           = swt;
    m_swte[task_id]->slot     = slot;
    m_swti[task_id].ref_time  = m_timer;
    m_swti[task_id].enable    = 0;
    m_swti[task_id].one_shot = one_shot;

    return slot;
}

void sw_timer_start(uint8_t slot)
{
    uint8_t task_id = sw_timer_get_task_id(slot);

    if(!sw_timer_acquire_slot(slot))
    {
        if(!m_swti[task_id].enable)
        {
            m_swti[task_id].enable    = 1;
            m_swti[task_id].ref_time  = m_timer;
            m_swti[task_id].pri_cnt   = 0;
            m_swti[task_id].sec_start = 0;
        }
    }
}

void sw_timer_restart(uint8_t slot)
{
    uint8_t task_id = sw_timer_get_task_id(slot);

    if(!sw_timer_acquire_slot(slot))
    {
        m_swti[task_id].enable    = 1;
        m_swti[task_id].ref_time  = m_timer;
        m_swti[task_id].pri_cnt   = 0;
        m_swti[task_id].sec_start = 0;
    }
}

void sw_timer_stop(uint8_t slot)
{
    uint8_t task_id = sw_timer_get_task_id(slot);

    if(!sw_timer_acquire_slot(slot))
    {
        m_swti[task_id].enable = 0;
    }
}

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


void sw_timer_tick(void)
{
    m_timer++;
}

void sw_timer_fnc(fp_t fp)
{
    if(fp)
    {
        fp();
    }
}

void sw_timer_run(void)
{
    #define PERIOD_ELAPSED           (trig_time > m_swti[task_id].ref_time && m_timer >= trig_time)
    #define PERIOD_ELAPSED_ROLLOVER  (m_timer   < m_swti[task_id].ref_time && m_timer >= trig_time)
    #define TASK_MAX_EXECUTIONS      (m_swti[task_id].pri_cnt >= m_swte[task_id]->pri_max && m_swte[task_id]->pri_max && !m_swte[task_id]->sec_period)
    #define SUBTASK_MAX_EXECUTIONS   (m_swti[task_id].pri_cnt >= m_swte[task_id]->pri_max && m_swte[task_id]->pri_max)

    uint16_t trig_time, task_id;
    uint8_t  put_flag, task, slot;
    uint8_t  active_tasks_num = cfifo_cnt(&m_cfifo);

    for(task = 0; task < active_tasks_num; task++)
    {
        put_flag = 1;
        cfifo_get(&m_cfifo, &task_id);

        if(m_swti[task_id].enable)
        {
            trig_time = m_swti[task_id].ref_time + m_swte[task_id]->pri_period;

            if( (PERIOD_ELAPSED||PERIOD_ELAPSED_ROLLOVER) && !m_swti[task_id].sec_start)
            {
                sw_timer_fnc(m_swte[task_id]->task_pri);
                m_swti[task_id].ref_time = m_timer;
                m_swti[task_id].pri_cnt++;

                if(TASK_MAX_EXECUTIONS)
                {
                    put_flag = 0;
                    m_swti[task_id].pri_cnt = 0;
                    m_swti[task_id].enable = 0;
                }

                if(m_swte[task_id]->sec_period)
                {
                    m_swti[task_id].sec_start = 1;
                }
            }
            else if(m_swti[task_id].sec_start)
            {
                trig_time = m_swti[task_id].ref_time + m_swte[task_id]->sec_period;

                if(PERIOD_ELAPSED||PERIOD_ELAPSED_ROLLOVER)
                {
                    sw_timer_fnc(m_swte[task_id]->task_sec);
                    m_swti[task_id].ref_time = m_timer;
                    m_swti[task_id].sec_start = 0;

                    if(SUBTASK_MAX_EXECUTIONS)
                    {
                        put_flag = 0;
                        m_swti[task_id].pri_cnt = 0;
                        m_swti[task_id].enable = 0;
                    }
                }
            }
        }
        else
        {
            m_swti[task_id].ref_time = m_timer;
        }

        if(put_flag || !m_swti[task_id].one_shot)
        {
            cfifo_put(&m_cfifo, &task_id);
        }
        else
        {
            slot = sw_timer_get_slot(task_id);
            sw_timer_release_slot(slot);
        }
    }
}
