#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

// Define a structure to hold an integer and an array
typedef struct
{
    int Int;
    int Array[3];
} MyStruct;

// Task function 1: Accepts an integer parameter
void Task_1(void *pvParameters)
{
    int *pInt = (int *)pvParameters;
    ESP_LOGI(TAG, "Received integer parameter: %d", *pInt);
    vTaskDelete(NULL); // Delete the task after execution
}

// Task function 2: Accepts an array parameter
void Task_2(void *pvParameters)
{
    int *pArray = (int *)pvParameters;
    ESP_LOGI(TAG, "Received array parameters: %d %d %d", *pArray, *(pArray + 1), *(pArray + 2));
    vTaskDelete(NULL); // Delete the task after execution
}

// Task function 3: Accepts a structure parameter
void Task_3(void *pvParameters)
{
    MyStruct *pStruct = (MyStruct *)pvParameters;
    ESP_LOGI(TAG, "Received structure parameters: %d %d %d %d", pStruct->Int, pStruct->Array[0], pStruct->Array[1], pStruct->Array[2]);
    vTaskDelete(NULL); // Delete the task after execution
}

// Task function 4: Accepts a string parameter
void Task_4(void *pvParameters)
{
    char *pChar = (char *)pvParameters;
    ESP_LOGI(TAG, "Received string parameter: %s", pChar);
    vTaskDelete(NULL); // Delete the task after execution
}

// Global parameters to be passed to tasks
int Parameters_1 = 1;
int Parameters_2[3] = {1, 2, 3};
MyStruct Parameters_3 = {1, {1, 2, 3}};
static const char *Parameters_4 = "Hello World!";

void app_main(void)
{
    // Create Task 1 and pass an integer parameter
    xTaskCreate(Task_1, "Task_1", 2048, (void *)&Parameters_1, 1, NULL);

    // Create Task 2 and pass an array parameter
    xTaskCreate(Task_2, "Task_2", 2048, (void *)&Parameters_2, 1, NULL);

    // Create Task 3 and pass a structure parameter
    xTaskCreate(Task_3, "Task_3", 3048, (void *)&Parameters_3, 1, NULL);

    // Create Task 4 and pass a string parameter (note: no address-of operator & is used)
    xTaskCreate(Task_4, "Task_4", 3048, (void *)Parameters_4, 1, NULL);
}