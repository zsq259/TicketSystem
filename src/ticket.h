#ifndef TICKET_SYSTEM_TICKET_H
#define TICKET_SYSTEM_TICKET_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "../STLite/map.hpp"
#include "user.h"
#include "train.h"

class Order{
    public:
    my_string statue, trainid, userid, from, to;
    int id, date, sDate, sTime, tDate, tTime, price, num;
    Order() = default;
    Order(const string &s_, const string &tid_, const string &uid_, 
          const string &from_, const string &to_, 
          int id_, int d_, int sD_, int sT_, int tD_, int tT_, int p_, int n_): 
          statue(s_), trainid(tid_), userid(uid_), from(from_), to(to_), id(id_), 
          date(d_), sDate(sD_), sTime(sT_), tDate(tD_), tTime(tT_), price(p_), num(n_) {}
    void print() {
        cout << statue << ' ' << trainid << ' ';
        cout << from << ' ' << Date(sDate).s << ' ' << Time(sTime).s << " -> ";
        cout << to << ' ' << Date(tDate).s << ' ' << Time(tTime).s << ' ';
        cout << price << ' ' << num << '\n';
    }
    bool operator <  (const Order &other) const { return id <  other.id; }
    bool operator >  (const Order &other) const { return id >  other.id; }
    bool operator <= (const Order &other) const { return id <= other.id; }
    bool operator >= (const Order &other) const { return id >= other.id; }
    bool operator == (const Order &other) const { return id == other.id; }
    bool operator != (const Order &other) const { return id != other.id; }
}; 

void stationAdd(const my_string &a, const Train &b);

int query_ticket (string (&m)[256]);
int query_transfer (string (&m)[256]);
int buy_ticket (string (&m)[256]);
int query_order (string (&m)[256]);
int refund_ticket (string (&m)[256]);

#endif