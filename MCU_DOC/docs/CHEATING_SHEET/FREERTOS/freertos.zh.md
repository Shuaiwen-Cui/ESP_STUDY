# FREERTOS

## 任务创建

### xTaskCreatePinnedToCore

> 函数原型

```c
BaseType_t xTaskCreatePinnedToCore(TaskFunction_t pxTaskCode, // 任务函数指针
                                   const char * const pcName, // 任务名称
                                   const uint32_t usStackDepth, // 任务栈大小
                                   void * const pvParameters, // 任务参数
                                   UBaseType_t uxPriority, // 任务优先级
                                   TaskHandle_t * const pxCreatedTask, // 任务句柄
                                   const BaseType_t xCoreID); // 核心ID
```
> 函数简介

在ESP-IDF（ESP32开发框架）中，`xTaskCreatePinnedToCore` 是一个用于创建任务并将任务固定到指定CPU核心的函数。ESP32是一款双核微控制器（ESP32-S系列是单核），它具有两个处理核心：Core 0 和 Core 1。这个函数可以帮助开发者将任务固定在特定的核心上运行，以便更好地管理CPU负载和实时性能。

> 参数

- `pxTaskCode`：任务函数指针。它指向要在新任务中执行的函数，该函数的定义格式通常为 `void function_name(void *pvParameters)`，其中 `pvParameters` 是传递给任务的参数。
- `pcName`：任务的名称字符串，通常用于调试和监控。可以使用一个方便识别的字符串名称。
- `usStackDepth`：任务栈的大小，以字（4字节）为单位。栈大小应根据任务中需要的栈空间来设置。
- `pvParameters`：传递给任务函数的参数指针。如果任务函数不需要参数，可以设为 `NULL`。
- `uxPriority`：任务的优先级。数值越大，优先级越高。ESP-IDF中优先级可以设置的范围是 0 到 24。
- `pxCreatedTask`：任务句柄的指针，用于接收创建任务的句柄。如果不需要任务句柄，可以将其设为`NULL`。
- `xCoreID`：`0`：绑定到核心0（主核心）。`1`：绑定到核心1。`tskNO_AFFINITY`：不绑定到特定核心，允许FreeRTOS在任一核心上调度任务。

> 返回值

- `pdPASS`：任务创建成功。
- `pdFAIL`：任务创建失败。

