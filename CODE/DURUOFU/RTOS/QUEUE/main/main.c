// QUEUE PARA PASS - CONST

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"

// static const char *TAG = "main";

// void Task_1(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     int i = 0;

//     for (;;)
//     {
//         // send data to the queue
//         if (xQueueSend(xQueue, &i, 0)!= pdPASS) {
//             ESP_LOGI(TAG, "send data failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "send data success, data is: %d", i);
//             i++;
//         }

//         if(i == 10)
//         {
//             i = 0;
//         }
//         vTaskDelay(3000 / portTICK_PERIOD_MS);
//     }
//     vTaskDelete(NULL);
// }

// void Task_2(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     for (;;)
//     {
//         int receivedData;
//         if (xQueueReceive(xQueue, &receivedData, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "data receive failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "data receive success, data is: %d", receivedData);
//         }
//         vTaskDelay(3000 / portTICK_PERIOD_MS);
        
//     }
//     vTaskDelete(NULL);
// }

// void app_main(void)
// {
//     TaskHandle_t taskHandle_1 = NULL;
//     TaskHandle_t taskHandle_2 = NULL;
//     QueueHandle_t xQueue;

//     // create a queue
//     xQueue = xQueueCreate(10, sizeof(int));

//     if (xQueue != NULL)
//     {
//         ESP_LOGI(TAG, "queue create success");
//         // send data task
//         xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_1);
//         // receive data task
//         xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_2);
//     }
//     else
//     {
//         ESP_LOGI(TAG, "queue create failed");
//     }
// }


// QUEUE PARA PASS - STRUCT

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"

// static const char *TAG = "main";

// // define a struct
// typedef struct
// {
//     int id;
//     int data[3];
// } MyStruct;

// void Task_1(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     MyStruct shendData = {1, {1, 2, 3}};

//     for (;;)
//     {
//         // send data to the queue
//         if (xQueueSend(xQueue, &shendData, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "data send failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "data send success, data is: %d-[%d,%d,%d]", shendData.id, shendData.data[0], shendData.data[1], shendData.data[2]);
//             shendData.id++;
//         }

//         if (shendData.id == 10)
//         {
//             shendData.id = 0;
//         }
//         vTaskDelay(3000 / portTICK_PERIOD_MS);
//     }
//     vTaskDelete(NULL);
// }

// void Task_2(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     for (;;)
//     {
//         MyStruct receivedData;
//         if (xQueueReceive(xQueue, &receivedData, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "data receive failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "data receive success, data is: %d-[%d,%d,%d]", receivedData.id, receivedData.data[0], receivedData.data[1], receivedData.data[2]);
//         }
//         vTaskDelay(3000 / portTICK_PERIOD_MS);
        
//     }
//     vTaskDelete(NULL);
// }

// void app_main(void)
// {
//     TaskHandle_t taskHandle_1 = NULL;
//     TaskHandle_t taskHandle_2 = NULL;
//     QueueHandle_t xQueue;

//     // create a queue
//     xQueue = xQueueCreate(10, sizeof(MyStruct));

//     if (xQueue != NULL)
//     {
//         ESP_LOGI(TAG, "queue create success");
//         // send data task
//         xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_1);
//         // receive data task
//         xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_2);
//     }
//     else
//     {
//         ESP_LOGI(TAG, "queue create failed");
//     }
// }


// QUEUE PARA PASS - POINTER

// // queue parameter passing pointer: generally used to pass data that occupies a large amount of memory. Passing a pointer can avoid copying data and improve efficiency.
// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"

// static const char *TAG = "main";

// void Task_1(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     int i = 0;

//     for (;;)
//     {
//         char *pCharSend = (char *)malloc(50); // allocate memory
//         snprintf(pCharSend, 50, "Hello World! - %d", i); 
//         i++;
//         // send data to the queue
//         if (xQueueSend(xQueue, &pCharSend, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "data send failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "data send success, data is: %s", pCharSend);
            
//         }

//         if (i == 10)
//         {
//             i = 0;
//         }
//         vTaskDelay(3000 / portTICK_PERIOD_MS);
//     }
//     vTaskDelete(NULL);
// }

// void Task_2(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;

//     char *pCharReceived = NULL; // receive data pointer
//     for (;;)
//     {
//         if (xQueueReceive(xQueue, &pCharReceived, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "data receive failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "data receive success, data is: %s", pCharReceived);
//             free(pCharReceived); // release memory
//         }
//         vTaskDelay(3000 / portTICK_PERIOD_MS);
        
//     }
//     vTaskDelete(NULL);
// }

// void app_main(void)
// {
//     TaskHandle_t taskHandle_1 = NULL;
//     TaskHandle_t taskHandle_2 = NULL;
//     QueueHandle_t xQueue;

//     // create a queue
//     xQueue = xQueueCreate(10, sizeof(char *));

//     if (xQueue != NULL)
//     {
//         ESP_LOGI(TAG, "queue create success");
//         // send data task
//         xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_1);
//         // receive data task
//         xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_2);
//     }
//     else
//     {
//         ESP_LOGI(TAG, "queue create failed");
//     }
// }


