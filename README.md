# TicketSystem

- ��Ʊ����ϵͳ

- by zsq aka hst

## �����ܸ���

ʵ����һ����Ʊ����ϵͳ��

ʹ�������У�Windows CMD �� Linux Terminal�����в��������������Ի��з���س���Ϊ�ָ�Ϊ����ָ���ϸָ��˵�������[ϵͳ˵��](https://github.com/zsq259/TicketSystem/blob/main/commands.md)��

## �����߼�˵��

### `parser.h`

���ڶ���������������Ƭ�Լ����������غ�����

### `user.h`

�û�ϵͳ��

ʹ�� map �����¼���

���˻���Ϣ�洢�� `user.db` �С�

### `bpt.hpp`

ʵ����һ��B+���࣬����Ϣ�������

## �����ļ��ṹ

```
������ TicketSystem
��   ������ STLite
��   ��   ������ exceptions.hpp
��   ��   ������ map.hpp
��   ��   ������ utility.hpp
��   ��   ������ vector.hpp
��   ������ b+tree
��   ��   ������ LinkedHashMap.hpp
��   ��   ������ bpt.hpp
��   ������ src
��   ��   ������ main.cpp
��   ��   ������ parser.cpp
��   ��   ������ parser.h
��   ��   ������ ticket.cpp
��   ��   ������ ticket.h
��   ��   ������ train.cpp
��   ��   ������ train.h
��   ��   ������ user.cpp
��   ������ ������ user.h
������ CMakeLists.txt
������ README.md
```

## �����߼�˵��

### `bpt.hpp`

B+����

ʵ����һ��B+����

�ӿڣ�

```
void Insert(const Key &key, const T &v);
bool Delete(const Key &key, const T &v);
void Find(const Key &key, vector<T> &array);
void Find(const Key &key, T &A);
void FindAll(vector<T> &array);
```



### `parser.h`

���ڶ���������������Ƭ�Լ����������غ�����

ʹ�� `trie` ʹ�ַ�����Ӧ����غ�����

### `user.h`

�û�ϵͳ��

�����û��˻���������⡣

��һ�� `map` ���¼�����

�ӿڣ�

```
int add_user (string (&m)[256]);
int login (string (&m)[256]);
int logout (string (&m)[256]);
int query_profile (string (&m)[256]);
int modify_profile (string (&m)[256]);
```

### `train.h`

��ϵͳ��

���ڴ���𳵵�������⡣

�ӿڣ�

```
int add_train (string (&m)[256]);
int delete_train (string (&m)[256]);
int release_train (string (&m)[256]);
int query_train (string (&m)[256]);
```

### `ticket.h`

��Ʊϵͳ��

���ڴ����ѯ����Ʊ��������⡣

�ӿڣ�

```
int query_ticket (string (&m)[256]);
int query_transfer (string (&m)[256]);
int buy_ticket (string (&m)[256]);
int query_order (string (&m)[256]);
int refund_ticket (string (&m)[256]);
```

## �ļ��洢˵��

`user.db`���˻���Ϣ��

`user_bin.db`����Ӧbpt�ռ���յ���Ϣ��

`train.db`������Ϣ���ڵ�λ�á�

`train_bin.db`����Ӧbpt�ռ���յ���Ϣ��

`trains.db`������Ϣ��

`station.db`������ĳ��վ��Ӧ�Ļ���Ϣ��

`station_bin.db`����Ӧbpt�ռ���յ���Ϣ��

`order.db`���û��Ķ�����Ϣ��

`order_bin.db`����Ӧbpt�ռ���յ���Ϣ��

`wait.db`�����б���Ϣ���ڵ�λ�á�

`wait_bin.db`����Ӧbpt�ռ���յ���Ϣ��

`waits.db`�����б�Ķ�����Ϣ��

`seat.db`���𳵵���λ��Ϣ��

### �޶���¼

2023/4/24����ɿ���еĻ���˵���� 

2023/5/22������˶����д�������˵����