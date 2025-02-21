# QUEUE

!!! tip
    The header file `#include "freertos/queue.h"` must be included wherever a queue is used.

## 1. Creating and Passing Parameters to a Queue

Queues are the primary form of communication between tasks. They can be used to send messages between **tasks** and **interrupts and tasks**. In most cases, queues are used as thread-safe FIFO (first in, first out) buffers, where new data is sent to the back of the queue, but can also be sent to the front.

### 1.1 API Description 

Queue operations mainly involve the following APIs:

| Function Name           | Function        | Remarks                  |
| ---------------------- | -------------- | ---------------------- |
| xQueueCreate           | Create a queue | Create a queue of a specified length and size |
| xQueueSend             | Send data to the queue | If the queue is full, the task can choose to block or return immediately |
| xQueueReceive          | Receive data from the queue | If the queue is empty, the task can choose to block or return immediately |
| xQueueSendToFront      | Send data to the front of the queue | Similar to xQueueSend, but with higher priority |
| xQueueSendToBack       | Send data to the back of the queue | Default behavior, equivalent to xQueueSend |
| uxQueueMessagesWaiting | Query the number of messages currently waiting in the queue | Returns the number of messages in the queue that have not been read |

#### xQueueCreate: Create a Queue

**Prototype:**

```c
QueueHandle_t xQueueCreate(UBaseType_t uxQueueLength, UBaseType_t uxItemSize);
```

**Parameter Description:**
- uxQueueLength: The length of the queue (the number of elements it can hold).
- uxItemSize: The size of each element in the queue (in bytes).

**Return Value:** Returns the queue handle on success; returns NULL on failure.

**Example:**

```c
QueueHandle_t xQueue;
xQueue = xQueueCreate(10, sizeof(int)); // Create a queue that can store 10 integers
if (xQueue == NULL) {
    // Queue creation failed, handle the error
}
```

#### xQueueSend: Send Data to the Queue

**Prototype:**

```c
BaseType_t xQueueSend(QueueHandle_t xQueue, const void *pvItemToQueue, TickType_t xTicksToWait);
```

**Parameter Description:**

- xQueue: The queue handle.
- pvItemToQueue: A pointer to the data to be sent to the queue.
- xTicksToWait: The time the task waits when the queue is full (in ticks). Set to 0 to indicate no waiting.

**Return Value:** Returns pdPASS on success; returns errQUEUE_FULL if the queue is full.
- pdPASS: The data was successfully sent to the queue.
- errQUEUE_FULL：The queue is full and the data could not be sent.

**Example:** 

```c
int data = 42;
if (xQueueSend(xQueue, &data, 0) != pdPASS) {
    // Data sending failed, handle the error
}
```

#### xQueueReceive: Receive Data from the Queue

**Prototype:**

```c
BaseType_t xQueueReceive(QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait);
```

**Parameter Description:**
- xQueue: The queue handle.
- pvBuffer: A pointer to the buffer that receives the data.
- xTicksToWait: The time the task waits when the queue is empty (in ticks). Set to 0 to indicate no waiting.

**Return Value:** Returns pdPASS on success; returns errQUEUE_EMPTY if the queue is empty.
- pdPASS: The data was successfully received from the queue.
- pdFALSE: The queue is empty and the data could not be received.

**Example:** 

```c
int receivedData;
if (xQueueReceive(xQueue, &receivedData, portMAX_DELAY) == pdPASS) {
    // Data received successfully
}
```

### 1.2 Queue Example

#### (1) Passing Constants to a Queue

