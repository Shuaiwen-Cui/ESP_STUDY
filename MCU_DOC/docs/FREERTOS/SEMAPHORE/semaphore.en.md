# SEMAPHORE | TASK EXECUTION ORDER CONTROL AND RESOURCE SHARING

!!! NOTE "References"
    This document is based on the DURUOFU's ESP32-GUIDE. For more information, please refer to the original version: [DURUOFU ESP32-GUIDE](https://github.com/DuRuofu/ESP32-Guide).

!!! NOTE
    FreeRTOS provides semaphores and mutexes for task synchronization and resource sharing management. **Semaphores** are more inclined to **task synchronization**, while **mutexes** are used to **protect shared resources**.

## 1. Binary Semaphore

The binary semaphore is the most basic semaphore, with only two states: available and unavailable (or 1 and 0). It is usually used for task-to-task or interrupt-to-task synchronization. When an event occurs, the semaphore is released by an interrupt or task, and the waiting task is awakened. Binary semaphores are suitable for simple event notification scenarios, such as notifying a task to process external input or complete a task.

### 1.1 API Description:

| API Function | Description |
|:------------:|:------------|
| `xSemaphoreCreateBinary()` | Create a binary semaphore. |
| `xSemaphoreTake()` | Take a semaphore. |
| `xSemaphoreGive()` | Give a semaphore. |

### 1.2 Example Code:

```c
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
```
