# FREERTOS MULTITASK

Note: Unlike native FreeRTOS, users using FreeRTOS in ESP-IDF should \*never\* call `vTaskStartScheduler()` and `vTaskEndScheduler()`. Instead, ESP-IDF will start FreeRTOS automatically. Users must define a `void app_main(void)` function as the entry point of the user application, which will be called automatically when ESP-IDF starts.

Usually, users will start other tasks of the application from `app_main`. The `app_main` function can return at any time (before the application terminates). The `app_main` function is called by the main task.

## Task Creation

FreeRTOS provides two methods to create tasks: `xTaskCreate` and `xTaskCreateStatic`.

### xTaskCreate: Dynamically create a task

Use when you need to create a task by dynamically allocating memory at runtime, which is the general normal case.

Prototype:

```c
static inline BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, 
                                     const char *const pcName, 
                                     const configSTACK_DEPTH_TYPE usStackDepth, 
                                     void *const pvParameters, 
                                     UBaseType_t uxPriority, 
                                     TaskHandle_t *const pxCreatedTask)
```

Create a new task and add it to the list of tasks that are ready to run. In the FreeRTOS implementation, tasks use two blocks of memory. The first block is used to store the task's data structure. The task uses the second block as its stack. If a task is created using xTaskCreate(), then these two blocks of memory will be automatically allocated within the xTaskCreate() function.

Parameters:

- `pxTaskCode`: A pointer to the task entry function. The task must be designed to never return (i.e., implemented in an infinite loop).
- `pcName`: A descriptive name for the task, mainly for debugging convenience. The maximum length is defined by `configMAX_TASK_NAME_LEN` (default 16).
- `usStackDepth`: The size of the task stack (in bytes). Note that this is different from the default unit in native FreeRTOS.
- `pvParameters`: A pointer to the parameter passed to the task function (type void pointer).
- `uxPriority`: The priority of the task (the larger the number, the higher the priority, with a minimum of 1). In systems that support MPU, privileged tasks can be created by setting the `portPRIVILEGE_BIT` bit (e.g., `(2 | portPRIVILEGE_BIT)` indicates a privileged task with a priority of 2).
- `pxCreatedTask`: Used to store the task handle (optional), which can be used to reference the created task.

Return value:

- `pdPASS`: Task created successfully.
- `errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`: Task creation failed (insufficient memory).

### xTaskCreateStatic: Statically create a task

Manually provide the task stack and task control block (TCB) to avoid dynamic memory allocation.

**Prototype:**

```c
TaskHandle_t xTaskCreateStatic(
    TaskFunction_t pvTaskCode,
    const char * const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters,
    UBaseType_t uxPriority,
    StackType_t * const puxStackBuffer,
    StaticTask_t * const pxTaskBuffer
);
```

**Parameters:**

- `pvTaskCode`: A pointer to the task entry function. The task must be designed to never return (i.e., implemented in an infinite loop).
- `pcName`: The task name (for debugging).
- `ulStackDepth`: The size of the task stack (in bytes).
- `pvParameters`: The parameter passed to the task function.
- `uxPriority`: The priority of the task.
- `puxStackBuffer`: A pointer to the stack buffer.
- `pxTaskBuffer`: A pointer to the task buffer.

**Return value:**

- `pdPASS`: Task created successfully.
- `errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`: Task creation failed (insufficient memory).

### Example Code

```c
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
    // Task entry function: myTask
    // Task name: "myTask", used for identifying the task during debugging
    // 3. Task stack size: 2048 bytes (allocate appropriately to avoid stack overflow)
    // 4. Task parameters: NULL (no parameters passed)
    // 5. Task priority: 1 (lower priority, idle task has priority 0)
    // 6. Task handle: NULL (no need to save the task handle)
    xTaskCreate(myTask, "myTask", 2048, NULL, 1, NULL);
}
```

## Task Deletion

To delete a task, use the `vTaskDelete` function. The task will be deleted immediately, and the memory used by the task will be freed.

