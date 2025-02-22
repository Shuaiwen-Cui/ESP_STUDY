# 互斥锁

!!! note "参考出处"
    本文档参考了DURUOFU的[ESP32-教程](https://github.com/DuRuofu/ESP32-Guide)。 

## 1. 互斥量

互斥量(互斥锁Mutex):互斥锁和二进制信号量极为相似，但有一些细微差异：互斥锁具有**优先级继承机制**， 但二进制信号量没有。


!!! note
    假设有三个任务：task_L, task_M, task_H, 优先级依次递增。task_L 优先级最低，task_M 优先级中等，task_H 优先级最高。task_L 和 task_H共享资源，之间有互斥锁(假设无优先级继承)保护。task_L首先开始，获取互斥锁，task_H后开始，尝试获取互斥锁，但由于task_L持有互斥锁，task_H会被阻塞。此时task_M开始，task_M的优先级高于task_L，task_M会抢占task_L的CPU时间片，task_L因为被task_M抢占，无法释放互斥锁，导致task_H无法执行，即便其优先级最高。这就是典型的**优先级反转现象**。

!!! 优先级继承
    优先级继承是一种解决优先级反转问题的方法。当一个低优先级任务持有一个互斥锁时，如果一个高优先级任务请求这个互斥锁，低优先级任务的优先级会被提升到高优先级任务的优先级，直到低优先级任务释放互斥锁。

### 1.1 示例代码：

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
    ESP_LOGI(TAG, "task1启动!");
    while (1)
    {
        if (xSemaphoreTake(mutexHandle, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TAG, "task1获取到互斥量!");
            for (int i = 0; i < 10; i++)
            {
                ESP_LOGI(TAG, "task1执行中...%d", i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            xSemaphoreGive(mutexHandle);
            ESP_LOGI(TAG, "task1释放互斥量!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            ESP_LOGI(TAG, "task1获取互斥量失败!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void task2(void *pvParameters)
{
    ESP_LOGI(TAG, "task2启动!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
    }
}

void task3(void *pvParameters)
{
    ESP_LOGI(TAG, "task3启动!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        if (xSemaphoreTake(mutexHandle, 1000) == pdPASS)
        {
            ESP_LOGI(TAG, "task3获取到互斥量!");
            for (int i = 0; i < 10; i++)
            {
                ESP_LOGI(TAG, "task3执行中...%d",i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }

            xSemaphoreGive(mutexHandle);
            ESP_LOGI(TAG, "task3释放互斥量!");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        else
        {
            ESP_LOGI(TAG, "task3未获取到互斥量!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void app_main(void)
{
    mutexHandle = xSemaphoreCreateMutex();
    // mutexHandle = xSemaphoreCreateBinary();
    xTaskCreate(task1, "task1", 1024 * 2, NULL, 1, NULL);
    xTaskCreate(task2, "task2", 1024 * 2, NULL, 2, NULL);
    xTaskCreate(task3, "task3", 1024 * 2, NULL, 3, NULL);
}

```

## 2. 递归互斥量

非递归互斥锁只能被一个任务获取一次，如果同一个任务想再次获取则会失败， 因为当任务第一次释放互斥锁时，互斥锁就一直处于释放状态。与非递归互斥锁相反，递归互斥锁可以被同一个任务获取很多次， 获取多少次就需要释放多少次， 此时才会返回递归互斥锁。
### 2.1  示例代码：

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
    ESP_LOGI(TAG, "task1启动!");
    while (1)
    {
        xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY);
        ESP_LOGI(TAG, "task1获取到互斥量-使用资源A");
        for (int i = 0; i < 10; i++)
        {
            ESP_LOGI(TAG, "task1执行中...%d -使用资源A", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY);
        ESP_LOGI(TAG, "task1获取到互斥量-使用资源B");
        for (int i = 0; i < 10; i++)
        {
            ESP_LOGI(TAG, "task1执行中...%d -使用资源B", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        xSemaphoreGiveRecursive(mutexHandle);
        ESP_LOGI(TAG, "task1释放互斥量-使用资源B");
        vTaskDelay(pdMS_TO_TICKS(3000));
        xSemaphoreGiveRecursive(mutexHandle);
        ESP_LOGI(TAG, "task1释放互斥量-使用资源A");
    }
    vTaskDelete(NULL);
}

void task2(void *pvParameters)
{
    ESP_LOGI(TAG, "task2启动!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        // 获取递归互斥锁
        if (xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TAG, "task2获取到互斥量");
            for (int i = 0; i < 10; i++)
            {
                ESP_LOGI(TAG, "task2执行中...%d", i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            xSemaphoreGiveRecursive(mutexHandle);
            ESP_LOGI(TAG, "task2释放互斥量");
        }
        else
        {
            ESP_LOGI(TAG, "task2获取互斥量失败");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void app_main(void)
{
    // 创建递归互斥量
    mutexHandle = xSemaphoreCreateRecursiveMutex();

    xTaskCreate(task1, "task1", 1024 * 2, NULL, 1, NULL);
    xTaskCreate(task2, "task2", 1024 * 2, NULL, 2, NULL);
}

```