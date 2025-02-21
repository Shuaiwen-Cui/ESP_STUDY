# ESP32 STUDY
This project is for ESP32 study and practice.

![cover](cover.jpg)

## ESPRESSIF SYSTEMS & ESPRESSIF IOT DEVELOPMENT FRAMEWORK (ESP-IDF)

!!! info
    **Espressif Systems** is a semiconductor company based in China, known for developing low-power wireless solutions, including Wi-Fi and Bluetooth modules and SoCs (System on Chips). Their products, such as the ESP8266 and ESP32 series, have gained popularity in IoT, embedded systems, and wireless communication due to their low cost, power efficiency, and ease of use. Espressif provides a range of development tools and software support, making their products widely adopted by developers and engineers in various industries.

!!! info
    **ESP-IDF (Espressif IoT Development Framework)** is the official development framework for the ESP32 series chips from Espressif. It provides a comprehensive set of tools and libraries to help developers create robust applications for ESP32-based devices. ESP-IDF supports FreeRTOS, Wi-Fi, Bluetooth, and a wide range of peripherals. It is compatible with the GCC toolchain and supports multiple languages like C and C++. The framework includes components for networking, security, power management, and driver development, making it suitable for a wide range of IoT applications.

!!! note
    There are two ways to use ESP-IDF: **ESP-IDF Command Prompt** and **GUI-based IDEs** like Visual Studio Code. The latter is the most popular way to use ESP-IDF.

## DEV TOOLCHAIN

| TOOLCHAIN | DESCRIPTION | PROS | CONS |
| --- | --- | --- | --- |
| ESP_IDF✅ | Espressif IoT Development Framework | Rich in features, good documentation | Steep learning curve |
| MICROPYTHON | Python for microcontrollers | Easy to learn, rapid development | Limited resources |
| ARDUINO | Open-source electronics platform | Easy to use, large community | Limited resources |

!!! note
    For rapid prototyping, we use MICROPYTHON; for high-performance applications, we use ESP_IDF.

## SETUP

!!! note
    In terms of interfacing manner, there are three ways to use ESP-IDF:

    - ESP-IDF Command Prompt: This is the default way to use ESP-IDF. It is a command-line interface that provides access to the tools and utilities required for ESP-IDF development.

    - ESP-IDF IDE: This is an integrated development environment that provides a graphical interface for ESP-IDF development. It includes tools like the ESP-IDF Command Prompt, a text editor, and a project manager.

    - IDE Plugins: These are plugins that integrate ESP-IDF with popular IDEs like Visual Studio Code and Eclipse. They provide features like code completion, syntax highlighting, and debugging support. This is the most popular way to use ESP-IDF.

<div class="grid cards" markdown>

