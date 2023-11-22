#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_system.h"

void task1(void *pvParameter)
{
    while (1)
    {
        ESP_LOGI("Task1", "Executed...");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Yielding the CPU for 1000 milliseconds
    }
}

void task2(void *pvParameter)
{
    while (1)
    {
        ESP_LOGI("Task2", "Executed...");
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Yielding the CPU for 2000 milliseconds
    }
}

void app_main()
{
    // Configure FreeRTOS scheduler
    vTaskPrioritySet(NULL, tskIDLE_PRIORITY + 1);

    // Create tasks with different priorities
    xTaskCreatePinnedToCore(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 0);
}