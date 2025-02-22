# MUTEX 

!!! NOTE "References"
    This document is based on the DURUOFU's ESP32-GUIDE. For more information, please refer to the original version: [DURUOFU ESP32-GUIDE](https://github.com/DuRuofu/ESP32-Guide).

## 1. Mutex

Mutex (Mutual Exclusion Lock): A mutex is very similar to a binary semaphore, but there are some subtle differences: mutexes have a **priority inheritance mechanism**, but binary semaphores do not.


!!! NOTE
    Suppose there are three tasks: task_L, task_M, task_H, with priorities increasing in order. task_L has the lowest priority, task_M has a medium priority, and task_H has the highest priority. task_L and task_H share resources, protected by a mutex (assuming no priority inheritance). task_L starts first, acquires the mutex, task_H starts later, tries to acquire the mutex, but because task_L holds the mutex, task_H is blocked. At this time, task_M starts, and task_M's priority is higher than task_L's. task_M preempts task_L's CPU time slice. task_L cannot release the mutex because it is preempted by task_M, causing task_H to be unable to execute, even though it has the highest priority. This is a typical **priority inversion** phenomenon.

!!! Priority Inheritance
    Priority inheritance is a method to solve the priority inversion problem. When a low-priority task holds a mutex, if a high-priority task requests the mutex, the priority of the low-priority task will be raised to the priority of the high-priority task until the low-priority task releases the mutex.

### 1.1 Example Code:

```c

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static const char *TAG = "main";

SemaphoreHandle_t mutexHandle;

void task1(void *pvParameters)
{
    ESP_LOGI(TAG, "task1 started!");
    while (1)
    {
        if (xSemaphoreTake(mutexHandle, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TAG, "task1 got the mutex!");
            for (int i = 0; i < 10; i++)
            {
                ESP_LOGI(TAG, "task1 running...%d", i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            xSemaphoreGive(mutexHandle);
            ESP_LOGI(TAG, "task1 released the mutex!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            ESP_LOGI(TAG, "task1 failed to get the mutex!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void task2(void *pvParameters)
{
    ESP_LOGI(TAG, "task2 started!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
    }
}

void task3(void *pvParameters)
{
    ESP_LOGI(TAG, "task3 started!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        if (xSemaphoreTake(mutexHandle, 1000) == pdPASS)
        {
            ESP_LOGI(TAG, "task3 got the mutex!");
            for (int i = 0; i < 10; i++)
            {
                ESP_LOGI(TAG, "task3 running...%d", i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            xSemaphoreGive(mutexHandle);
            ESP_LOGI(TAG, "task3 released the mutex!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            ESP_LOGI(TAG, "task3 failed to get the mutex!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void app_main(void)
{
    mutexHandle = xSemaphoreCreateMutex();
    xTaskCreate(task1, "task1", 1024 * 2, NULL, 10, NULL);
    xTaskCreate(task2, "task2", 1024 * 2, NULL, 10, NULL);
    xTaskCreate(task3, "task3", 1024 * 2, NULL, 10, NULL);
}

```

## 2. Recursive Mutex

Recursive mutexes are a special type of mutex that can be locked multiple times by the same task. Each time the mutex is locked, the task must unlock it the same number of times before it is actually released.

### 2.1 Example Code:

```c

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
```