```c
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "main";

void Task_1(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    int i = 0;

    for (;;)
    {
        // send data to the queue
        if (xQueueSend(xQueue, &i, 0)!= pdPASS) {
            ESP_LOGI(TAG, "send data failed");
        }
        else
        {
            ESP_LOGI(TAG, "send data success, data is: %d", i);
            i++;
        }

        if(i == 10)
        {
            i = 0;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_2(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    for (;;)
    {
        int receivedData;
        if (xQueueReceive(xQueue, &receivedData, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "data receive failed");
        }
        else
        {
            ESP_LOGI(TAG, "data receive success, data is: %d", receivedData);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    TaskHandle_t taskHandle_1 = NULL;
    TaskHandle_t taskHandle_2 = NULL;
    QueueHandle_t xQueue;

    // create a queue
    xQueue = xQueueCreate(10, sizeof(int));

    if (xQueue != NULL)
    {
        ESP_LOGI(TAG, "queue create success");
        // send data task
        xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_1);
        // receive data task
        xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_2);
    }
    else
    {
        ESP_LOGI(TAG, "queue create failed");
    }
}
```

#### (2) Passing Structures to a Queue

```c
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "main";

// define a struct
typedef struct
{
    int id;
    int data[3];
} MyStruct;

void Task_1(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    MyStruct shendData = {1, {1, 2, 3}};

    for (;;)
    {
        // send data to the queue
        if (xQueueSend(xQueue, &shendData, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "data send failed");
        }
        else
        {
            ESP_LOGI(TAG, "data send success, data is: %d-[%d,%d,%d]", shendData.id, shendData.data[0], shendData.data[1], shendData.data[2]);
            shendData.id++;
        }

        if (shendData.id == 10)
        {
            shendData.id = 0;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_2(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    for (;;)
    {
        MyStruct receivedData;
        if (xQueueReceive(xQueue, &receivedData, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "data receive failed");
        }
        else
        {
            ESP_LOGI(TAG, "data receive success, data is: %d-[%d,%d,%d]", receivedData.id, receivedData.data[0], receivedData.data[1], receivedData.data[2]);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    TaskHandle_t taskHandle_1 = NULL;
    TaskHandle_t taskHandle_2 = NULL;
    QueueHandle_t xQueue;

    // create a queue
    xQueue = xQueueCreate(10, sizeof(MyStruct));

    if (xQueue != NULL)
    {
        ESP_LOGI(TAG, "queue create success");
        // send data task
        xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_1);
        // receive data task
        xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_2);
    }
    else
    {
        ESP_LOGI(TAG, "queue create failed");
    }
}
```

#### (3) Passing Pointers to a Queue

```c
// queue parameter passing pointer: generally used to pass data that occupies a large amount of memory. Passing a pointer can avoid copying data and improve efficiency.
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "main";

void Task_1(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    int i = 0;

    for (;;)
    {
        char *pCharSend = (char *)malloc(50); // allocate memory
        snprintf(pCharSend, 50, "Hello World! - %d", i);
        i++;
        // send data to the queue
        if (xQueueSend(xQueue, &pCharSend, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "data send failed");
        }
        else
        {
            ESP_LOGI(TAG, "data send success, data is: %s", pCharSend);
            
        }

        if (i == 10)
        {
            i = 0;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_2(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;

    char *pCharReceived = NULL; // receive data pointer
    for (;;)
    {
        if (xQueueReceive(xQueue, &pCharReceived, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "data receive failed");
        }
        else
        {
            ESP_LOGI(TAG, "data receive success, data is: %s", pCharReceived);
            free(pCharReceived); // release memory
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    TaskHandle_t taskHandle_1 = NULL;
    TaskHandle_t taskHandle_2 = NULL;
    QueueHandle_t xQueue;

    // create a queue
    xQueue = xQueueCreate(10, sizeof(char *));

    if (xQueue != NULL)
    {
        ESP_LOGI(TAG, "queue create success");
        // send data task
        xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_1);
        // receive data task
        xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_2);
    }
    else
    {
        ESP_LOGI(TAG, "queue create failed");
    }
}
```

## 2. Queue Multi-In-Single-Out Model 

!!! info
    Actually, for MISO model, there are several tasks sending data to the queue, and only one task receiving data from the queue. This model is typically used for data listening.

Example Code:

