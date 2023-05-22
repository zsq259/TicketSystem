#include <iostream>
#include <cstdio>
#include <cstring>
#include "../STLite/map.hpp"
#include "parser.h"
#include "user.h"
#include "train.h"
#include "ticket.h"
using std::string;
using std::cin;

char str[4005];
//extern vector<int> rbq;

//extern int cnt1, cnt2;

int main() {
    init();
    while(1) {
        if (!cin.getline(str, 4000)) break;
        cut(str);
        if (!find()) break;
    }
    //std::cerr << ' ' << rbq.size() << '\n';
    return 0;
}