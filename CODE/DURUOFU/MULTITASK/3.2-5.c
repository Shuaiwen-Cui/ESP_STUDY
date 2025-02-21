#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

// Task 1: Periodically prints logs
void Task_1(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
        ESP_LOGI(TAG, "Task_1");
    }
    vTaskDelete(NULL); // Delete the task (this line is unreachable in this example)
}

// Task 2: Periodically prints logs
void Task_2(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
        ESP_LOGI(TAG, "Task_2");
    }
    vTaskDelete(NULL); // Delete the task (this line is unreachable in this example)
}

void app_main(void)
{
    TaskHandle_t taskHandle_1 = NULL; // Handle for Task 1
    TaskHandle_t taskHandle_2 = NULL; // Handle for Task 2

    // Create Task 1 with a stack size of 2048 bytes and priority 12
    xTaskCreate(Task_1, "Task_1", 2048, NULL, 12, &taskHandle_1);
    // Create Task 2 with a stack size of 2048 bytes and priority 12
    xTaskCreate(Task_2, "Task_2", 2048, NULL, 12, &taskHandle_2);

    // Print the task list
    static char cBuffer[512] = {0}; // Buffer to store task list
    vTaskList(cBuffer); // Populate the buffer with task list information
    ESP_LOGI(TAG, "Task List:\n%s", cBuffer); // Log the task list

    while (1)
    {
        // Get the remaining stack space for Task 1
        int istack = uxTaskGetStackHighWaterMark(taskHandle_1);
        ESP_LOGI(TAG, "Task_1 Remaining Stack Space: %d", istack); // Log the stack space
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
}