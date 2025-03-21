# FREERTOS

## 简介

!!! info
    FreeRTOS 是一个小型的实时操作系统内核，它支持多任务、定时器、信号量、队列、互斥锁等功能。FreeRTOS 是一个开源项目，现在由Amazon Web Services维护。

!!! warning
    原生FREERTOS和ESP-IDF中FREERTOS是不同的，ESP-IDF中的FREERTOS是基于原生FREERTOS的二次开发。

    1. 优先级问题，多核情况并不适用，因为多个任务可同时运行。
    2. esp-idf自动创建空闲（0）、定时器（1）、app_main（1）、IPC-多核协调（24）、ESP定时器-ESP定时器回调（22）。括号内为优先级。
    3. esp-idf不使用原生FreeRTOS的内存堆管理实现了自己的堆。
    4. 创建任务使用xTaskCreatePinnedToCore()。
    5. 删除任务避免删除另外一个核的任务。
    6. 临界区使用自旋锁确保同步。
    7. 如果任务中用到浮点运算，则创建任务的时候必须指定具体运行在哪个核上，不能由系统自动安排。

    总的来说，建议如下:

    1) 程序应用开发创建任务指定内核，建议不要使用tskNO AFFINITY。

    2) 通常，负责处理无线网络的任务(例如，WiFi或蓝牙)将被固定到CPUO(因此名称PRO_CPU)，而处理应用程序其余部分的任务将被固定到CPU1(因此名称APP CPU)


## 核心概念

> without RTOS

```c
while(1)
{
    task1();
    task2();
    task3();
}
```

> with RTOS

```c
void task1(void *pvParameters)
{
    for(;;)
    {
        // task1 code
    }
}

void task2(void *pvParameters)
{
    for(;;)
    {
        // task2 code
    }
}

void task3(void *pvParameters)
{
    for(;;)
    {
        // task3 code
    }
}

int main(void)
{
    xTaskCreate(task1, "Task1", 100, NULL, 1, NULL);
    xTaskCreate(task2, "Task2", 100, NULL, 1, NULL);
    xTaskCreate(task3, "Task3", 100, NULL, 1, NULL);
    vTaskStartScheduler();
    for(;;);
}
```
!!! note
    从上面的代码可以看出，在没有RTOS的情况下，面对功能庞杂的项目，代码会变得非常复杂，而使用RTOS，我们可以将任务分解为多个子任务，使代码更加清晰、易于维护。

!!! note
    任务运行时间片的基本单位是Tick，Tick的周期由configTICK_RATE_HZ宏定义决定，单位是Hz。

## 任务

!!! info
    使用FREERTOS的实时应用程序可以被构建成一组独立的任务，每个任务在自己的上下文中运行，不依赖于系统内的其他任务或RTOS任务调度器本身。

!!! note
    任务状态有以下几种：

    - 运行态（Running）

    - 就绪态（Ready）

    - 阻塞态（Blocked）
  
    - 挂起态（Suspended）

> 运行态（Running）

当任务实际执行的时候，它被称为处于运行状态。如果运行RTOS的处理器只有一个内核，那么只有一个任务可以处于运行状态。

> 就绪态（Ready）

当任务已经准备好运行，但是由于其他任务正在运行，所以它还没有得到执行的机会，这种状态被称为就绪状态。

> 阻塞态（Blocked）

当任务由于某种原因而不能运行时，它被称为阻塞状态。任务可能会因为等待事件、等待信号量、等待消息队列等原因而被阻塞。通常一个处于阻塞状态的任务都有一个超时时间，当超时时间到达时，任务会被唤醒。

> 挂起态（Suspended）

与阻塞状态的任务一样，挂起状态的任务不能进入运行状态。但是，与阻塞状态的任务不同的是，挂起状态的任务不会被唤醒，直到它被显式地恢复。只有通过API命令明确挂起和恢复，任务才会进入或者退出挂起状态。


## FreeRTOS 概念对比

以下是 FreeRTOS 中 **队列**、**信号量**、**互斥量**、**事件组** 和 **任务通知** 的简明对比：

| 特性            | 队列               | 信号量           | 互斥量           | 事件组           | 任务通知         |
|-----------------|--------------------|------------------|------------------|------------------|------------------|
| **主要用途**    | 数据传输           | 同步/资源管理    | 共享资源保护     | 多任务同步       | 轻量级同步       |
| **数据传递**    | 支持               | 不支持           | 不支持           | 不支持           | 支持（通知值）   |
| **资源占用**    | 较高               | 中等             | 中等             | 中等             | 最低             |
| **优先级继承**  | 不支持             | 不支持           | 支持             | 不支持           | 不支持           |
| **适用场景**    | 任务间消息传递     | 任务同步         | 共享资源保护     | 复杂事件触发     | 轻量级同步       |

### 关键区别
- **队列**：适合传递数据。
- **信号量**：适合简单同步或资源计数。
- **互斥量**：适合保护共享资源。
- **事件组**：适合多任务同步和复杂事件触发。
- **任务通知**：适合轻量级同步，效率最高。