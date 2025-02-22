# FREERTOS 多任务管理 | 实现任务并行处理

!!! note "参考出处"
    本文档参考了DURUOFU的[ESP32-教程](https://github.com/DuRuofu/ESP32-Guide)。 

注意：与原生 FreeRTOS 不同，在 ESP-IDF 中使用 FreeRTOS 的用户 \永远不应调用 `vTaskStartScheduler()` 和 `vTaskEndScheduler()`。相反，ESP-IDF 会自动启动 FreeRTOS。用户必须定义一个 `void app_main(void)` 函数作为用户应用程序的入口点，并在 ESP-IDF 启动时被自动调用。

通常，用户会从 `app_main` 中启动应用程序的其他任务。`app_main` 函数可以在任何时候返回（应用终止前）。`app_main` 函数由 main 任务调用。

## 任务创建

FreeRTOS 提供了两种创建任务的方法： `xTaskCreate` 和 `xTaskCreateStatic`。

### xTaskCreate : 动态创建一个任务

当需要在运行时动态分配内存来创建任务时使用，也就是一般的正常情况。

原型：

```c
static inline BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, 
									 const char *const pcName, 
									 const configSTACK_DEPTH_TYPE usStackDepth, 
									 void *const pvParameters, 
									 UBaseType_t uxPriority, 
									 TaskHandle_t *const pxCreatedTask)
```

创建一个新任务并将其添加到准备运行的任务列表中。在 FreeRTOS 实现中，任务使用两块内存。第一个块用于保存任务的数据结构。任务使用第二个块作为其堆栈。如果使用 xTaskCreate () 创建任务，那么这两个内存块将在 xTaskCreate () 函数中自动分配。

参数：

- `pxTaskCode`：指向任务入口函数的指针，任务必须设计为永不返回（即以无限循环形式实现）。
- `pcName`：任务的描述性名称，主要用于调试方便，最大长度由 `configMAX_TASK_NAME_LEN` 定义（默认 16）。
- `usStackDepth`：任务栈大小（以字节为单位）。注意，这与原生 FreeRTOS 的默认单位不同。
- `pvParameters`：传递给任务函数的参数指针（类型为void指针）。
- `uxPriority`：任务的优先级（数字越大优先级越高,最低为1）。支持 MPU 的系统中，通过设置 `portPRIVILEGE_BIT` 位可创建特权任务（例如 `(2 | portPRIVILEGE_BIT)` 表示优先级为 2 的特权任务）。
- `pxCreatedTask`：用于存储任务句柄（可选），通过句柄可以引用创建的任务。

返回值:

- `pdPASS`：任务创建成功。
- `errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`：任务创建失败（内存不足）

### xTaskCreateStatic：静态创建一个任务

手动提供任务栈和任务控制块（TCB），避免动态内存分配。

**原型：**

``` c
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

**参数：**

- `pvTaskCode`：任务函数的入口地址。
- `pcName`：任务名称（用于调试）。
- `usStackDepth`：任务栈大小（以字为单位）。
- `pvParameters`：传递给任务函数的参数。
- `uxPriority`：任务的优先级。
- `puxStackBuffer`：指向任务栈缓冲区的指针（由用户提供）。
- `pxTaskBuffer`：指向任务控制块的缓冲区（由用户提供）。

**返回值:**

- `pdPASS`：任务创建成功。
- `errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`：任务创建失败（内存不足）

### 代码示例

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

## 任务删除

任务可以通过调用 `vTaskDelete` 函数删除自身或其他任务。

### vTaskDelete：删除一个任务

当任务完成其功能后，需要释放资源，或当系统需要动态调整任务时使用
注意：调用后，任务进入删除 状态，但动态分配的内存需要由 FreeRTOS 自动释放。

**原型：**

```c
void vTaskDelete( TaskHandle_t xTask );
```

**参数说明：**

- xTask：要删除的任务句柄。如果传递 NULL，则删除当前任务。

#### 删除别人

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

#### 自我删除

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

## 任务创建时传递参数

### 介绍

在 FreeRTOS 中，任务函数的参数通过创建任务时的 pvParameters 指针传递。pvParameters 是一个 void * 类型的指针，可以传递任意类型的数据（整型、数组、结构体或字符串等）。**任务接收到参数后，需要将其强制类型转换为对应的数据类型，**以便正确使用。

### 代码示例

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

## 任务优先级

FreeRTOS 中每个任务都有一个优先级，优先级决定了任务的执行顺序。优先级数值较大的任务具有更高的优先级，会在低优先级任务之前被调度执行。当多个任务具有相同优先级时，调度器会使用时间片轮转机制在这些任务之间分配 CPU 时间。

关于任务优先级可参考文档：[任务优先级](https://www.freertos.org/zh-cn-cmn-s/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/03-Task-priorities)，文档中提到：

>每个任务均被分配了从 0 到 `( configMAX_PRIORITIES - 1 )` 的优先级，其中 `configMAX_PRIORITIES` 定义为 FreeRTOSConfig.h。

在ESP-IDF中configMAX_PRIORITIES的值为25，所以任务优先级为0-24.

如果我们创建任务时设定优先级为25

```c
xTaskCreate(Task_1, "Task_1", 2048, NULL, 25, &taskHandle);
```

则会报错，因为优先级超出了范围。

任务优先级涉及到`uxTaskPriorityGet`和`vTaskPrioritySet`两个函数，分别用于获取和设置任务的优先级。

### uxTaskPriorityGet：获取任务的优先级

该函数用于获取指定任务的当前优先级。如果任务句柄为 `NULL`，则返回当前任务的优先级。
**原型：**

```c
UBaseType_t uxTaskPriorityGet( TaskHandle_t xTask );
```

**参数说明：**
- xTask：任务句柄。如果为 NULL，则返回当前任务的优先级。

**返回值**：任务的优先级。

### vTaskPrioritySet：设置任务的优先级

该函数用于设置指定任务的优先级。如果任务句柄为 NULL，则设置当前任务的优先级.

**原型：**

```c
void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxPriority );
```

**参数说明：**

- xTask：任务句柄。如果为 NULL，则设置当前任务的优先级。
- uxPriority：要设置的优先级值。

### 代码示例    

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

## 任务挂起和恢复

FreeRTOS 任务挂起是指暂停任务的执行，直到通过显式恢复操作再次启动任务。挂起操作不会影响任务所占用的资源，仅是暂停任务调度。这部分涉及到`vTaskSuspend`和`vTaskResume`两个函数。

### vTaskSuspend：挂起任务

`vTaskSuspend()` 用于挂起指定任务，任务被挂起后无法再执行，直到通过 `xTaskResume()` 恢复任务。

**原型：**

```c
void vTaskSuspend(TaskHandle_t xTask);
```

**参数说明：**

- xTask：要挂起的任务句柄。如果传递 NULL，则挂起当前任务。

### xTaskResume：恢复任务

用于恢复一个挂起的任务。恢复任务后，任务重新进入准备就绪状态，等待调度器调度。

**原型：**

```c
BaseType_t xTaskResume(TaskHandle_t xTask);
```

**参数说明：**
- xTask：要恢复的任务句柄。如果传递 NULL，则恢复当前任务。

### 代码示例

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

## 系统任务信息显示

FreeRTOS 提供了多种方法来显示和分析任务信息，帮助开发者了解系统运行状况、优化性能以及定位问题。

系统任务信息显示主要通过 `vTaskList` 和 `uxTaskGetStackHighWaterMark` 函数实现。

### vTaskList: 输出任务列表

可通过 `vTaskList()` 来协助分析操作系统当前 task 状态，以帮助优化内存，帮助定位栈溢出问题，帮助理解和学习操作系统原理相关知识。

**原型：**
```c
void vTaskList( char *pcWriteBuffer );
```

**注意：**`configUSE_TRACE_FACILITY`和`configUSE_STATS_FORMATTING_FUNCTIONS`必须在 FreeRTOSConfig.h 中定义为 1，才可使用此函数。 
对于ESP32来说。使用 vTaskList() 前需使能:

menuconfig -> Component config -> FreeRTOS -> Kernel->`configUSE_TRACE_FACILITY`
menuconfig -> Component config -> FreeRTOS -> Kernel->`configUSE_STATS_FORMATTING_FUNCTIONS`

### uxTaskGetStackHighWaterMark:获取任务最小剩余栈空间

用于获取任务在运行期间的最小剩余栈空间（即栈的高水位标记）。此函数可帮助检测任务是否存在栈溢出的风险。

**原型**：

```c
UBaseType_t uxTaskGetStackHighWaterMark( TaskHandle_t xTask );
```

**参数：**

- xTask：任务句柄。如果传递 NULL，则返回当前任务的栈高水位标记。

**返回值：** 剩余栈空间的字数（单位为字）,(但是在ESP-IDF里为字节)

### 代码示例

```c
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