-   :fontawesome-brands-youtube:{ .lg .middle } __How to Install the ESP-IDF Toolchain on Windows 🎯🏆✅__

    ---

    [:octicons-arrow-right-24: <a href="https://www.youtube.com/watch?v=byVPAfodTyY&list=PLOzvoM7_Knrc9MKih2gNIMwZhGBfIKQb7" target="_blank"> Portal </a>](#)

-   :fontawesome-brands-youtube:{ .lg .middle } __How to Install the ESP-IDF Toolchain on Ubuntu 🎯🏆✅__

    ---

    [:octicons-arrow-right-24: <a href="https://www.youtube.com/watch?v=Hj3okDU-CIY&list=PLOzvoM7_Knrc9MKih2gNIMwZhGBfIKQb7&index=4" target="_blank"> Portal </a>](#)

</div>

## OFFICIAL GUIDE

### Online Documentation

<div class="grid cards" markdown>

-   :simple-espressif:{ .lg .middle } __ESP-IDF Programming Guide 🎯🏆__

    ---

    [:octicons-arrow-right-24: <a href="https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html" target="_blank"> Portal </a>](#)

</div>

### Video Tutorials

<div class="grid cards" markdown>

-   :fontawesome-brands-bilibili:{ .lg .middle } __【Espressif Tutorial】| Introduction to ESP-IDF IoT Development Framework 🎯🏆✅__

    ---

    [:octicons-arrow-right-24: <a href="https://www.bilibili.com/video/BV1tY4y1L7HV/?spm_id_from=333.999.0.0&vd_source=5a427660f0337fedc22d4803661d493f" target="_blank"> Link </a>](#)

-   :fontawesome-brands-bilibili:{ .lg .middle } __【Espressif Tutorial】| Espressif AIoT Development Resources and Application Guide 🎯🏆✅__

    ---

    [:octicons-arrow-right-24: <a href="https://www.bilibili.com/video/BV1uY411N7ns/?spm_id_from=333.999.0.0&vd_source=5a427660f0337fedc22d4803661d493f" target="_blank"> Link </a>](#)

-   :fontawesome-brands-bilibili:{ .lg .middle } __【Espressif Tutorial】| AIoT Programming with ESP32-S3 🎯🏆✅__

    ---

    [:octicons-arrow-right-24: <a href="https://www.bilibili.com/video/BV1Jr4y1E79s/?spm_id_from=333.999.0.0&vd_source=5a427660f0337fedc22d4803661d493f" target="_blank"> Link </a>](#)

</div>

<iframe src="http://www.cuishuaiwen.com:7500/DEV/ESP32/OFFICIAL/briefing.pdf" width="100%" height="600px"></iframe> 

## Third-Party Tutorials

<div class="grid cards" markdown>

-   :fontawesome-brands-bilibili:{ .lg .middle } __【2024 Latest ESP32 Tutorial (Based on ESP-IDF)】Entry-Level ESP32 Development Course - Updating with Subtitles 🎯🏆__

    ---

    [:octicons-arrow-right-24: <a href="https://www.bilibili.com/video/BV1eRg7exEcT/?spm_id_from=333.788.recommend_more_video.0&vd_source=5a427660f0337fedc22d4803661d493f" target="_blank"> Link </a>](#)

-   :fontawesome-brands-bilibili:{ .lg .middle } __Binary Lone Wolf ESP-IDF Tutorial 🎯🏆__

    ---

    [:octicons-arrow-right-24: <a href="https://www.bilibili.com/video/BV1hM411k7zz/?spm_id_from=333.999.0.0" target="_blank"> Link </a>](#)

-   :material-file:{ .lg .middle } __DURUOFU ESP32-GUIDE 🎯🏆__

    ---

    [:octicons-arrow-right-24: <a href="https://github.com/DuRuofu/ESP32-Guide" target="_blank"> New Version </a>](#)

    [:octicons-arrow-right-24: <a href="https://www.duruofu.top/2024/01/30/4.%E7%A1%AC%E4%BB%B6%E7%9B%B8%E5%85%B3/MCU/ESP32/00.%E7%9B%AE%E5%BD%95/ESP32%E5%AD%A6%E4%B9%A0%E8%AE%A1%E5%88%92/" target="_blank"> Old Version </a>](#)

-   :fontawesome-brands-youtube:{ .lg .middle} __LEARNEPS32 🎯🏆__

    ---

    [:octicons-arrow-right-24: <a href="https://learnesp32.com/videos/course-introduction/course-introduction" target="_blank"> Link </a>](#)

</div>

## FreeRTOS

Since ESP-IDF is based on FreeRTOS, learning FreeRTOS is essential for understanding ESP-IDF.

<div class="grid cards" markdown>

-   :fontawesome-brands-bilibili:{ .lg .middle } __ESP32 FreeRTOS Tutorial 🎯🏆__

    ---

    [:octicons-arrow-right-24: <a href="https://www.bilibili.com/video/BV1Nb4y1q7xz/?spm_id_from=333.337.search-card.all.click&vd_source=5a427660f0337fedc22d4803661d493f" target="_blank"> Link </a>](#)

-   :fontawesome-brands-bilibili:{ .lg .middle } __Binary Lone Wolf FreeRTOS Tutorial__

    ---

    Based on Arduino IDE

    [:octicons-arrow-right-24: <a href="https://www.bilibili.com/video/BV1q54y1Z7ca/?spm_id_from=333.999.0.0&vd_source=5a427660f0337fedc22d4803661d493f" target="_blank"> Link </a>](#)

</div>