### vTaskDelete: Delete a task

When a task completes its function and needs to release resources, or when the system needs to dynamically adjust tasks. Note, after calling `vTaskDelete`, the task will be deleted immediately, and the memory used by the task will be freed by the FreeRTOS kernel.

**Prototype:**

```c
void vTaskDelete(TaskHandle_t xTaskToDelete);
```

**Parameters:**

- xTaskToDelete: The handle of the task to be deleted.

#### Delete other task

```c
// Delete other task

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
        vTaskDelay(500 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "myTask");
    }
}

void app_main(void)
{
    // Task handle
    TaskHandle_t taskHandle = NULL;
    // Create a FreeRTOS task
    xTaskCreate(myTask, "myTask", 2048, NULL, 1, &taskHandle);

    // Delay for 2 seconds
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // Delete the task
    if (taskHandle != NULL)
    {
        vTaskDelete(taskHandle);
    }
}
```

#### Delete itself

```c
// Delete itself

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

// Task function
void myTask(void *pvParameters)
{
    // Delay for 1 second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "myTask:1");

    // Delay for another 1 second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "myTask:2");

    // Delay for another 1 second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "myTask:3");

    // Delete the task (if NULL is passed, the current task is deleted)
    vTaskDelete(NULL);
}

void app_main(void)
{
    // Task handle
    TaskHandle_t taskHandle = NULL;
    // Create a FreeRTOS task
    xTaskCreate(myTask, "myTask", 2048, NULL, 1, &taskHandle);
}
```

## Passing Parameters When Creating Tasks

When creating a task, you can pass parameters to the task function. The parameters are passed as a void pointer, and data of any type can be passed, e.g., a structure, an array, or a single variable. After receiving the parameters, the task function can cast the void pointer to the desired data type.

### Example Code

```c
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

// Define a structure to hold an integer and an array
typedef struct
{
    int Int;
    int Array[3];
} MyStruct;

// Task function 1: Accepts an integer parameter
void Task_1(void *pvParameters)
{
    int *pInt = (int *)pvParameters;
    ESP_LOGI(TAG, "Received integer parameter: %d", *pInt);
    vTaskDelete(NULL); // Delete the task after execution
}

// Task function 2: Accepts an array parameter
void Task_2(void *pvParameters)
{
    int *pArray = (int *)pvParameters;
    ESP_LOGI(TAG, "Received array parameters: %d %d %d", *pArray, *(pArray + 1), *(pArray + 2));
    vTaskDelete(NULL); // Delete the task after execution
}

// Task function 3: Accepts a structure parameter
void Task_3(void *pvParameters)
{
    MyStruct *pStruct = (MyStruct *)pvParameters;
    ESP_LOGI(TAG, "Received structure parameters: %d %d %d %d", pStruct->Int, pStruct->Array[0], pStruct->Array[1], pStruct->Array[2]);
    vTaskDelete(NULL); // Delete the task after execution
}

// Task function 4: Accepts a string parameter
void Task_4(void *pvParameters)
{
    char *pChar = (char *)pvParameters;
    ESP_LOGI(TAG, "Received string parameter: %s", pChar);
    vTaskDelete(NULL); // Delete the task after execution
}

// Global parameters to be passed to tasks
int Parameters_1 = 1;
int Parameters_2[3] = {1, 2, 3};
MyStruct Parameters_3 = {1, {1, 2, 3}};
static const char *Parameters_4 = "Hello World!";

void app_main(void)
{
    // Create Task 1 and pass an integer parameter
    xTaskCreate(Task_1, "Task_1", 2048, (void *)&Parameters_1, 1, NULL);

    // Create Task 2 and pass an array parameter
    xTaskCreate(Task_2, "Task_2", 2048, (void *)&Parameters_2, 1, NULL);

    // Create Task 3 and pass a structure parameter
    xTaskCreate(Task_3, "Task_3", 3048, (void *)&Parameters_3, 1, NULL);

    // Create Task 4 and pass a string parameter (note: no address-of operator & is used)
    xTaskCreate(Task_4, "Task_4", 3048, (void *)Parameters_4, 1, NULL);
}
```

