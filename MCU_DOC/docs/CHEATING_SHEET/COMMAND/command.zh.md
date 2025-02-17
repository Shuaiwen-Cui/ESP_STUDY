# 常用命令

## 目录

| 用途 | 命令 |
| --- | --- |
| 创建项目 | `idf.py create_project <project_name>` |
| 设置目标 | `idf.py set_target <target>` |
| 配置 | `idf.py menuconfig` |
| 创建组件 | `idf.py -C <project_path> create_component <component_name>` |
| 添加注册表组件 | `idf.py add-dependency <component_name>` |
| 编译 | `idf.py build` |
| 烧写 | `idf.py -p <port> flash` |
| 监视 | `idf.py -p <port> monitor` |
| Build & Flash & Monitor | `idf.py build flash monitor` |

## 创建项目

> 命令原型

```bash
idf.py create_project <project_name>
```

> 示例

```bash
idf.py create_project my_project
```

## 设置目标

> 命令原型

```bash
idf.py set_target <target>
```

> 示例

```bash
idf.py set_target esp32s3
```

## 配置

> 命令原型

```bash
idf.py menuconfig
```

> 示例

```bash
idf.py menuconfig
```

!!! note "ESP32 配置体系"
    ESP32项目中，通常我们会通过 `menuconfig` 命令来配置项目。`menuconfig` 命令会打开一个配置界面，你可以在这个界面中配置项目的各种选项，而这个配置界面是通过 `Kconfig` 文件生成的。我们在项目中的 `sdkconfig` 文件中保存了这些配置选项的值。
    
    简单来说，配置体系的逻辑关系是：`Kconfig` -> `menuconfig` -> `sdkconfig`, 我们通过`Kconfig`文件定义配置选项，通过`menuconfig`命令配置这些选项，然后通过`sdkconfig`文件保存这些配置选项的值。

## 创建组件

> 命令原型

```bash
idf.py -C <project_path> create_component <component_name>
```

!!! note

    `-C` 选项用于指定项目的路径。

    `<project_path>` 是项目的路径。

    `<component_name>` 是组件的名称。

> 示例

```bash
idf.py -C /path/to/project create_component my_component
```

## 添加注册表组件

> 命令原型

```bash
idf.py add-dependency <component_name>
```

> 示例

```bash
idf.py add-dependency "joltwallet/littlefs^1.16.4"
```

## 编译

> 命令原型

```bash
idf.py build
```

> 示例

```bash
idf.py build
```

## 烧写

> 命令原型

```bash
idf.py -p <port> flash
```

> 示例

```bash
idf.py -p /dev/ttyUSB0 flash
```
!!! tip
    简短起见，大部分情况下，你都可以使用 `idf.py flash` 命令，电脑会自动检测端口并烧写。

## 监视

> 命令原型

```bash
idf.py -p <port> monitor
```

> 示例

```bash
idf.py -p /dev/ttyUSB0 monitor
```

!!! tip
    简短起见，大部分情况下，你都可以使用 `idf.py monitor` 命令，电脑会自动检测端口并监视。

## 编译 & 烧写 & 监视

> 命令原型

```bash
idf.py build flash monitor
```

> 示例

```bash
idf.py build flash monitor
```

