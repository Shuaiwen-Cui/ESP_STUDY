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

## 任务间同步

!!! note
    RTOS中的同步，是指是不同任务之间或者任务与外部事件之间的协同工作方式确保多个并发执行的任务按照预期的顺序或时机执行。"它涉及到线程或任务间的通信和协调机制，目的是为了避免数据竞争、解决竞态条件，并确保系统的正确行为。

!!! note
    互斥是指某一资源同时只允许一个访问者对其进行访问,具有唯一性和排它性。

!!! note
    队列是一种数据结构，用于在任务之间传递数据。队列是一种先进先出（FIFO）的数据结构，任务可以将数据放入队列，也可以从队列中取出数据。

## 队列

### xQueueCreate

> 函数原型

```c
QueueHandle_t xQueueCreate(const UBaseType_t uxQueueLength, // 队列长度
                           const UBaseType_t uxItemSize); // 队列中每个元素的大小
```
> 函数简介

`xQueueCreate` 是一个用于创建队列的函数。队列是一种先进先出（FIFO）的数据结构，用于在任务之间传递数据。

> 参数

- `uxQueueLength`：队列的长度，即队列中可以存放的元素个数。
- `uxItemSize`：队列中每个元素的大小，以字节为单位。

> 返回值

- 队列句柄：队列创建成功。
- `NULL`：队列创建失败。

### xQueueSend

> 函数原型

```c
BaseType_t xQueueSend(QueueHandle_t xQueue, // 队列句柄
                      const void *pvItemToQueue, // 要发送到队列的数据指针，拷贝到队列中
                      TickType_t xTicksToWait); // 等待时间
```

> 函数简介

`xQueueSend` 是一个用于向队列发送数据的函数。调用 `xQueueSend` 函数后，数据将被发送到队列中。

> 参数

- `xQueue`：队列句柄。
- `pvItemToQueue`：要发送到队列的数据指针。
- `xTicksToWait`：等待时间，即在队列满时等待的时间。如果队列已满，任务将在等待时间内等待队列有空间可用。如果等待时间为 0，则任务将立即返回。

### xQueueSendToBack

> 函数原型

```c
BaseType_t xQueueSendToBack(QueueHandle_t xQueue, // 队列句柄
                            const void *pvItemToQueue, // 要发送到队列的数据指针，拷贝到队列中
                            TickType_t xTicksToWait); // 等待时间
```
> 函数简介

`xQueueSendToBack` 是一个用于向队列发送数据的函数。调用 `xQueueSendToBack` 函数后，数据将被发送到队列中, 位置为队列的尾部。 适合明确需要发送到队列尾部的情况。

> 参数

- `xQueue`：队列句柄。
- `pvItemToQueue`：要发送到队列的数据指针。
- `xTicksToWait`：等待时间，即在队列满时等待的时间。如果队列已满，任务将在等待时间内等待队列有空间可用。如果等待时间为 0，则任务将立即返回。

### xQueueReceive

> 函数原型

```c
BaseType_t xQueueReceive(QueueHandle_t xQueue, // 队列句柄
                         void *pvBuffer, // 接收数据的缓冲区指针
                         TickType_t xTicksToWait); // 等待时间
```

> 函数简介

`xQueueReceive` 是一个用于从队列接收数据的函数。调用 `xQueueReceive` 函数后，数据将从队列中接收。

> 参数

- `xQueue`：队列句柄。
- `pvBuffer`：接收数据的缓冲区指针。


### xQueueSendFromISR

> 函数原型

```c
BaseType_t xQueueSendFromISR(QueueHandle_t xQueue, // 队列句柄
                             const void *pvItemToQueue, // 要发送到队列的数据指针
                             BaseType_t *pxHigherPriorityTaskWoken); // 高优先级任务唤醒标志
```

> 函数简介

`xQueueSendFromISR` 是一个用于从中断服务程序（ISR）中向队列发送数据的函数。调用 `xQueueSendFromISR` 函数后，数据将被发送到队列中。

> 参数

- `xQueue`：队列句柄。
- `pvItemToQueue`：要发送到队列的数据指针。
- `pxHigherPriorityTaskWoken`：高优先级任务唤醒标志。如果在发送数据时唤醒了一个高优先级任务，则将此参数设置为 `pdTRUE`。

## 信号量

### xSemaphoreCreateBinary

> 函数原型

```c
SemaphoreHandle_t xSemaphoreCreateBinary(void);
```

> 函数简介

`xSemaphoreCreateBinary` 是一个用于创建二值信号量的函数。二值信号量是一种只有两种状态的信号量，即可用和不可用。

> 返回值

- 信号量句柄：信号量创建成功。
- `NULL`：信号量创建失败。

### xSemaphoreCreateCounting

> 函数原型

```c
SemaphoreHandle_t xSemaphoreCreateCounting(const UBaseType_t uxMaxCount, // 最大计数值
                                           const UBaseType_t uxInitialCount); // 初始计数值
```

