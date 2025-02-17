# COMMON COMMAND

## TOC

| Use | Command |
| --- | --- |
| Create Project | `idf.py create_project <project_name>` |
| Set Target | `idf.py set_target <target>` |
| Configure | `idf.py menuconfig` |
| Create Component | `idf.py create_component <component_name>` |
| Add Component from ESP Component Register | `idf.py add-dependency <component_name>` |
| Build | `idf.py build` |
| Flash | `idf.py -p <port> flash` |
| Monitor | `idf.py -p <port> monitor` |
| Build & Flash & Monitor | `idf.py build flash monitor` |

## CREATE PROJECT

> Command Prototype

```bash
idf.py create_project <project_name>
```

> Example

```bash
idf.py create_project my_project
```

## SET TARGET

> Command Prototype

```bash
idf.py set_target <target>
```

> Example

```bash
idf.py set_target esp32s3
```

## CONFIGURE

> Command Prototype

```bash
idf.py menuconfig
```

> Example

```bash
idf.py menuconfig
```

## CREATE COMPONENT

> Command Prototype

```bash
idf.py -C <project_path> create_component <component_name>
```

!!! note

    The `-C` option is used to specify the path of the project.

    `<project_path>` is the path of the project.
    
    `<component_name>` is the name of the component.

> Example

```bash
idf.py -C /path/to/project create_component my_component
```

## ADD COMPONENT FROM ESP COMPONENT REGISTER

> Command Prototype

```bash
idf.py add-dependency <component_name>
```

> Example

```bash
idf.py add-dependency "joltwallet/littlefs^1.16.4"
```

## BUILD

> Command Prototype

```bash
idf.py build
```

> Example

```bash
idf.py build
```

## FLASH

> Command Prototype

```bash
idf.py -p <port> flash
```

> Example

```bash
idf.py -p /dev/ttyACM0 flash
```

!!! tip
    For short, most of time, you can use `idf.py flash` and the computer will automatically detect the port.

## MONITOR

> Command Prototype

```bash
idf.py -p <port> monitor
```

> Example

```bash
idf.py -p /dev/ttyACM0 monitor
```

!!! tip
    For short, most of time, you can use `idf.py monitor` and the computer will automatically detect the port.

## BUILD & FLASH & MONITOR

This is a combination of the above commands.

> Command Prototype

```bash
idf.py build flash monitor
```

> Example

```bash
idf.py build flash monitor
```