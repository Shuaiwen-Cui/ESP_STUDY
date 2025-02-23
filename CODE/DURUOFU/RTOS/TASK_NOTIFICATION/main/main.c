// // Direct Task Notification

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// static const char *TAG = "main";

// static TaskHandle_t xTask1 = NULL, xTask2 = NULL;

// void task1(void *pvParameters)
// {
//     ESP_LOGI(TAG, "-------------------------------");
//     ESP_LOGI(TAG, "task1 started!");

//     while (1)
//     {
//         ESP_LOGI(TAG, "task1: Waiting for task notification");
//         ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//         ESP_LOGI(TAG, "task1: Received task notification");
//         vTaskDelay(pdMS_TO_TICKS(3000));
//     }
// }

// void task2(void *pvParameters)
// {
//     ESP_LOGI(TAG, "-------------------------------");
//     ESP_LOGI(TAG, "task2 started!");

//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(5000));
//         ESP_LOGI(TAG, "task2: Sending task notification");
//         xTaskNotifyGive(xTask1);
//     }
// }

// void app_main(void)
// {

//     xTaskCreate(task1, "task1", 1024 * 2, NULL, 1, &xTask1);
//     xTaskCreate(task2, "task2", 1024 * 2, NULL, 1, &xTask2);
// }


// NOTIFICATION BY BIT


#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

static TaskHandle_t xTask1 = NULL, xTask2 = NULL;

void task1(void *pvParameters)
{
    ESP_LOGI(TAG, "-------------------------------");
    ESP_LOGI(TAG, "task1 started!");

    uint32_t ulNotifiedValue;

    while (1)
    {
        ESP_LOGI(TAG, "task1: Waiting for task notification");
        xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
        // Determine the source of the notification by different bit positions
        if ((ulNotifiedValue & 0x01) != 0)
        {
            ESP_LOGI(TAG, "task1: Received task notification-bit0");
        }
        if ((ulNotifiedValue & 0x02) != 0)
        {
            ESP_LOGI(TAG, "task1: Received task notification-bit1");
        }
        if ((ulNotifiedValue & 0x04) != 0)
        {
            ESP_LOGI(TAG, "task1: Received task notification-bit2");
        }
    }
}

void task2(void *pvParameters)
{
    ESP_LOGI(TAG, "-------------------------------");
    ESP_LOGI(TAG, "task2 started!");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "task2: Sending task notification-bit0");
        xTaskNotify(xTask1, 0x01, eSetValueWithOverwrite);

        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "task2: Sending task notification-bit1");
        xTaskNotify(xTask1, 0x02, eSetValueWithOverwrite);

        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "task2: Sending task notification-bit2");
        xTaskNotify(xTask1, 0x04, eSetValueWithOverwrite);

    }
}

void app_main(void)
{

    xTaskCreate(task1, "task1", 1024 * 4, NULL, 1, &xTask1);
    xTaskCreate(task2, "task2", 1024 * 4, NULL, 1, &xTask2);
}
