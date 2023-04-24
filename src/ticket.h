#ifndef TICKET_SYSTEM_TICKET_H
#define TICKET_SYSTEM_TICKET_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "../STLite/map.hpp"
#include "user.h"

void query_ticket (map<char, string> &m);
void query_transfer (map<char, string> &m);
void buy_ticket (map<char, string> &m);
void query_order (map<char, string> &m);
void refund_ticket (map<char, string> &m);

#endif