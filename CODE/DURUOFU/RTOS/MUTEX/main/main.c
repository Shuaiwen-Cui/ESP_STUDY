// NON-RECURSIVE MUTEX

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"

// static const char *TAG = "main";

// SemaphoreHandle_t mutexHandle;

// void task1(void *pvParameters)
// {
//     ESP_LOGI(TAG, "task1 started!");
//     while (1)
//     {
//         if (xSemaphoreTake(mutexHandle, portMAX_DELAY) == pdTRUE)
//         {
//             ESP_LOGI(TAG, "task1 got the mutex!");
//             for (int i = 0; i < 10; i++)
//             {
//                 ESP_LOGI(TAG, "task1 running...%d", i);
//                 vTaskDelay(pdMS_TO_TICKS(1000));
//             }
//             xSemaphoreGive(mutexHandle);
//             ESP_LOGI(TAG, "task1 released the mutex!");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//         else
//         {
//             ESP_LOGI(TAG, "task1 failed to get the mutex!");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//     }
// }

// void task2(void *pvParameters)
// {
//     ESP_LOGI(TAG, "task2 started!");
//     while (1)
//     {
//     }
// }

// void task3(void *pvParameters)
// {
//     ESP_LOGI(TAG, "task3 started!");
//     vTaskDelay(pdMS_TO_TICKS(1000));
//     while (1)
//     {
//         if (xSemaphoreTake(mutexHandle, 1000) == pdPASS)
//         {
//             ESP_LOGI(TAG, "task3 got the mutex!");
//             for (int i = 0; i < 10; i++)
//             {
//                 ESP_LOGI(TAG, "task3 running...%d", i);
//                 vTaskDelay(pdMS_TO_TICKS(1000));
//             }
//             xSemaphoreGive(mutexHandle);
//             ESP_LOGI(TAG, "task3 released the mutex!");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//         else
//         {
//             ESP_LOGI(TAG, "task3 failed to get the mutex!");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//     }
// }

// void app_main(void)
// {
//     mutexHandle = xSemaphoreCreateMutex();
//     xTaskCreate(task1, "task1", 1024 * 2, NULL, 10, NULL);
//     xTaskCreate(task2, "task2", 1024 * 2, NULL, 10, NULL);
//     xTaskCreate(task3, "task3", 1024 * 2, NULL, 10, NULL);
// }

// RECUSIVE MUTEX

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static const char *TAG = "main";

SemaphoreHandle_t mutexHandle;

void task1(void *pvParameters)
{
    ESP_LOGI(TAG, "-------------------------------");
    ESP_LOGI(TAG, "task1 started!");
    while (1)
    {
        xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY);
        ESP_LOGI(TAG, "task1 got the mutex - using resource A");
        for (int i = 0; i < 10; i++)
        {
            ESP_LOGI(TAG, "task1 running...%d - using resource A", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY);
        ESP_LOGI(TAG, "task1 got the mutex - using resource B");
        for (int i = 0; i < 10; i++)
        {
            ESP_LOGI(TAG, "task1 running...%d - using resource B", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        xSemaphoreGiveRecursive(mutexHandle);
        ESP_LOGI(TAG, "task1 released the mutex - using resource B");
        vTaskDelay(pdMS_TO_TICKS(3000));
        xSemaphoreGiveRecursive(mutexHandle);
        ESP_LOGI(TAG, "task1 released the mutex - using resource A");
    }
    vTaskDelete(NULL);
}

void task2(void *pvParameters)
{
    ESP_LOGI(TAG, "task2 started!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        // Get the recursive mutex
        if (xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TAG, "task2 got the mutex");
            for (int i = 0; i < 10; i++)
            {
                ESP_LOGI(TAG, "task2 running...%d", i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            xSemaphoreGiveRecursive(mutexHandle);
            ESP_LOGI(TAG, "task2 released the mutex");
        }
        else
        {
            ESP_LOGI(TAG, "task2 failed to get the mutex");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void app_main(void)
{
    // Create a recursive mutex
    mutexHandle = xSemaphoreCreateRecursiveMutex();

    xTaskCreate(task1, "task1", 1024 * 2, NULL, 1, NULL);
    xTaskCreate(task2, "task2", 1024 * 2, NULL, 2, NULL);
}