// MISO MODEL

// // Task 1 and Task 2 send data to the queue, and Task 3 receives data. The priority of Task 3 is higher than that of Task 1 and Task 2, so as to achieve the purpose of data listening.

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"

// static const char *TAG = "main";

// void Task_1(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     int i = 111;

//     for (;;)
//     {
//         // send data to the queue
//         if (xQueueSend(xQueue, &i, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "Task 1 data send failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "Task 1 data send success, data is: %d", i);
//         }
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//     vTaskDelete(NULL);
// }

// void Task_2(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     int i = 222;

//     for (;;)
//     {
//         // send data to the queue
//         if (xQueueSend(xQueue, &i, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "Task 2 data send failed");   
//         }
//         else
//         {
//             ESP_LOGI(TAG, "Task 2 data send success, data is: %d", i);
//         }
//         vTaskDelay(5000 / portTICK_PERIOD_MS);
//     }
//     vTaskDelete(NULL);
// }

// void Task_3(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     for (;;)
//     {
//         int receivedData;
//         // use portMAX_DELAY to block and wait for data
//         if (xQueueReceive(xQueue, &receivedData, portMAX_DELAY) != pdPASS)
//         {
//             ESP_LOGI(TAG, "Task 3 data receive failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "Task 3 data receive success, data is: %d", receivedData);
//         }
//     }
//     vTaskDelete(NULL);
// }

// void app_main(void)
// {

//     QueueHandle_t xQueue;

//     // create a queue
//     xQueue = xQueueCreate(10, sizeof(int));

//     if (xQueue != NULL)
//     {
//         ESP_LOGI(TAG, "Queue create success");
//         // send data task
//         xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 3, NULL);
//         xTaskCreate(Task_2, "Task_2", 1024 * 4, (void *)xQueue, 3, NULL);
//         // receive data
//         xTaskCreate(Task_3, "Task_3", 1024 * 4, (void *)xQueue, 2, NULL);
//     }
//     else
//     {
//         ESP_LOGI(TAG, "Queue create failed");
//     }
// }


// QUEUE SET

// // 队列集合:
// // https : // www.bilibili.com/video/BV1zq4y1m7UK?spm_id_from=333.788.videopod.sections&vd_source=ef5a0ab0106372751602034cdd9ab98e

// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"

// static const char *TAG = "main";

// void Task_1(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     int i = 111;

//     for (;;)
//     {
//         // send data to the queue
//         if (xQueueSend(xQueue, &i, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "Task 1 data send failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "Task 1 data send success, data is: %d", i);
//         }
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//     vTaskDelete(NULL);
// }

// void Task_2(void *pvParameters)
// {
//     // get the queue handle
//     QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
//     int i = 222;

//     for (;;)
//     {
//         // send data to the queue
//         if (xQueueSend(xQueue, &i, 0) != pdPASS)
//         {
//             ESP_LOGI(TAG, "Task 2 data send failed");
//         }
//         else
//         {
//             ESP_LOGI(TAG, "Task 2 data send success, data is: %d", i);
//         }
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//     vTaskDelete(NULL);
// }

// void Task_3(void *pvParameters)
// {
//     // get the queue set handle
//     QueueSetHandle_t xQueueSet = (QueueSetHandle_t)pvParameters;
//     // get the queue handle from the queue with data
//     QueueSetMemberHandle_t QueueData;
//     for (;;)
//     {
//         QueueData = xQueueSelectFromSet(xQueueSet, portMAX_DELAY);
//         if (QueueData != NULL)
//         {
//             int i;
//             if (xQueueReceive(QueueData, &i, portMAX_DELAY) != pdPASS)
//             {
//                 ESP_LOGI(TAG, "Task 3 data receive failed");
//             }
//             else
//             {
//                 ESP_LOGI(TAG, "Task 3 data receive success, data is: %d, and the queue is: %p", i, QueueData);
//             }
//         }
//     }
//     vTaskDelete(NULL);
// }

// void app_main(void)
// {
//     QueueHandle_t xQueue_1;
//     QueueHandle_t xQueue_2;

//     // create a queue
//     xQueue_1 = xQueueCreate(10, sizeof(int));
//     xQueue_2 = xQueueCreate(10, sizeof(int));

//     // create a queue set
//     QueueSetHandle_t xQueueSet;
//     xQueueSet = xQueueCreateSet(20);

//     // add the queue to the queue set
//     xQueueAddToSet(xQueue_1, xQueueSet);
//     xQueueAddToSet(xQueue_2, xQueueSet);

//     if ((xQueue_1 != NULL )&& (xQueue_2 != NULL) && (xQueueSet != NULL))
//     {
//         ESP_LOGI(TAG, "queue create success");
//         // send data task
//         xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue_1, 1, NULL);
//         xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue_2, 1, NULL);
//         // receive data
//         xTaskCreate(Task_3, "Task_1", 1024 * 4, (void *)xQueueSet, 2, NULL);
//     }
//     else
//     {
//         ESP_LOGI(TAG, "queue create failed");
//     }
// }


// QUEUE MAILBOX

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
