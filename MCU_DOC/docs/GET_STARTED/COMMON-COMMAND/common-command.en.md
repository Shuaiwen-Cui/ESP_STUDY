# COMMON COMMANDS

>Create a new project

```bash
idf.py create-project <project_name>
```

>Create a new component

```bash
idf.py -C <component_dir>   create-component <component_name>
```

>Select the target chip

```bash
idf.py set-target <target>
```

>Build the project

```bash
idf.py build
```

>Clean the build results

```bash
idf.py fullclean
```

>Flash the firmware to the serial port

```bash
idf.py -p <port> flash
```

>Open the serial monitor

```bash
idf.py -p <port> monitor
```

>Configure the menu

```bash
idf.py menuconfig
```

>Build + Flash + Monitor

```bash
idf.py build flash monitor
```