// Task 1: Periodically prints logs
void Task_1(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
        ESP_LOGI(TAG, "Task_1");
    }
    vTaskDelete(NULL); // Delete the task (this line is unreachable in this example)
}

// Task 2: Periodically prints logs
void Task_2(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
        ESP_LOGI(TAG, "Task_2");
    }
    vTaskDelete(NULL); // Delete the task (this line is unreachable in this example)
}

void app_main(void)
{
    TaskHandle_t taskHandle_1 = NULL; // Handle for Task 1
    TaskHandle_t taskHandle_2 = NULL; // Handle for Task 2

    // Create Task 1 with a stack size of 2048 bytes and priority 12
    xTaskCreate(Task_1, "Task_1", 2048, NULL, 12, &taskHandle_1);
    // Create Task 2 with a stack size of 2048 bytes and priority 12
    xTaskCreate(Task_2, "Task_2", 2048, NULL, 12, &taskHandle_2);

    // Print the task list
    static char cBuffer[512] = {0}; // Buffer to store task list
    vTaskList(cBuffer); // Populate the buffer with task list information
    ESP_LOGI(TAG, "Task List:\n%s", cBuffer); // Log the task list

    while (1)
    {
        // Get the remaining stack space for Task 1
        int istack = uxTaskGetStackHighWaterMark(taskHandle_1);
        ESP_LOGI(TAG, "Task_1 Remaining Stack Space: %d", istack); // Log the stack space
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
}
```