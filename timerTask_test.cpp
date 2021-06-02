#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "TimerTask.hpp"

void led_on(void)
{
    printf("LED ON\n");
}
void led_off(void)
{
    printf("LED OFF\n");
}

volatile uint32_t m_timer;

TimerTask led_timer1{led_on, led_off, &m_timer, "LED task1!", false};
// TimerTask led_timer2{led_on, led_off, "LED task2!", false};
// TimerTask led_timer3{led_on, led_off, "LED task3!", false};
// TimerTask led_timer4{led_on, led_off, "LED task4!", false};
TimerTask tt[] = {led_timer1};//, led_timer2, led_timer3, led_timer4};

bool done = false;

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
        led_timer1.mainLoop();
    }
    return 0;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, threadA, NULL);
    pthread_create(&thread_id, NULL, threadB, NULL);

    led_timer1.setPeriod(1000, 100);
    led_timer1.enable_task();


    printf("%s\n", led_timer1.getDescription());
    // printf("%s\n", led_timer2.getDescription());
    // printf("%s\n", led_timer3.getDescription());
    // printf("%s\n", led_timer4.getDescription());

     pthread_join(thread_id, NULL);

    printf("Done!\n");

}