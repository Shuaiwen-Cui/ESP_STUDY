#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

// Task function
void myTask(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "myTask");
    }
}

void app_main(void)
{
    // Create a FreeRTOS task
    // Parameter description:
    // 1. Task entry function: myTask
    // 2. Task name: "myTask", used for identifying the task during debugging
    // 3. Task stack size: 2048 bytes (allocate appropriately to avoid stack overflow)
    // 4. Task parameters: NULL (no parameters passed)
    // 5. Task priority: 1 (lower priority, idle task has priority 0)
    // 6. Task handle: NULL (no need to save the task handle)
    xTaskCreate(myTask, "myTask", 2048, NULL, 1, NULL);
}