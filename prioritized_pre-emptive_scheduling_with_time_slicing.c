#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_system.h"

#define TASK_STACK_SIZE 2048
TaskHandle_t task1Handle;
TaskHandle_t task2Handle;
TaskHandle_t task3Handle;

void task1(void *pvParameter)
{
    while (1)
    {
        ESP_LOGI("Task1", "Executed...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void *pvParameter)
{
    while (1)
    {
        ESP_LOGI("Task2", "Executed...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task3(void *pvParameter)
{
    while (1)
    {
        ESP_LOGI("Task3", "Executed...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    esp_task_wdt_deinit();
    xTaskCreatePinnedToCore(task1, "Task1", 2048, NULL, 1, &task1Handle, 0);
    xTaskCreatePinnedToCore(task2, "Task2", 2048, NULL, 1, &task2Handle, 0);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(task3, "Task3", 2048, NULL, 2, &task3Handle, 0);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    vTaskSuspend(task3Handle);
}