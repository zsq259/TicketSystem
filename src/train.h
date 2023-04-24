#ifndef TICKET_SYSTEM_TRAIN_H
#define TICKET_SYSTEM_TRAIN_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "../STLite/map.hpp"
#include "user.h"
void add_train (map<char, string> &m);
void delete_train (map<char, string> &m);
void release_train (map<char, string> &m);
void query_train (map<char, string> &m);

#endif