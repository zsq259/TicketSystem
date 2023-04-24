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

int main() {
    init();
    string str;
    while(1) {
        if (!getline(cin, str)) break;
        cut();
        if (!find()) break;
    }
    return 0;
}