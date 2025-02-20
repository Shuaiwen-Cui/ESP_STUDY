// // Delete other task

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// static const char *TAG = "main";

// // Task function
// void myTask(void *pvParameters)
// {
//     for (;;)
//     {
//         vTaskDelay(500 / portTICK_PERIOD_MS);
//         ESP_LOGI(TAG, "myTask");
//     }
// }

// void app_main(void)
// {
//     // Task handle
//     TaskHandle_t taskHandle = NULL;
//     // Create a FreeRTOS task
//     xTaskCreate(myTask, "myTask", 2048, NULL, 1, &taskHandle);

//     // Delay for 2 seconds
//     vTaskDelay(2000 / portTICK_PERIOD_MS);

//     // Delete the task
//     if (taskHandle != NULL)
//     {
//         vTaskDelete(taskHandle);
//     }
// }

// Delete itself

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

// Task function
void myTask(void *pvParameters)
{
    // Delay for 1 second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "myTask:1");

    // Delay for another 1 second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "myTask:2");

    // Delay for another 1 second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "myTask:3");

    // Delete the task (if NULL is passed, the current task is deleted)
    vTaskDelete(NULL);
}

void app_main(void)
{
    // Task handle
    TaskHandle_t taskHandle = NULL;
    // Create a FreeRTOS task
    xTaskCreate(myTask, "myTask", 2048, NULL, 1, &taskHandle);
}