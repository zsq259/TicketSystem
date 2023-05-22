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
│   ├── b+tree
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

## 主体逻辑说明

### `bpt.hpp`

B+树库

实现了一个B+树。

接口：

```
void Insert(const Key &key, const T &v);
bool Delete(const Key &key, const T &v);
void Find(const Key &key, vector<T> &array);
void Find(const Key &key, T &A);
void FindAll(vector<T> &array);
```



### `parser.h`

用于对输入的命令进行切片以及处理调用相关函数。

使用 `trie` 使字符串对应到相关函数。

### `user.h`

用户系统库

处理用户账户的相关问题。

用一个 `map` 存登录情况。

接口：

```
int add_user (string (&m)[256]);
int login (string (&m)[256]);
int logout (string (&m)[256]);
int query_profile (string (&m)[256]);
int modify_profile (string (&m)[256]);
```

### `train.h`

火车系统库

用于处理火车的相关问题。

接口：

```
int add_train (string (&m)[256]);
int delete_train (string (&m)[256]);
int release_train (string (&m)[256]);
int query_train (string (&m)[256]);
```

### `ticket.h`

售票系统库

用于处理查询和售票的相关问题。

接口：

```
int query_ticket (string (&m)[256]);
int query_transfer (string (&m)[256]);
int buy_ticket (string (&m)[256]);
int query_order (string (&m)[256]);
int refund_ticket (string (&m)[256]);
```

## 文件存储说明

`user.db`：账户信息。

`user_bin.db`：对应bpt空间回收的信息。

`train.db`：火车信息所在的位置。

`train_bin.db`：对应bpt空间回收的信息。

`trains.db`：火车信息。

`station.db`：经过某个站对应的火车信息。

`station_bin.db`：对应bpt空间回收的信息。

`order.db`：用户的订单信息。

`order_bin.db`：对应bpt空间回收的信息。

`wait.db`：候补列表信息所在的位置。

`wait_bin.db`：对应bpt空间回收的信息。

`waits.db`：候补列表的订单信息。

`seat.db`：火车的座位信息。

### 修订记录

2023/4/24：完成框架中的基本说明。 

2023/5/22：完成了对所有代码的相关说明。