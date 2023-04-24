# TicketSystem

- 火车票管理系统

- by zsq aka hst

## 程序功能概述

实现了一个火车票管理系统。

使用命令行（Windows CMD 或 Linux Terminal）进行操作，输入数据以换行符或回车符为分隔为若干指令。详细指令说明请查阅[系统说明](https://github.com/zsq259/TicketSystem/blob/main/commands.md)。

## 主题逻辑说明

### `parser.h`

用于对输入的命令进行切片以及处理调用相关函数。

### `user.h`

用户系统库

使用 map 处理登录情况

将账户信息存储于 `user.db` 中。

### `bpt.hpp`

实现了一个B+树类，将信息存于外存

## 代码文件结构

```
├── TicketSystem
│   ├── STLite
│   │   ├── exceptions.hpp
│   │   ├── map.hpp
│   │   ├── utility.hpp
│   │   ├── vector.hpp
│   ├── STLite
│   │   ├── LinkedHashMap.hpp
│   │   ├── bpt.hpp
│   ├── src
│   │   ├── main.cpp
│   │   ├── parser.cpp
│   │   ├── parser.h
│   │   ├── ticket.cpp
│   │   ├── ticket.h
│   │   ├── train.cpp
│   │   ├── train.h
│   │   ├── user.cpp
│   └── └── user.h
├── CMakeLists.txt
└── README.md
```

## 类的接口及成员

## 文件存储说明

`user.db`：账户信息。

### 修订记录

2023/4/24：完成框架中的基本说明。