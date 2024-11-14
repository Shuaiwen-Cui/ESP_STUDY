# FREERTOS

## Introduction

!!! info
    FreeRTOS is a small real-time operating system kernel that supports multitasking, timers, semaphores, queues, mutexes, and other functions. FreeRTOS is an open-source project now maintained by Amazon Web Services.

## Core Concepts

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
    It can be seen that in the case of no RTOS, when facing a project with complex functions, the code will become very complicated, while using RTOS, we can decompose tasks into multiple sub-tasks, making the code clearer and easier to maintain.

!!! note
    The basic time unit of FreeRTOS is tick. The tick frequency is determined by the `configTICK_RATE_HZ` macro in `FreeRTOSConfig.h`.

## Task

!!! info
    Real-time applications using FREERTOS can be built as a set of independent tasks, each task running in its own context and not dependent on other tasks within the system or the RTOS task scheduler itself.

!!! note
    Tasks have the following status:

    - Running

    - Ready

    - Blocked

    - Suspended

> Running

When a task is actually executing, it is said to be in the Running state. If the processor running the RTOS has only one core, then only one task can be in the Running state.

> Ready

When a task is ready to run, but it has not yet been given a chance to run because other tasks are running, this state is called the Ready state.

> Blocked

When a task cannot run for some reason, it is said to be in the Blocked state. A task may be blocked for waiting for an event, waiting for a semaphore, waiting for a message queue, etc. Usually a blocked task has a timeout, and when the timeout is reached, the task is awakened.

> Suspended

Like a blocked task, a suspended task cannot enter the Running state. However, unlike a blocked task, a suspended task will not be awakened until it is explicitly resumed. A task will only enter or exit the Suspended state if it is explicitly suspended and resumed through an API command. > Running

When a task is actually executing, it is said to be in the Running state. If the processor running the RTOS has only one core, then only one task can be in the running state.

> Ready

When a task is ready to run, but it has not yet been given a chance to execute because other tasks are running, this state is called the ready state.

> Blocked

When a task cannot run for some reason, it is called the blocked state. Tasks may be blocked for waiting for events, waiting for semaphores, waiting for message queues, etc. Usually a task in the blocked state has a timeout, and when the timeout is reached, the task will be awakened.

> Suspended

Like a task in the blocked state, a task in the suspended state cannot enter the running state. However, unlike a task in the blocked state, a task in the suspended state will not be awakened until it is explicitly resumed. A task will only enter or exit the suspended state if it is explicitly suspended and resumed through API commands.