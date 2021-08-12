#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "TimerTask.hpp"

void led1_on(void)
{
    printf("LED1 ON\n");
}
void led1_off(void)
{
    printf("LED1 OFF\n");
}

void led2_on(void)
{
    printf("BUZZER ON\n");
}
void led2_off(void)
{
    printf("BUZZER OFF\n");
}

volatile uint32_t m_timer;
volatile uint32_t *const app::os::TimerTask::timer = &m_timer;
app::os::task_list_t task_list1 = {{led1_on, led1_off, led1_on, led1_off, 0, 0, 0, 0}, {1000, 500, 250, 100, 0, 0, 0, 0}, {4}};
app::os::task_list_t task_list2 = {{led2_on, led2_off, led2_on, led2_off, 0, 0, 0, 0}, {100, 250, 500, 1000, 0, 0, 0, 0}, {4}};

app::os::TimerTask timerTasks[] = {
    {&task_list1, 5, "Flash LED1 10 times!"},
    // {&task_list2, 10, "Flash LED2 10 times!"},
};

bool done = false;

void *SysTickFakeInterrupt(void *vargp)
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
    app::os::TimerTask *p_tasks = nullptr;

    while (!done)
    {
        p_tasks = timerTasks;
        for (size_t i = 0; i < 1; i++)
        {
            if (p_tasks->scheduler() == true)
            {
                // printf("Executed task %s\n", p_tasks->getDescription());
            }
            if (p_tasks->isSequenceComplete())
            {
                p_tasks->start();
                printf("Executed task %s restarted!\n", p_tasks->getDescription());

            }
            p_tasks++;
        }
    }
    return 0;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, SysTickFakeInterrupt, NULL);
    pthread_create(&thread_id, NULL, threadB, NULL);

    for (size_t i = 0; i < 2; i++)
    {
        timerTasks[i].start();
    }

    pthread_join(thread_id, NULL);
    printf("Done!\n");
}