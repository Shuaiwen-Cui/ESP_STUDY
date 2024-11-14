# FREERTOS

## TASK CREATION

### xTaskCreatePinnedToCore

> Function prototype

```c
BaseType_t xTaskCreatePinnedToCore(TaskFunction_t pxTaskCode, // Task function pointer
const char * const pcName, // Task name
const uint32_t usStackDepth, // Task stack size
void * const pvParameters, // Task parameters
UBaseType_t uxPriority, // Task priority
TaskHandle_t * const pxCreatedTask, // Task handle
const BaseType_t xCoreID); // Core ID
```
> Function introduction

In ESP-IDF (ESP32 Development Framework), `xTaskCreatePinnedToCore` is a function used to create a task and pin it to a specified CPU core. ESP32 is a dual-core microcontroller (ESP32-S series is single-core) with two processing cores: Core 0 and Core 1. This function helps developers pin tasks to specific cores for better CPU load management and real-time performance management.

> Parameters

- `pxTaskCode`: Task function pointer. It points to the function to be executed in the new task, which is usually defined in the format of `void function_name(void *pvParameters)`, where `pvParameters` are the parameters passed to the task.
- `pcName`: The name string of the task, usually used for debugging and monitoring. You can use a string name that is easy to identify.
- `usStackDepth`: The size of the task stack in words (4 bytes). The stack size should be set according to the stack space required in the task.
- `pvParameters`: The parameter pointer passed to the task function. If the task function does not require parameters, it can be set to `NULL`.
- `uxPriority`: The priority of the task. The larger the value, the higher the priority. The range of priority that can be set in ESP-IDF is 0 to 24.
- `pxCreatedTask`: The pointer to the task handle, used to receive the handle of the created task. If a task handle is not needed, it can be set to `NULL`.
- `xCoreID`: `0`: bound to core 0 (primary core). `1`: bound to core 1. `tskNO_AFFINITY`: not bound to a specific core, allowing FreeRTOS to schedule the task on any core.

> Return Value

- `pdPASS`: Task creation was successful.
- `pdFAIL`: Task creation failed.

> Example

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define task function
void myTask(void *pvParameters)
{
while(1)
{
printf("Hello from myTask on Core 0\n");
vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
}
}

void app_main(void)
{
// Create a task and pin it to core 0
xTaskCreatePinnedToCore(
myTask, // Task function
"MyTask", // Task name
2048, // Task stack size
NULL, // Task parameters
5, // Task priority
NULL, // Task handle
0 // Core ID (0 means bound to core 0)
);
}
```

### xTaskCreateStaticPinnedToCore

> Function prototype

```c
TaskHandle_t xTaskCreateStaticPinnedToCore(TaskFunction_t pxTaskCode, // Task function pointer
const char * const pcName, // Task name
const uint32_t ulStackDepth, // Task stack size
void * const pvParameters, // Task parameters
UBaseType_t uxPriority, // Task priority
StackType_t * const puxStackBuffer, // Points to the available memory area of ​​the stack space
StaticTask_t * const pxTaskBuffer, // Points to the available memory area of ​​the task description
const BaseType_t xCoreID); // Core ID
```
> Function introduction

`xTaskCreateStaticPinnedToCore` is a function used to create a task and pin the task to a specified CPU core. Unlike `xTaskCreatePinnedToCore`, `xTaskCreateStaticPinnedToCore` allows the user to provide the task's stack space and the memory area of ​​the task descriptor. This allows for better control over the memory allocation and management of the task.

> Parameters

- `pxTaskCode`: Task function pointer. It points to the function to be executed in the new task, which is usually defined in the format of `void function_name(void *pvParameters)`, where `pvParameters` are the parameters passed to the task.
- `pcName`: The name string of the task, usually used for debugging and monitoring. You can use a string name that is easy to identify.
- `ulStackDepth`: The size of the task stack, in words (4 bytes). The stack size should be set according to the stack space required in the task.
- `pvParameters`: The parameter pointer passed to the task function. If the task function does not require parameters, it can be set to `NULL`.
- `uxPriority`: The priority of the task. The larger the value, the higher the priority. The priority in ESP-IDF can be set in the range of 0 to 24.
- `puxStackBuffer`: Pointer to the statically allocated task stack. The stack memory needs to be allocated in advance and passed to the function. The task will use this memory instead of dynamically allocated stack space.
- `pxTaskBuffer`: Pointer to the static task control block. The control block is used by FreeRTOS to manage task information (such as status, priority, stack pointer, etc.). This control block also needs to be allocated in advance.
- `xCoreID`: `0`: Bound to core 0 (primary core). `1`: Bound to core 1. `tskNO_AFFINITY`: Not bound to a specific core, allowing FreeRTOS to schedule tasks on any core.

> Return value

- `pdPASS`: Task creation is successful.
- `pdFAIL`: Task creation failed.

> Example

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define static memory for task stack and task control block
static StackType_t myTaskStack[2048]; // Task stack size is 2048 bytes
static StaticTask_t myTaskBuffer; // Task control block

// Define task function
void myTask(void *pvParameters)
{
while(1)
{
printf("Running myTask on Core 1\n");
vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
}
}

void app_main(void)
{
// Create a task using static memory and bind it to core 0
xTaskCreateStaticPinnedToCore(
myTask, // Task function
"MyStaticTask", // Task name
2048, // Task stack size
NULL, // Task parameters
5, // Task priority
myTaskStack, // Static stack memory
&myTaskBuffer, // Static task control block
0 // Core ID (0 means bound to core 0)
);
}
```