```c

// Task 1 and Task 2 send data to the queue, and Task 3 receives data. The priority of Task 3 is higher than that of Task 1 and Task 2, so as to achieve the purpose of data listening.

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "main";

void Task_1(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    int i = 111;

    for (;;)
    {
        // send data to the queue
        if (xQueueSend(xQueue, &i, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "Task 1 data send failed");
        }
        else
        {
            ESP_LOGI(TAG, "Task 1 data send success, data is: %d", i);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_2(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    int i = 222;

    for (;;)
    {
        // send data to the queue
        if (xQueueSend(xQueue, &i, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "Task 2 data send failed");   
        }
        else
        {
            ESP_LOGI(TAG, "Task 2 data send success, data is: %d", i);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_3(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    for (;;)
    {
        int receivedData;
        // use portMAX_DELAY to block and wait for data
        if (xQueueReceive(xQueue, &receivedData, portMAX_DELAY) != pdPASS)
        {
            ESP_LOGI(TAG, "Task 3 data receive failed");
        }
        else
        {
            ESP_LOGI(TAG, "Task 3 data receive success, data is: %d", receivedData);
        }
    }
    vTaskDelete(NULL);
}

void app_main(void)
{

    QueueHandle_t xQueue;

    // create a queue
    xQueue = xQueueCreate(10, sizeof(int));

    if (xQueue != NULL)
    {
        ESP_LOGI(TAG, "Queue create success");
        // send data task
        xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 1, NULL);
        xTaskCreate(Task_2, "Task_2", 1024 * 4, (void *)xQueue, 1, NULL);
        // receive data
        xTaskCreate(Task_3, "Task_3", 1024 * 4, (void *)xQueue, 2, NULL);
    }
    else
    {
        ESP_LOGI(TAG, "Queue create failed");
    }
}
```

## 3. Queue Set

!!! note
    Queue set is a mechanism to listen to multiple queues at the same time. When data is sent to any queue in the queue set, the task will receive the data.

### 3.1 API Description

Queue set operations mainly involve the following APIs:

| Function Name           | Function        | Remarks                  |
| ---------------------- | -------------- | ---------------------- |
| xQueueCreateSet        | Create a queue set | Create a queue set |
| xQueueAddToSet         | Add a queue to the queue set | Add a queue to the queue set |
| xQueueRemoveFromSet | Remove a queue from the queue set | Remove a queue from the queue set |
| xQueueSelectFromSet    | Listen to the queue set | Listen to the queue set |

### 3.2 Queue Set Example

```c
// 队列集合:
// https : // www.bilibili.com/video/BV1zq4y1m7UK?spm_id_from=333.788.videopod.sections&vd_source=ef5a0ab0106372751602034cdd9ab98e

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "main";

void Task_1(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    int i = 111;

    for (;;)
    {
        // send data to the queue
        if (xQueueSend(xQueue, &i, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "Task 1 data send failed");
        }
        else
        {
            ESP_LOGI(TAG, "Task 1 data send success, data is: %d", i);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_2(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    int i = 222;

    for (;;)
    {
        // send data to the queue
        if (xQueueSend(xQueue, &i, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "Task 2 data send failed");
        }
        else
        {
            ESP_LOGI(TAG, "Task 2 data send success, data is: %d", i);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_3(void *pvParameters)
{
    // get the queue set handle
    QueueSetHandle_t xQueueSet = (QueueSetHandle_t)pvParameters;
    // get the queue handle from the queue with data
    QueueSetMemberHandle_t QueueData;
    for (;;)
    {
        QueueData = xQueueSelectFromSet(xQueueSet, portMAX_DELAY);
        if (QueueData != NULL)
        {
            int i;
            if (xQueueReceive(QueueData, &i, portMAX_DELAY) != pdPASS)
            {
                ESP_LOGI(TAG, "Task 3 data receive failed");
            }
            else
            {
                ESP_LOGI(TAG, "Task 3 data receive success, data is: %d, and the queue is: %p", i, QueueData);
            }
        }
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    QueueHandle_t xQueue_1;
    QueueHandle_t xQueue_2;
    
    // create a queue
    xQueue_1 = xQueueCreate(10, sizeof(int));
    xQueue_2 = xQueueCreate(10, sizeof(int));

    // create a queue set
    QueueSetHandle_t xQueueSet;
    xQueueSet = xQueueCreateSet(20);

    // add the queue to the queue set
    xQueueAddToSet(xQueue_1, xQueueSet);
    xQueueAddToSet(xQueue_2, xQueueSet);

    if ((xQueue_1 != NULL )&& (xQueue_2 != NULL) && (xQueueSet != NULL))
    {
        ESP_LOGI(TAG, "queue create success");
        // send data task
        xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue_1, 1, NULL);
        xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue_2, 1, NULL);
        // receive data
        xTaskCreate(Task_3, "Task_1", 1024 * 4, (void *)xQueueSet, 2, NULL);
    }
    else
    {
        ESP_LOGI(TAG, "queue create failed");
    }
}
```



