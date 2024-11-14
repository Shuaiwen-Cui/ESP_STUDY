# 常用模块

## 标准库模块

### stdio

!!! info "简介"
    标准输入输出库是C语言的标准库，提供了一系列的输入输出函数，如printf、scanf等。标准输入输出库的头文件是stdio.h，引入该头文件后，就可以使用标准输入输出库的函数。


用法：

```c
#include <stdio.h>
```

### string

!!! info "简介"
    字符串库是C语言的标准库，提供了一系列的字符串处理函数，如strcpy、strcat等。字符串库的头文件是string.h，引入该头文件后，就可以使用字符串库的函数。

用法：

```c
#include <string.h>
```

## 日志模块

### esp_log

!!! info "简介"
    ESP_LOG是ESP-IDF的日志模块，提供了一些日志输出函数，如ESP_LOGI、ESP_LOGE等。esp_log.h 代表了日志模块的头文件，引入该头文件后，就可以使用日志模块的函数。

用法：

```c
#include "esp_log.h"
```

## 实时操作系统-FREERTOS模块

### freertos/FreeRTOS

!!! info "简介"
    FreeRTOS是一个小型的实时操作系统内核，用于嵌入式系统。FreeRTOS是一个开源项目，提供了一些基本的实时操作系统功能，如任务管理、时间管理、内存管理等。FreeRTOS.h 代表了其核心库的头文件，引入该头文件后，就可以使用FreeRTOS的功能。

用法：

```c
#include "freertos/FreeRTOS.h"  
```

### freertos/task

!!! info "简介"
    FreeRTOS任务管理库是FreeRTOS的一个模块，提供了一些任务管理函数，如任务创建、任务删除、任务挂起、任务恢复等。task.h 代表了任务管理库的头文件，引入该头文件后，就可以使用任务管理库的函数。

用法：

```c
#include "freertos/task.h"   
```

### freertos/queue

!!! info "简介"
    FreeRTOS队列管理库是FreeRTOS的一个模块，提供了一些队列管理函数，如队列创建、队列删除、队列发送、队列接收等。queue.h 代表了队列管理库的头文件，引入该头文件后，就可以使用队列管理库的函数。

用法：

```c
#include "freertos/queue.h"   
```