## DELAY AND BLOCKING

### vTaskDelay

> Function prototype

```c
void vTaskDelay(const TickType_t xTicksToDelay);
```

> Function introduction

`vTaskDelay` is a function used to delay task execution. After calling the `vTaskDelay` function in a task, the task will pause for the specified time and then continue to execute.

> Parameters

- `xTicksToDelay`: Delay time, in FreeRTOS clock ticks. The frequency of the clock tick is defined by the `configTICK_RATE_HZ` macro in `FreeRTOSConfig.h`.

> Example

```c
void myTask(void *pvParameters)
{
while(1)
{
printf("Hello from myTask\n");
vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
}
}
```

> Summary `vTaskDelay`

- Function: `vTaskDelay` delays a task for a specified time, starting from the current time. This means that each time `vTaskDelay` is called, the starting point of the delay is the current time.
- Applicable scenarios: Suitable for tasks that need to be delayed for a fixed time based on the last call time (regardless of when it was called).

### vTaskDelayUntil

> Function prototype

```c
void vTaskDelayUntil(TickType_t *pxPreviousWakeTime, const TickType_t xTimeIncrement);
```

> Function introduction

`vTaskDelayUntil` is a function for periodic task execution. After calling the `vTaskDelayUntil` function in a task, the task will be executed periodically at the specified time interval.

> Parameters

- `pxPreviousWakeTime`: Pointer to the last wake-up time. When called for the first time, a pointer to 0 should be passed in.
- `xTimeIncrement`: time interval in FreeRTOS ticks. The tick frequency is defined in `FreeRTOSConfig.h` by the `configTICK_RATE_HZ` macro.

> Example

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

void taskA(void *param)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(500); // 500 ms
    // Initialize last wakeup time
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        ESP_LOGI("main","Hello world from CSW!");

        // Wait for next cycle
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


/**
 * @brief Entry point of the program
 * @param None
 * @retval None
 */
void app_main(void)
{
    xTaskCreatePinnedToCore(taskA, "helloworld", 2048, NULL, 3, NULL, tskNO_AFFINITY);
}

