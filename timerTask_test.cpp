#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "TimerTask.hpp"
#include "TaskRunner.hpp"

bool done = false;
long long timestamp_start1;
long long timestamp_start2;
long long timestamp_start3;

long long current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

void led1_on(void)
{
    printf("LED1 ON: %lld\n", current_timestamp() - timestamp_start1);
}
void led1_off(void)
{
    printf("LED1 OFF: %lld\n", current_timestamp() - timestamp_start1);
}

void buzzer_on(void)
{
    printf("BUZZER ON: %lld\n", current_timestamp() - timestamp_start2);
}
void buzzer_off(void)
{
    printf("BUZZER OFF: %lld\n", current_timestamp() - timestamp_start2);
}

void led2_on(void)
{
    printf("LED2 ON: %lld\n", current_timestamp() - timestamp_start3);
}
void led2_off(void)
{
    printf("LED2 OFF: %lld\n", current_timestamp() - timestamp_start3);
}

volatile uint32_t m_timer = 0xFFFFF800;
volatile uint32_t *const app::os::TimerTask::timer = &m_timer;
app::os::task_list_t task_list1 = {{led1_on, led1_off, led1_on, led1_off, 0, 0, 0, 0}, {1000, 500, 250, 100, 0, 0, 0, 0}, {4}};
app::os::task_list_t task_list2 = {{buzzer_on, buzzer_off, buzzer_on, buzzer_off, 0, 0, 0, 0}, {100, 250, 500, 1000, 0, 0, 0, 0}, {4}};
app::os::task_list_t task_list3 = {{led2_on, led2_off, led2_on, led2_off, 0, 0, 0, 0}, {100, 50, 150, 100, 0, 0, 0, 0}, {4}};

app::os::TimerTask timerTasks[] = {
    {&task_list1, 5, "LED1 sequence task"},
    {&task_list2, 3, "Buzzer sequence task"},
    {&task_list3, 7, "LED2 sequence task"},
};

app::os::TaskRunner taskRunner{timerTasks, 3};

void *SysTickFakeInterrupt(void *vargp)
{
    (void)vargp;
    while (!done)
    {
        m_timer++;
        if (!m_timer)
        {
            printf("**************rollover!\n");
        }
        usleep(1000);
    }
    return 0;
}

void *main_loop(void *vargp)
{
    (void)vargp;
    app::os::TimerTask *p_tasks = nullptr;
    timestamp_start1 = current_timestamp();
    timestamp_start2 = current_timestamp();
    timestamp_start3 = current_timestamp();

    while (!done)
    {
        p_tasks = timerTasks;

        for (size_t i = 0; i < 3; i++)
        {
            if (p_tasks->scheduler() == true)
            {
                // printf("Executed task %s\n", p_tasks->getDescription());
                if (i == 0)
                    timestamp_start1 = current_timestamp();
                if (i == 1)
                    timestamp_start2 = current_timestamp();
                if (i == 2)
                    timestamp_start3 = current_timestamp();
            }
            if (p_tasks->isDone())
            {
                p_tasks->start();
                printf("Executed task %s restarted!\n", p_tasks->getDescription());
            }
            p_tasks++;
        }
    }
    return 0;
}
void *main_loop2(void *vargp)
{
    (void)vargp;
    timestamp_start1 = current_timestamp();
    timestamp_start2 = current_timestamp();
    timestamp_start3 = current_timestamp();
    uint8_t taskid;
    while (!done)
    {
        if (taskRunner.run(&taskid))
        {
            if (taskid == 0)
            {
                timestamp_start1 = current_timestamp();
            }
            if (taskid == 1)
            {
                timestamp_start2 = current_timestamp();
            }
            if (taskid == 2)
            {
                timestamp_start3 = current_timestamp();
            }
            printf("Executed task [%d] %s\n", taskid, taskRunner.getDescription(taskid));
        }
        else
        {
            for (size_t i = 0; i < 3; i++)
            {
                if (!taskRunner.isRunning(i))
                {
                    taskRunner.start(i);
                    printf("Executed task %s restarted!\n", taskRunner.getDescription(taskid));
                }
            }
        }
    }
    return 0;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, SysTickFakeInterrupt, NULL);
    // pthread_create(&thread_id, NULL, main_loop, NULL);
    pthread_create(&thread_id, NULL, main_loop2, NULL);

    for (size_t i = 0; i < 3; i++)
    {
        // timerTasks[i].start();
        taskRunner.start(i);
    }

    pthread_join(thread_id, NULL);
    printf("Done!\n");
}