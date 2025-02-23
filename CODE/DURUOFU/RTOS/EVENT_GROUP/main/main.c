// // EVENT GROUP WAITING

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"

// static const char *TAG = "main";

// EventGroupHandle_t xCreatedEventGroup;

// #define BIT_0 (1 << 0)
// #define BIT_4 (1 << 4)

// void task1(void *pvParameters)
// {
//     ESP_LOGI(TAG, "-------------------------------");
//     ESP_LOGI(TAG, "task1 started!");

//     while (1)
//     {
//         // Wait for BIT_0 or BIT_4 to be set in the event group
//         EventBits_t uxBits;
//         uxBits = xEventGroupWaitBits(
//             xCreatedEventGroup, /* The event group being tested. */
//             BIT_0 | BIT_4,      /* The bits within the event group to wait for. */
//             pdTRUE,             /* BIT_0 & BIT_4 should be cleared before returning. */
//             pdFALSE,            /* Don't wait for both bits, either bit will do. */
//             portMAX_DELAY);     /* Wait indefinitely for either bit to be set. */

//         // Check which bits are set
//         if ((uxBits & (BIT_0 | BIT_4)) == (BIT_0 | BIT_4))
//         {
//             ESP_LOGI(TAG, "BIT_0 and BIT_4 are both set");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "One of the bits (BIT_0 or BIT_4) is set");
//         }
//     }
// }

// void task2(void *pvParameters)
// {
//     ESP_LOGI(TAG, "task2 started!");
//     vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
//     while (1)
//     {
//         // Set BIT_0 in the event group
//         xEventGroupSetBits(xCreatedEventGroup, BIT_0);
//         ESP_LOGI(TAG, "BIT_0 is set");
//         vTaskDelay(pdMS_TO_TICKS(3000)); // Delay for 3 seconds

//         // Set BIT_4 in the event group
//         xEventGroupSetBits(xCreatedEventGroup, BIT_4);
//         ESP_LOGI(TAG, "BIT_4 is set");
//         vTaskDelay(pdMS_TO_TICKS(3000)); // Delay for 3 seconds

//         xEventGroupSetBits(xCreatedEventGroup, BIT_0 | BIT_4);
//         ESP_LOGI(TAG, "ddd BIT_0 and BIT_4 are both set");
//         vTaskDelay(pdMS_TO_TICKS(3000)); // Delay for 3 seconds
//     }
// }

// void app_main(void)
// {

//     // create event group
//     xCreatedEventGroup = xEventGroupCreate();

//     if (xCreatedEventGroup == NULL)
//     {
//         ESP_LOGE(TAG, "create event group failed");
//     }
//     else
//     {
//         xTaskCreate(task1, "task1", 1024 * 2, NULL, 1, NULL);
//         xTaskCreate(task2, "task2", 1024 * 2, NULL, 1, NULL);
//     }
// }



// EVENT GROUP SYNCHRONIZATION
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

/* Bits used by the three tasks. */
#define TASK_0_BIT (1 << 0)
#define TASK_1_BIT (1 << 1)
#define TASK_2_BIT (1 << 2)

#define ALL_SYNC_BITS (TASK_0_BIT | TASK_1_BIT | TASK_2_BIT)

static const char *TAG = "main";
EventGroupHandle_t xEventBits;


void task0(void *pvParameters)
{
    ESP_LOGI(TAG, "-------------------------------");
    ESP_LOGI(TAG, "task0 started!");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));
        ESP_LOGI(TAG, "task0: task synchronization started");
        // Event synchronization
        xEventGroupSync(
            xEventBits,     /* The event group being tested. */
            TASK_0_BIT,     /* The bits within the event group to wait for. */
            ALL_SYNC_BITS,  /* The bits within the event group to wait for. */
            portMAX_DELAY); /* Wait a maximum of 100ms for either bit to be set. */

    ESP_LOGI(TAG, "task0: task synchronization completed");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
void task1(void *pvParameters)
{
    ESP_LOGI(TAG, "-------------------------------");
    ESP_LOGI(TAG, "task1 started!");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(4000));
        ESP_LOGI(TAG, "task1: task synchronization started");

        // Event synchronization
        xEventGroupSync(
            xEventBits,     /* The event group being tested. */
            TASK_1_BIT,     /* The bits within the event group to wait for. */
            ALL_SYNC_BITS,  /* The bits within the event group to wait for. */
            portMAX_DELAY); /* Wait a maximum of 100ms for either bit to be set. */

        ESP_LOGI(TAG, "task1: task synchronization completed");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void task2(void *pvParameters)
{
    ESP_LOGI(TAG, "-------------------------------");
    ESP_LOGI(TAG, "task2 started!");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "task2: task synchronization started");
        // Event synchronization
        xEventGroupSync(
            xEventBits,     /* The event group being tested. */
            TASK_2_BIT,     /* The bits within the event group to wait for. */
            ALL_SYNC_BITS,  /* The bits within the event group to wait for. */
            portMAX_DELAY); /* Wait a maximum of 100ms for either bit to be set. */

        ESP_LOGI(TAG, "task2: task synchronization completed");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}


void app_main(void)
{
    // Create event group
    xEventBits = xEventGroupCreate();

    if (xEventBits == NULL)
    {
        ESP_LOGE(TAG, "Failed to create event group");
    }
    else
    {
        xTaskCreate(task0, "task0", 1024 * 2, NULL, 3, NULL);
        xTaskCreate(task1, "task1", 1024 * 2, NULL, 2, NULL);
        xTaskCreate(task2, "task2", 1024 * 2, NULL, 1, NULL);
    }
}