```

> Summary `vTaskDelay`

- Function: `vTaskDelayUntil` uses an "absolute time" concept for delay, which allows tasks to **precisely control the execution period**. The delay starts from a fixed reference point, which ensures that the task is executed at a fixed interval, even if there are other computational overheads in the task.
- Applicable scenarios: Suitable for periodic tasks, ensuring that the task is executed accurately at fixed intervals to avoid delay accumulation caused by other tasks or code.

!!! tip
    In FreeRTOS, the frequency of the clock tick is defined by the `configTICK_RATE_HZ` macro. And the input time in the delay function is actually the system tick, not the physical world time, so for ease of use, FreeRTOS provides the `pdMS_TO_TICKS` macro to convert milliseconds to ticks.

## Inter-task synchronization

!!! note
Synchronization in RTOS refers to the collaborative working method between different tasks or between tasks and external events to ensure that multiple concurrently executed tasks are executed in the expected order or timing. "It involves the communication and coordination mechanism between threads or tasks, the purpose is to avoid data competition, solve race conditions, and ensure the correct behavior of the system.

!!! note
Mutual exclusion means that a resource is only allowed to be accessed by one visitor at a time, which is unique and exclusive.

!!! note
A queue is a data structure used to transfer data between tasks. A queue is a first-in-first-out (FIFO) data structure. Tasks can put data into the queue and take data out of the queue.

## Queue

### xQueueCreate

> Function prototype

```c
QueueHandle_t xQueueCreate(const UBaseType_t uxQueueLength, // Queue length
const UBaseType_t uxItemSize); // Size of each element in the queue
```
> Function introduction

`xQueueCreate` is a function used to create a queue. A queue is a first-in-first-out (FIFO) data structure used to pass data between tasks.

> Parameters

- `uxQueueLength`: The length of the queue, that is, the number of elements that can be stored in the queue.

- `uxItemSize`: The size of each element in the queue, in bytes.

> Return value

- Queue handle: The queue was created successfully.

- `NULL`: The queue was not created successfully.

### xQueueSend

> Function prototype

```c
BaseType_t xQueueSend(QueueHandle_t xQueue, // Queue handle
const void *pvItemToQueue, // Data pointer to be sent to the queue, copied to the queue
TickType_t xTicksToWait); // Waiting time
```

> Function introduction

`xQueueSend` is a function used to send data to a queue. After calling the `xQueueSend` function, the data will be sent to the queue.

> Parameters

- `xQueue`: Queue handle.
- `pvItemToQueue`: Data pointer to be sent to the queue.
- `xTicksToWait`: Waiting time, that is, the waiting time when the queue is full. If the queue is full, the task will wait for space to be available in the queue within the waiting time. If the waiting time is 0, the task will return immediately.

### xQueueSendToBack

> Function prototype

```c
BaseType_t xQueueSendToBack(QueueHandle_t xQueue, // Queue handle
const void *pvItemToQueue, // Data pointer to be sent to the queue, copied to the queue
TickType_t xTicksToWait); // Waiting time
```
> Function introduction

`xQueueSendToBack` is a function used to send data to a queue. After calling the `xQueueSendToBack` function, the data will be sent to the queue at the end of the queue. Suitable for situations where it is clear that the data needs to be sent to the end of the queue.

> Parameters

- `xQueue`: Queue handle.
- `pvItemToQueue`: Data pointer to be sent to the queue.
- `xTicksToWait`: Waiting time, that is, the waiting time when the queue is full. If the queue is full, the task will wait for space to be available in the queue during the waiting time. If the wait time is 0, the task will return immediately.

### xQueueReceive

> Function prototype

```c
BaseType_t xQueueReceive(QueueHandle_t xQueue, // Queue handle
void *pvBuffer, // Buffer pointer for receiving data
TickType_t xTicksToWait); // Waiting time
```

> Function introduction

`xQueueReceive` is a function for receiving data from a queue. After calling the `xQueueReceive` function, data will be received from the queue.

> Parameters

- `xQueue`: Queue handle.
- `pvBuffer`: Buffer pointer for receiving data.

### xQueueSendFromISR

> Function prototype

```c
BaseType_t xQueueSendFromISR(QueueHandle_t xQueue, // Queue handle
const void *pvItemToQueue, // Data pointer to be sent to the queue
BaseType_t *pxHigherPriorityTaskWoken); // High priority task wake-up flag
```

> Function introduction

`xQueueSendFromISR` is a function used to send data to a queue from an interrupt service routine (ISR). After calling the `xQueueSendFromISR` function, the data will be sent to the queue.

> Parameters

- `xQueue`: Queue handle.
- `pvItemToQueue`: Data pointer to be sent to the queue.
- `pxHigherPriorityTaskWoken`: High priority task wake-up flag. If a high priority task is woken up when sending data, this parameter is set to `pdTRUE`.

## Semaphore

### xSemaphoreCreateBinary

> Function prototype

```c
SemaphoreHandle_t xSemaphoreCreateBinary(void);

```

> Function introduction

`xSemaphoreCreateBinary` is a function used to create a binary semaphore. A binary semaphore is a semaphore with only two states, available and unavailable.

> Return value

- Semaphore handle: semaphore creation is successful.

- `NULL`: semaphore creation failed.

### xSemaphoreCreateCounting

> Function prototype

```c
SemaphoreHandle_t xSemaphoreCreateCounting(const UBaseType_t uxMaxCount, // Maximum count value
const UBaseType_t uxInitialCount); // Initial count value

