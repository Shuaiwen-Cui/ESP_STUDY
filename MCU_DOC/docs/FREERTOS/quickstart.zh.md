# FREERTOS

## 简介

!!! info
    FreeRTOS 是一个小型的实时操作系统内核，它支持多任务、定时器、信号量、队列、互斥锁等功能。FreeRTOS 是一个开源项目，现在由Amazon Web Services维护。

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