> 函数简介

`xSemaphoreCreateCounting` 是一个用于创建计数信号量的函数。计数信号量是一种可以存储多个计数值的信号量。

> 参数

- `uxMaxCount`：最大计数值。
- `uxInitialCount`：初始计数值。

> 返回值

- 信号量句柄：信号量创建成功。
- `NULL`：信号量创建失败。

### xSemaphoreCreateMutex

> 函数原型

```c
SemaphoreHandle_t xSemaphoreCreateMutex(void);
```

> 函数简介

`xSemaphoreCreateMutex` 是一个用于创建互斥信号量的函数。互斥信号量是一种用于实现互斥访问的信号量，用于保护共享资源。

> 返回值

- 信号量句柄：信号量创建成功。
- `NULL`：信号量创建失败。

!!! info "互斥锁"
    互斥锁是一种用于实现互斥访问的同步机制。在 FreeRTOS 中，互斥锁是通过互斥信号量实现的。互斥锁可以确保在任何时候只有一个任务可以访问共享资源。与二值信号量不同，互斥锁实现了优先级继承和优先级反转机制，以确保任务能够按照优先级顺序访问共享资源。

!!! info "优先级翻转"
    优先级翻转是指低优先级任务暂时占用资源，导致高优先级任务被阻塞，甚至被中等优先级的任务进一步延迟。最终，高优先级任务的执行被低优先级任务间接地延后了，这种现象被称为优先级翻转。
    
    典型的优先级翻转场景如下：
    假设有三个任务，优先级从高到低依次是 任务A、任务B 和 任务C，且它们共享一个资源（例如互斥锁）。
    步骤 1：低优先级的 任务C 获得了该资源（锁）并正在使用它。
    步骤 2：在 任务C 还没释放资源时，高优先级的 任务A 开始运行，并尝试访问相同的资源。但是因为资源已经被 任务C 占用，所以 任务A 被阻塞，等待 任务C 释放资源。
    步骤 3：这时，优先级介于两者之间的 任务B 开始运行，并且由于它的优先级比 任务C 高，因此 任务B 会抢占 任务C 的执行。
    结果：由于 任务B 的执行，任务C 的进程被推迟，从而导致 任务A 也被延迟。即使 任务A 的优先级最高，它也无法立即执行，因为中优先级的 任务B 间接阻碍了它的执行。
    这种情况称为优先级翻转，因为低优先级任务 任务C 的执行阻碍了高优先级任务 任务A 的执行，而 任务B 的介入使得翻转效果更为严重。

    优先级翻转会造成实时系统的高优先级任务无法按时完成，导致系统性能下降或不稳定。在实时应用中（如控制系统或通信系统），优先级翻转可能会带来严重的后果。

    FreeRTOS 和许多其他 RTOS 使用优先级继承（Priority Inheritance）来解决优先级翻转问题。优先级继承机制的原理如下：
    当低优先级任务持有某个资源并阻塞了一个高优先级任务时，低优先级任务会继承该高优先级任务的优先级，直到资源被释放。
    在上述示例中，任务C 在阻塞了高优先级的 任务A 后，会临时提升为 任务A 的优先级。
    这样一来，任务C 能够优先于 任务B 继续运行并尽快释放资源，从而让 任务A 能够及时获取资源并执行。
    一旦 任务C 释放资源，它的优先级会恢复到原始水平。




### xSemaphoreTake

> 函数原型

```c
BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, // 信号量句柄
                          TickType_t xTicksToWait); // 等待时间
```

> 函数简介

`xSemaphoreTake` 是一个用于获取信号量的函数。调用 `xSemaphoreTake` 函数后，任务将尝试获取信号量。

> 参数

- `xSemaphore`：信号量句柄。
- `xTicksToWait`：等待时间，即在信号量不可用时等待的时间。如果信号量不可用，任务将在等待时间内等待信号量可用。如果等待时间为 0，则任务将立即返回。

> 返回值

- `pdPASS`：获取信号量成功。
- `pdFAIL`：获取信号量失败。

### xSemaphoreGive

> 函数原型

```c
BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore); // 信号量句柄
```

> 函数简介

`xSemaphoreGive` 是一个用于释放信号量的函数。调用 `xSemaphoreGive` 函数后，信号量将被释放。

> 参数

- `xSemaphore`：信号量句柄。

> 返回值

- `pdPASS`：释放信号量成功。
- `pdFAIL`：释放信号量失败。

### xSemaphoreDelete

> 函数原型

```c
void vSemaphoreDelete(SemaphoreHandle_t xSemaphore); // 信号量句柄
```

> 函数简介

`vSemaphoreDelete` 是一个用于删除信号量的函数。调用 `vSemaphoreDelete` 函数后，信号量将被删除。

> 参数

- `xSemaphore`：信号量句柄。

