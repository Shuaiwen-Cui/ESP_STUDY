# HELLO WORLD

## 准备项目

复制模板项目到工作目录

## 替换代码
将main.c中的代码替换为如下代码：

```c
#include <stdio.h>                     // Standard input-output library for basic I/O operations
#include "freertos/FreeRTOS.h"         // FreeRTOS core library, providing real-time operating system features
#include "freertos/task.h"             // FreeRTOS task management library, includes functions for creating, deleting, and delaying tasks
#include "esp_log.h"                   // ESP32 logging library for debugging and information output

// Define a log tag to identify the module where the log is coming from
static const char* TAG = "main";

// Main function, the entry point of the ESP32 program
void app_main(void)
{
    // Infinite loop, the code inside the loop will be repeatedly executed
    while(1)
    {
        // Log an informational message with the "Hello world!" string
        // TAG identifies the log's source module, here it is "main"
        ESP_LOGI(TAG,"Hello world!");
        
        // Delay the task for 1 second before repeating the loop
        // vTaskDelay pauses the task for the specified time, where pdMS_TO_TICKS(1000) converts 1000 milliseconds (1 second) to FreeRTOS ticks
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

## 编译，烧录，监测

在串口监视器中，您将看到每秒打印一次“Hello world!”。