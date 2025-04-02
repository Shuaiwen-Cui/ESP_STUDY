# 常见命令

>创建工程

```bash
idf.py create-project <project_name>
```

>创建组件

```bash
idf.py -C <component_dir>   create-component <component_name>
```

>选择芯片型号

```bash
idf.py set-target <target>
```

>编译工程

```bash
idf.py build
```

>清除编译结果

```bash
idf.py fullclean
```

>向串口烧录固件

```bash
idf.py -p <port> flash
```
>打开串口监视器

```bash
idf.py -p <port> monitor
```
>配置菜单

```bash
idf.py menuconfig
```

>编译+烧录+监视器

```bash
idf.py build flash monitor
```