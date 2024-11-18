# COMMON MODULES

## STANDARD LIBRARY MODULES

### stdio

!!! info "Introduction"
    The standard input and output library is a standard library of the C language, which provides a series of input and output functions, such as printf, scanf, etc. The header file of the standard input and output library is stdio.h. After introducing this header file, you can use the functions of the standard input and output library.

Usage:

```c
#include <stdio.h>
```

### string

!!! info "Introduction"
    The string library is a standard library of the C language, which provides a series of string processing functions, such as strcpy, strcat, etc. The header file of the string library is string.h. After introducing this header file, you can use the functions of the string library. 

Usage:

```c
#include <string.h>
```

## LOG MODULES

### esp_log

!!! info "Introduction"
    ESP_LOG is the log module of ESP-IDF, which provides some log output functions, such as ESP_LOGI, ESP_LOGE, etc. esp_log.h represents the header file of the log module. After introducing this header file, you can use the functions of the log module.

Usage:

```c
#include "esp_log.h"
```

## TIMER MODULES

### esp_timer

!!! info "Introduction"
    ESP_TIMER is the timer module of ESP-IDF, which provides some timer functions, such as esp_timer_create, esp_timer_start_once, etc. esp_timer.h represents the header file of the timer module. After introducing this header file, you can use the functions of the timer module.

Usage:

```c
#include "esp_timer.h"
```

## RTOS-FREERTOS MODULES

### freertos/FreeRTOS

!!! info "Introduction"
    FreeRTOS is a small real-time operating system kernel for embedded systems. FreeRTOS is an open-source project that provides some basic real-time operating system functions, such as task management, time management, memory management, etc. FreeRTOS.h represents the header file of its core library. After introducing this header file, you can use the functions of FreeRTOS.

Usage:

```c
#include "freertos/FreeRTOS.h"  
```

### freertos/FreeRTOS

!!! info "Introduction"
    The FreeRTOS task management library is a module of FreeRTOS, which provides some task management functions, such as task creation, task deletion, task suspension, task resumption, etc. task.h represents the header file of the task management library. After introducing this header file, you can use the functions of the task management library.

Usage:

```c
#include "freertos/task.h"   
```

### freertos/queue

!!! info "Introduction"
    The FreeRTOS queue management library is a module of FreeRTOS, which provides some queue management functions, such as queue creation, queue deletion, queue sending, queue receiving, etc. queue.h represents the header file of the queue management library. After introducing this header file, you can use the functions of the queue management library.

Usage:

```c
#include "freertos/queue.h"   
```

### freertos/event_groups

!!! info "Introduction"
    The FreeRTOS event group management library is a module of FreeRTOS, which provides some event group management functions, such as event group creation, event group deletion, event group setting, event group clearing, etc. event_groups.h represents the header file of the event group management library. After introducing this header file, you can use the functions of the event group management library.

Usage:

```c
#include "freertos/event_groups.h"   
```