```

> Function introduction

`xSemaphoreCreateCounting` is a function used to create a counting semaphore. A counting semaphore is a semaphore that can store multiple count values.

> Parameters

- `uxMaxCount`: maximum count value.

- `uxInitialCount`: initial count value.

> Return value

- Semaphore handle: semaphore creation is successful.

- `NULL`: semaphore creation fails.

### xSemaphoreCreateMutex

> Function prototype

```c
SemaphoreHandle_t xSemaphoreCreateMutex(void);

```

> Function introduction

`xSemaphoreCreateMutex` is a function used to create a mutex semaphore. A mutex semaphore is a semaphore used to implement mutually exclusive access and is used to protect shared resources.

> Return value

- Semaphore handle: semaphore creation is successful.

- `NULL`: semaphore creation fails.

!!! info "Mutex"
A mutex is a synchronization mechanism used to implement mutually exclusive access. In FreeRTOS, mutexes are implemented by mutex semaphores. Mutexes ensure that only one task can access a shared resource at any time. Unlike binary semaphores, mutexes implement priority inheritance and priority inversion mechanisms to ensure that tasks can access shared resources in order of priority.

!!! info "Priority Flip"
Priority flip refers to the temporary occupation of resources by low-priority tasks, which causes high-priority tasks to be blocked or even further delayed by medium-priority tasks. Ultimately, the execution of high-priority tasks is indirectly delayed by low-priority tasks, a phenomenon known as priority flip.

A typical priority flip scenario is as follows:
Suppose there are three tasks, with priorities from high to low: Task A, Task B, and Task C, and they share a resource (such as a mutex).
Step 1: Low-priority Task C acquires the resource (lock) and is using it.
Step 2: Before Task C releases the resource, high-priority Task A starts running and tries to access the same resource. But because the resource is already occupied by Task C, Task A is blocked, waiting for Task C to release the resource.
Step 3: At this time, Task B, which has a priority between the two, starts running, and because its priority is higher than Task C, Task B will preempt the execution of Task C.
Result: Due to the execution of Task B, the progress of Task C is delayed, causing Task A to be delayed as well. Even though Task A has the highest priority, it cannot be executed immediately because the medium-priority Task B indirectly blocks its execution.
This situation is called priority flipping, because the execution of the low-priority task Task C blocks the execution of the high-priority task Task A, and the intervention of Task B makes the flipping effect more serious.

Priority flipping can cause high-priority tasks of real-time systems to fail to complete on time, resulting in system performance degradation or instability. In real-time applications (such as control systems or communication systems), priority flipping can have serious consequences.

FreeRTOS and many other RTOS use priority inheritance to solve the priority flipping problem. The principle of priority inheritance mechanism is as follows:
When a low-priority task holds a resource and blocks a high-priority task, the low-priority task inherits the priority of the high-priority task until the resource is released.
In the above example, after Task C blocks the high-priority Task A, it is temporarily promoted to the priority of Task A.
In this way, Task C can continue to run before Task B and release resources as soon as possible, so that Task A can obtain resources and execute in time.
Once Task C releases resources, its priority will be restored to the original level.

### xSemaphoreTake

> Function prototype

```c
BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, // semaphore handle
TickType_t xTicksToWait); // waiting time
```

> Function introduction

`xSemaphoreTake` is a function used to obtain a semaphore. After calling the `xSemaphoreTake` function, the task will try to acquire the semaphore.

> Parameters

- `xSemaphore`: semaphore handle.

- `xTicksToWait`: waiting time, that is, the waiting time when the semaphore is not available. If the semaphore is not available, the task will wait for the semaphore to be available within the waiting time. If the waiting time is 0, the task will return immediately.

> Return value

- `pdPASS`: Successfully acquired the semaphore.

- `pdFAIL`: Failed to acquire the semaphore.

### xSemaphoreGive

> Function prototype

```c
BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore); // Semaphore handle
```

> Function introduction

`xSemaphoreGive` is a function used to release a semaphore. After calling the `xSemaphoreGive` function, the semaphore will be released.

> Parameters

- `xSemaphore`: semaphore handle.

> Return value

- `pdPASS`: Release semaphore successfully.

- `pdFAIL`: Release semaphore failed.

### xSemaphoreDelete

> Function prototype

```c
void vSemaphoreDelete(SemaphoreHandle_t xSemaphore); // semaphore handle
```

> Function introduction

`vSemaphoreDelete` is a function used to delete a semaphore. After calling the `vSemaphoreDelete` function, the semaphore will be deleted.

> Parameters

- `xSemaphore`: semaphore handle.