## Task Priority

Each task in FreeRTOS has a priority, which determines the order in which tasks are executed. Tasks with larger priority values ​​have higher priority and are scheduled before lower priority tasks. When multiple tasks have the same priority, the scheduler uses a round-robin mechanism to allocate CPU time between these tasks. 

The priority range is from 0 to (configMAX_PRIORITIES - 1), where configMAX_PRIORITIES is defined in FreeRTOSConfig.h. The default value is 25. The idle task has the lowest priority (0), and the highest priority is configMAX_PRIORITIES - 1.

### uxTaskPriorityGet: Get the priority of a task

**Prototype:**

```c
UBaseType_t uxTaskPriorityGet(TaskHandle_t xTask);
```

**Parameters:**

- `xTask`: The handle of the task to query.
  
- **Return value:** The priority of the task.

### vTaskPrioritySet: Set the priority of a task

**Prototype:**

```c
void vTaskPrioritySet(TaskHandle_t xTask, UBaseType_t uxNewPriority);
```

**Parameters:**

- `xTask`: The handle of the task to set the priority.
- `uxNewPriority`: The new priority to set.

### Example Code

```c
void app_main(void)
{
    UBaseType_t taskPriority_1 = 0;
    UBaseType_t taskPriority_2 = 0;
    TaskHandle_t taskHandle_1 = NULL;
    TaskHandle_t taskHandle_2 = NULL;
    
    xTaskCreate(Task_1, "Task_1", 2048, NULL, 12, &taskHandle_1);
    taskPriority_1 = uxTaskPriorityGet(taskHandle_1);
    ESP_LOGI(TAG, "Task_1 Priority：%d", taskPriority_1);

    xTaskCreate(Task_2, "Task_1", 2048, NULL, 12, &taskHandle_2);
    taskPriority_2 = uxTaskPriorityGet(taskHandle_2);
    ESP_LOGI(TAG, "Task_1 Priority：%d", taskPriority_2);
}
```

## Task Suspend and Resume

FreeRTOS provides two functions to suspend and resume tasks: `vTaskSuspend` and `vTaskResume`.

### vTaskSuspend: Suspend a task

**Prototype:**

```c
void vTaskSuspend(TaskHandle_t xTask);
```

**Parameters:**

- `xTask`: The handle of the task to suspend.

### vTaskResume: Resume a task

**Prototype:**

```c
void vTaskResume(TaskHandle_t xTask);
```

**Parameters:**

- `xTask`: The handle of the task to resume.

### Example Code

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "main";

TaskHandle_t taskHandle_1 = NULL; // Handle for Task 1

// Task 1: Periodically prints logs
void Task_1(void *pvParameters)
{
    while (1)
    {
        ESP_LOGI(TAG, "Task 1 is running...");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

// Task 2: Controls the suspension and resumption of Task 1
void Task_2(void *pvParameters)
{
    while (1)
    {
        ESP_LOGI(TAG, "Suspending Task 1...");
        vTaskSuspend(taskHandle_1); // Suspend Task 1
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds

        ESP_LOGI(TAG, "Resuming Task 1...");
        vTaskResume(taskHandle_1); // Resume Task 1
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for another 5 seconds
    }
}

void app_main(void)
{
    // Create Task 1
    xTaskCreate(Task_1, "Task_1", 2048, NULL, 6, &taskHandle_1);

    // Create Task 2
    xTaskCreate(Task_2, "Task_2", 2048, NULL, 5, NULL);
}
```

## System Task Information Listing

FreeRTOS provides many functions to list task information to help developer to debug and monitor the system.

System information are mainly listed by `vTaskList` and `uxTaskGetStackHighWaterMark`.

### vTaskList: List task information