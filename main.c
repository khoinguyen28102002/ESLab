#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <time.h>
#include <string.h>

#define NUM_FUNCTIONAL_TASKS 4
#define TASK_DELAY 5

struct QueueData {
    int requestID ;
    char message[20];
    char reject;
};
struct TaskType {
    int taskID;
    char taskName[20];
};
struct TaskType Led = {.taskName = "Led" , .taskID = 0};
struct TaskType Print = {.taskName = "Print" , .taskID = 1};
struct TaskType Fan = {.taskName = "Fan" , .taskID = 2};
QueueHandle_t requestQueue;
static uint8_t s_led_state = 0;
static uint8_t s_fan_state = 0;

void reception_task(void* parameter)
{
    time_t t;
    srand((unsigned) time(&t));
    while (1){
        while(requestQueue == NULL){
            // Do nothing
        }
        int ID = rand() % (NUM_FUNCTIONAL_TASKS);
        int Delay = rand() % (TASK_DELAY) + 1;
        struct  QueueData *request = malloc(sizeof(struct QueueData));
        if(request != NULL){
            switch (ID){
                case 0:
                    request->requestID = 0;
                    strcpy(request->message, "LED");
                    request->reject = 0;
                    break;
                case 1:
                    request->requestID = 1;
                    strcpy(request->message, "HI 2013923 2015081");
                    request->reject = 0;
                    break;
                case 2:
                    request->requestID = 2;
                    strcpy(request->message, "FAN");
                    request->reject = 0;
                    break;
                case 3:
                    request->requestID = 99;
                    strcpy(request->message, "DO NOTHING");
                    request->reject = 0;
                    break;
                default:
                    break;
            }   
            if (xQueueSend(requestQueue, (void *)&request, 0) != pdPASS){
                printf("Failed to send request with ID %d to the queue!\n", ID);
            }
        }else{
            printf("Can't allocate memory for new struct!\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000 * Delay));
    }
    vTaskDelete(NULL);
}
void functional_task(void* pvParameter){
    while (1){
        struct TaskType *task = (struct TaskType *)pvParameter;
        struct QueueData *receiveMsg;
        if(requestQueue != NULL){
            if (xQueueReceive(requestQueue, &receiveMsg, (TickType_t)10) == pdPASS){
                if (receiveMsg->requestID == task->taskID){
                    if(receiveMsg->requestID == 0){
                        printf("Turning the LED %s\n", s_led_state == true ? "ON" : "OFF");
                        s_led_state = !s_led_state;
                    }else if(receiveMsg->requestID == 2){
                        printf("Turning the FAN %s\n", s_fan_state == true ? "ON" : "OFF");
                        s_fan_state = !s_fan_state;
                    }else{
                        printf("%s\n", receiveMsg->message);
                    }
                    free(receiveMsg);
                }else{
                    printf("%s: Receive %s, but it's not my task!\n", task->taskName, receiveMsg->message);
                    if(receiveMsg->reject < 2){
                        receiveMsg->reject++;
                        xQueueSendToFront(requestQueue, (void *)&receiveMsg, (TickType_t) 10);
                    }else{
                        printf("Task message: %s is rejected %d times, skip this task\n", receiveMsg->message, receiveMsg->reject);
                        free(receiveMsg);
                    }
                }
            }else{
                // printf("Failed to receive request from the queue\n");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}
void app_main()
{
    requestQueue = xQueueCreate(10 , sizeof(struct QueueData *) ) ;
    if (requestQueue == NULL ) {
        printf(" Failed to create queue : not enough RAM!");
    }
    xTaskCreate(reception_task, "Reception Task", 2048, NULL, 10, NULL);
    xTaskCreate(&functional_task, "Led", 2048, (void *)&Led, 10, NULL);
    xTaskCreate(&functional_task, "Print", 2048, (void *)&Print, 10, NULL);
    xTaskCreate(&functional_task, "Fan", 2048, (void *)&Fan, 10, NULL);
}