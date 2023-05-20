#include <iostream>
#include <cstdio>
#include <cstring>
#include "../b+tree/bpt.hpp"
#include "../STLite/map.hpp"
#include "parser.h"
#include "user.h"
#include "train.h"
#include "ticket.h"
using std::string;
using std::cin;

char str[4005];

int main() {
    init();
    while(1) {
        //str.clear();
        //puts("ok");
        if (!cin.getline(str, 4000)) break;
        //puts("ok1");
        //std::cout << str << '\n';
        cut(str);
        if (!find()) break;
    }
    return 0;
}