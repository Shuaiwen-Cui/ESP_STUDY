#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "main";

TaskHandle_t taskHandle_1 = NULL; // Handle for Task 1

// Task 1: Periodically prints logs
void Task_1(void *pvParameters)
{
    while (1)
    {
        ESP_LOGI(TAG, "Task 1 is running...");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

// Task 2: Controls the suspension and resumption of Task 1
void Task_2(void *pvParameters)
{
    while (1)
    {
        ESP_LOGI(TAG, "Suspending Task 1...");
        vTaskSuspend(taskHandle_1); // Suspend Task 1
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds

        ESP_LOGI(TAG, "Resuming Task 1...");
        vTaskResume(taskHandle_1); // Resume Task 1
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for another 5 seconds
    }
}

void app_main(void)
{
    // Create Task 1
    xTaskCreate(Task_1, "Task_1", 2048, NULL, 6, &taskHandle_1);

    // Create Task 2
    xTaskCreate(Task_2, "Task_2", 2048, NULL, 5, NULL);
}