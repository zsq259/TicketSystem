#ifndef TICKET_SYSTEM_TICKET_H
#define TICKET_SYSTEM_TICKET_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "../STLite/map.hpp"
#include "user.h"

class Train;
void stationAdd(const my_string &a, const Train &b);

int query_ticket (string (&m)[256]);
int query_transfer (string (&m)[256]);
int buy_ticket (string (&m)[256]);
int query_order (string (&m)[256]);
int refund_ticket (string (&m)[256]);

#endif