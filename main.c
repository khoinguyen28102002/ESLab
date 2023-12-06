#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "FreeRTOSConfig.h"

uint8_t counter_timer_1 = 0;
uint8_t counter_timer_2 = 0;
uint8_t counter_time = 1;

void timer_Callback(TimerHandle_t xTimer)
{
    // Get the timer's ID
    const uint8_t* timer_id = pvTimerGetTimerID(xTimer);

    // Check the ID to determine which timer called the callback
    if((int)timer_id == 1)
    {
        // Print the message and increment the counter for timer 1
        printf("At %ldms: ahihi count %d times \r\n", pdTICKS_TO_MS(xTaskGetTickCount()),  ++counter_timer_1);
        
        // Stop timer after 10 counts
        if(counter_timer_1 == 10) {
          printf("STOP ahihi timer \r\n");
          xTimerStop(xTimer, 0);
        } 
      }
    else if((int)timer_id == 2)
    {
        // Print the message and increment the counter for timer 2
        printf("At %ldms: ihaha count %d times \r\n", pdTICKS_TO_MS(xTaskGetTickCount()), ++counter_timer_2);
        
        // Stop timer after 5 counts
        if(counter_timer_2 == 5) {
          printf("STOP ihaha timer\r\n");
          xTimerStop(xTimer, 0);
        } 
      }
}

// void timer_CountTime()
// {
//     counter_time++;
//     if (counter_time <= 20)
//         printf("TimeStamp: %d \n", counter_time);
// }

void app_main()
{
    // Create a timer that counts every 1000ms
    // TimerHandle_t xTimer_temp = xTimerCreate("Timer temp", pdMS_TO_TICKS(1000), pdTRUE, 0, timer_CountTime);

    // Create timer 1 that calls the callback function every 2000m
    TimerHandle_t xTimer_1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(2000), pdTRUE, (void * const)1, timer_Callback);

    // Create timer 2 that calls the callback function every 3000m
    TimerHandle_t xTimer_2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(3000), pdTRUE, (void * const)2, timer_Callback);
        
    // printf("TimeStamp: %d \n", counter_time);
    printf("Start timer !!!\r\n");

    // start timers
    // xTimerStart(xTimer_temp, 0);

    xTimerStart(xTimer_1, 0);
    xTimerStart(xTimer_2, 0);
}