> 示例

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 定义任务函数
void myTask(void *pvParameters)
{
    while(1)
    {
        printf("Hello from myTask on Core 0\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // 延时1秒
    }
}

void app_main(void)
{
    // 创建任务并将其固定在核心0
    xTaskCreatePinnedToCore(
        myTask,          // 任务函数
        "MyTask",        // 任务名称
        2048,            // 任务栈大小
        NULL,            // 任务参数
        5,               // 任务优先级
        NULL,            // 任务句柄
        0                // 核心ID（0表示绑定到核心0）
    );
}
```


### xTaskCreateStaticPinnedToCore

> 函数原型

```c
TaskHandle_t xTaskCreateStaticPinnedToCore(TaskFunction_t pxTaskCode, // 任务函数指针
                                           const char * const pcName, // 任务名称
                                           const uint32_t ulStackDepth, // 任务栈大小
                                           void * const pvParameters, // 任务参数
                                           UBaseType_t uxPriority, // 任务优先级
                                           StackType_t * const puxStackBuffer, // 指向栈空间的可用内存区域
                                           StaticTask_t * const pxTaskBuffer, // 指向任务描述的可用内存区域
                                           const BaseType_t xCoreID); // 核心ID
```
> 函数简介

`xTaskCreateStaticPinnedToCore` 是一个用于创建任务并将任务固定到指定CPU核心的函数。与 `xTaskCreatePinnedToCore` 不同的是，`xTaskCreateStaticPinnedToCore` 允许用户提供任务的栈空间和任务描述符的内存区域。这样可以更好地控制任务的内存分配和管理。

> 参数

- `pxTaskCode`：任务函数指针。它指向要在新任务中执行的函数，该函数的定义格式通常为 `void function_name(void *pvParameters)`，其中 `pvParameters` 是传递给任务的参数。
- `pcName`：任务的名称字符串，通常用于调试和监控。可以使用一个方便识别的字符串名称。
- `ulStackDepth`：任务栈的大小，以字（4字节）为单位。栈大小应根据任务中需要的栈空间来设置。
- `pvParameters`：传递给任务函数的参数指针。如果任务函数不需要参数，可以设为 `NULL`。
- `uxPriority`：任务的优先级。数值越大，优先级越高。ESP-IDF中优先级可以设置的范围是 0 到 24。
- `puxStackBuffer`：指向静态分配的任务栈的指针。栈内存需提前分配并传入函数，任务会使用这块内存而不是动态分配栈空间。
- `pxTaskBuffer`：指向静态任务控制块的指针。控制块用于FreeRTOS管理任务信息（例如状态、优先级、栈指针等）。此控制块也需提前分配。
- `xCoreID`：`0`：绑定到核心0（主核心）。`1`：绑定到核心1。`tskNO_AFFINITY`：不绑定到特定核心，允许FreeRTOS在任一核心上调度任务。

> 返回值

- `pdPASS`：任务创建成功。
- `pdFAIL`：任务创建失败。

> 示例

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 定义任务栈和任务控制块的静态内存
static StackType_t myTaskStack[2048];      // 任务栈大小为2048字节
static StaticTask_t myTaskBuffer;          // 任务控制块

// 定义任务函数
void myTask(void *pvParameters)
{
    while(1)
    {
        printf("Running myTask on Core 1\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // 延时1秒
    }
}

void app_main(void)
{
    // 使用静态内存创建任务并将其绑定到核心0
    xTaskCreateStaticPinnedToCore(
        myTask,            // 任务函数
        "MyStaticTask",    // 任务名称
        2048,              // 任务栈大小
        NULL,              // 任务参数
        5,                 // 任务优先级
        myTaskStack,       // 静态栈内存
        &myTaskBuffer,     // 静态任务控制块
        0                  // 核心ID（0表示绑定到核心0）
    );
}
```

## 延时与阻塞

### vTaskDelay

> 函数原型

```c
void vTaskDelay(const TickType_t xTicksToDelay);
```

> 函数简介

`vTaskDelay` 是一个用于延时任务执行的函数。在任务中调用 `vTaskDelay` 函数后，任务会暂停执行指定的时间，然后再继续执行。

> 参数

- `xTicksToDelay`：延时的时间，以 FreeRTOS 的时钟节拍（tick）为单位。时钟节拍的频率由 `configTICK_RATE_HZ` 宏在 `FreeRTOSConfig.h` 中定义。

> 示例

```c
void myTask(void *pvParameters)
{
    while(1)
    {
        printf("Hello from myTask\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // 延时1秒
    }
}
```

> 小结 `vTaskDelay`

- 功能：`vTaskDelay` 使任务延迟指定的时间，从当前时刻开始计时。这意味着每次调用 `vTaskDelay` 时，延时的起点都是当前时刻。
- 适用场景：适合任务需要根据上一次的调用时间（无论何时调用）来延迟固定时间。

### vTaskDelayUntil

> 函数原型

```c
void vTaskDelayUntil(TickType_t *pxPreviousWakeTime, const TickType_t xTimeIncrement);
```

> 函数简介

`vTaskDelayUntil` 是一个用于周期性任务执行的函数。在任务中调用 `vTaskDelayUntil` 函数后，任务会在指定的时间间隔内周期性执行。

> 参数

- `pxPreviousWakeTime`：指向上一次唤醒时间的指针。在第一次调用时，应该传入一个指向 0 的指针。
- `xTimeIncrement`：时间间隔，以 FreeRTOS 的时钟节拍（tick）为单位。时钟节拍的频率由 `configTICK_RATE_HZ` 宏在 `FreeRTOSConfig.h` 中定义。
  
> 示例

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

> 小结 `vTaskDelay`

- 功能：`vTaskDelayUntil` 使用一个“绝对时间”概念进行延时，它允许任务**精确控制执行周期**。延时从一个固定的基准点开始计时，这样可以确保任务按固定的间隔时间执行，即使任务中有其他计算开销。
- 适用场景：适合周期性任务，确保任务精确地每隔固定周期执行一次，避免因其他任务或代码造成的延时累积。

!!! tip
    在 FreeRTOS 中，时钟节拍的频率由 `configTICK_RATE_HZ` 宏定义。且延时函数中输入时间其实是系统节拍，而非物理世界时间，所以为了方便使用，FreeRTOS 提供了 `pdMS_TO_TICKS` 宏来将毫秒转换为节拍数。