## 4. Queue Mailbox

The mailbox in FreeRTOS is different from other RTOSs, it is a queue but the length is only 1.

Write Mailbox: Overwrite the data in the mailbox, if the mailbox is full, the data will be overwritten.
Read Mailbox: Read the data in the mailbox, if the mailbox is empty, the task will block and wait for the data.


```c

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "main";

// write data queue
void Task_1(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t Mailbox = (QueueHandle_t)pvParameters;
    int i = 0;

    for (;;)
    {
        // send data
        if (xQueueOverwrite(Mailbox, &i) != pdPASS)
        {
            ESP_LOGI(TAG, "Task 1 data send failed");
        }
        else
        {
            ESP_LOGI(TAG, "Task 1 data send success, data is: %d", i);
        }
        i++;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


// read data queue
void Task_2(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t Mailbox = (QueueHandle_t)pvParameters;

    int i = 0;

    for (;;)
    {
        // read data
        if (xQueuePeek(Mailbox, &i, portMAX_DELAY) == pdPASS)
        {
            ESP_LOGI(TAG, "Task 2 data read success, data is: %d", i);
        }
        else
        {
            ESP_LOGI(TAG, "Task 2 data read failed");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_3(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t Mailbox = (QueueHandle_t)pvParameters;
    int i = 0;

    for (;;)
    {
        // read data
        if (xQueuePeek(Mailbox, &i, portMAX_DELAY) == pdPASS)
        {
            ESP_LOGI(TAG, "Task 3 data read success, data is: %d", i);
        }
        else
        {
            ESP_LOGI(TAG, "Task 3 data read failed");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_4(void *pvParameters)
{
    // get the queue handle
    QueueHandle_t Mailbox = (QueueHandle_t)pvParameters;
    int i = 0;

    for (;;)
    {
        // read data
        if (xQueuePeek(Mailbox, &i, portMAX_DELAY) == pdPASS)
        {
            ESP_LOGI(TAG, "Task 4 data read success, data is: %d", i);
        }
        else
        {
            ESP_LOGI(TAG, "Task 4 data read failed");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    QueueHandle_t Mailbox; // Create a queue

    // create a queue (note: the queue length is only 1)
    Mailbox = xQueueCreate(1, sizeof(int));

    if ((Mailbox != NULL) )
    {
        ESP_LOGI(TAG, "create queue success");
        // write data task
        xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)Mailbox, 2, NULL);
        xTaskCreate(Task_2, "Task_2", 1024 * 4, (void *)Mailbox, 1, NULL);
        xTaskCreate(Task_3, "Task_3", 1024 * 4, (void *)Mailbox, 1, NULL);
        xTaskCreate(Task_4, "Task_4", 1024 * 4, (void *)Mailbox, 1, NULL);
    }
    else
    {
        ESP_LOGI(TAG, "queue create failed");
    }
}

```