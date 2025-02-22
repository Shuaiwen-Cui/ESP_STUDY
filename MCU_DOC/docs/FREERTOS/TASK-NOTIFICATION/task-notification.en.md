# TASK NOTIFICATION | EFFECTIVE COMMUNICATION BETWEEN TASKS

!!! NOTE "References"
    This document is based on the DURUOFU's ESP32-GUIDE. For more information, please refer to the original version: [DURUOFU ESP32-GUIDE](https://github.com/DuRuofu/ESP32-Guide).

Task Notifications is a lightweight inter-task communication and synchronization mechanism that is more efficient than queues or event groups because it does not require dynamic memory allocation. Each task has a built-in task notification value that other tasks or interrupt service routines (ISRs) can use to notify the task of an event.

**Features**:

- Each task has a task notification value (32-bit integer) that can be used to store information.

- The task notification value is initialized to 0 by default.

- One task can operate on another task's notification value through the notification function.

- The task notification value can be used as a binary semaphore, counting semaphore, or a simple 32-bit 
variable.

- When a task receives a notification, it can choose to block (wait for notification) or process the notification.

## 1. API Description:

Task notifications mainly involve the following APIs:

| Function Name | Function |
| -------------- | -------- |
| xTaskNotify | Sends a notification to a specified task, and the notification value can be overwritten |
| xTaskNotifyGive | Simplified notification function for sending semaphore notifications |
| xTaskNotifyWait | Waits for the notification value to be updated and chooses whether to clear the notification value |
| ulTaskNotifyTake | Waits for task notifications and automatically decrements the notification value (usually used for counting semaphores) |
| xTaskNotifyStateClear | Clears the notification state of the task |

### xTaskNotify

**Function**: Sends a notification to a specified task and modifies the task's notification value.

**Prototype**:

```c
BaseType_t xTaskNotify(
    TaskHandle_t xTaskToNotify,
    uint32_t ulValue,
    eNotifyAction eAction
);
```

**Parameters**:

- xTaskToNotify: Task handle to receive the notification.

- ulValue: Notification value to send.

- eAction: Operation type of the notification value.

- eSetBits: Set the specified bit of the notification value.

- eIncrement: Increment the notification value.

- eSetValueWithOverwrite: Overwrite the notification value.

- eSetValueWithoutOverwrite: Do not overwrite the current value if the notification value is not processed.

**Return Value**:

- pdPASS: Operation successful.

- pdFAIL：Operation failed (usually occurs in eSetValueWithoutOverwrite).

**Example**:

```c
xTaskNotify(xTaskHandle, 0x01, eSetBits); // Set the 0th bit of the task notification value
```

### xTaskNotifyGive

**Function**: Sends a "semaphore-style" notification to a task, equivalent to the eIncrement mode of xTaskNotify().

**Prototype**:

```c
void xTaskNotifyGive(TaskHandle_t xTaskToNotify);
```

**Parameters**:

- xTaskToNotify: Task handle to receive the notification.   

Example:

```c
xTaskNotifyGive(xTaskHandle); // Increment the task notification value
```

### ulTaskNotifyTake

**Function**: Waits for task notifications and automatically decrements the notification value (usually used for counting semaphores).

**Prototype**:

```c
uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait);
```

**Parameters**:

- xClearCountOnExit:
  pdTRUE：Clear the notification value when exiting the wait.
  pdFALSE：Retain the remaining notification value when exiting the wait.

- xTicksToWait: Maximum wait time.

**Return Value**: Retur the notification value. If it is counting semaphore, the return value is the remaining notification value.

**Example**:

```c
if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0) {
    // Process the task after receiving the notification
}
```

###  xTaskNotifyWait

**Function**: Waits for task notifications, can choose to get the notification value, and decide whether to clear the notification value.

**Prototype**:

```c
BaseType_t xTaskNotifyWait(
    uint32_t ulBitsToClearOnEntry,
    uint32_t ulBitsToClearOnExit,
    uint32_t *pulNotificationValue,
    TickType_t xTicksToWait
);
```

**Parameters**:

- ulBitsToClearOnEntry: Clear the specified bit of the notification value when entering the wait.

- ulBitsToClearOnExit: Clear the specified bit of the notification value when exiting the wait.

- pulNotificationValue: Pointer to save the notification value.

- xTicksToWait: Maximum wait time.

**Return Value**:

- pdTRUE: Received the notification.

- pdFALSE: Timeout without receiving the notification.

**Example**:

```c
uint32_t ulNotificationValue;
if (xTaskNotifyWait(0x00, 0xFFFFFFFF, &ulNotificationValue, portMAX_DELAY) == pdTRUE) {
    // Process the task after receiving the notification
}
```

### xTaskNotifyStateClear

**Function**: Clear the notification state of the task.

**Prototype**:

```c
void vTaskNotifyStateClear(TaskHandle_t xTask);
```

**Parameters**:

- xTask: Task handle to clear the notification state.

**Example**:

```c
vTaskNotifyStateClear(xTaskHandle); // Clear the notification state of the task
```

## 2. Example Code:

### 1. Direct Task Notification

task2 controls the execution of task1:

```c
// Direct Task Notification
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

    while (1)
    {
        ESP_LOGI(TAG, "task1: Waiting for task notification");
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ESP_LOGI(TAG, "task1: Received task notification");
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
        ESP_LOGI(TAG, "task2: Sending task notification");
        xTaskNotifyGive(xTask1);
    }
}

void app_main(void)
{

    xTaskCreate(task1, "task1", 1024 * 2, NULL, 1, &xTask1);
    xTaskCreate(task2, "task2", 1024 * 2, NULL, 1, &xTask2);
}
```

### 2. Task Notification Value

Task notification judged by bit - to replace queue mailbox or event group:

```c
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

```