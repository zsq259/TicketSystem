#ifndef TICKET_SYSTEM_PARSER_H
#define TICKET_SYSTEM_PARSER_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "../STLite/map.hpp"
#include "user.h"
#include "train.h"
#include "ticket.h"
using std::string;

void init();
void cut(const string &str);
bool find();

#endif