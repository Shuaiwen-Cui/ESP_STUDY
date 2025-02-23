// Binary Semaphore
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static const char *TAG = "main";

// Binary Semaphore
SemaphoreHandle_t semaphoreHandle;

// Shared variable
int shareVariable = 0;

void task1(void *pvParameters)
{
    for (;;)
    {
        // Take the semaphore, the semaphore becomes 0
        xSemaphoreTake(semaphoreHandle, portMAX_DELAY);
        for (int i = 0; i < 10; i++)
        {
            shareVariable++;
            ESP_LOGI(TAG, "task1 shareVariable:%d", shareVariable);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        // Give the semaphore, the semaphore becomes 1
        xSemaphoreGive(semaphoreHandle);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2(void *pvParameters)
{
    for (;;)
    {
        // Take the semaphore
        xSemaphoreTake(semaphoreHandle, portMAX_DELAY);
        for (int i = 0; i < 10; i++)
        {
            shareVariable--;
            ESP_LOGI(TAG, "task2 shareVariable:%d", shareVariable);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        // Give the semaphore
        xSemaphoreGive(semaphoreHandle);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    semaphoreHandle = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphoreHandle);
    xTaskCreate(task1, "task1", 2048, NULL, 10, NULL);
    xTaskCreate(task2, "task2", 2048, NULL, 10, NULL);
}