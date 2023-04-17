#include <iostream>
#include <cstdio>
#include <cstring>
#include "bpt.hpp"
#include "vector.hpp"

class Key{
public:
    char key[65];
    Key() {strcpy(key, ""); }
    Key(const char key_[]) { strcpy(key, key_); }
    Key(const Key &other) { strcpy(key, other.key); }
    Key operator = (const Key &other) { 
        if (&other == this) return *this; 
        strcpy(key, other.key); 
        return *this;    
    }
    bool operator < (const Key &other) const { return strcmp(key, other.key) < 0; }
    bool operator == (const Key &other) const { return strcmp(key, other.key) == 0; }
    bool operator > (const Key &other) const { return strcmp(key, other.key) > 0; }
    bool operator != (const Key &other) const { return strcmp(key, other.key); }
    bool operator <= (const Key &other) const { return strcmp(key, other.key) <= 0; }
    bool operator >= (const Key &other) const { return strcmp(key, other.key) >= 0; }
    
};

std::ostream &operator <<(std::ostream &os, const Key &a) {
    os << a.key;
    return os;
}

char ch[65];

int main() {
    int n, x, y;
    sjtu::BPlusTree<Key, int> BPT("data.db");
    sjtu::vector<int> v;
    std::string s;
    std::cin >> n;
    for (int i = 1; i <= n; ++i) {
        //std::cerr << "i=" << i << '\n';
        std::cin >> s;
        if (s == "insert") {
            std::cin >> ch >> y;
            BPT.Insert(Key(ch), y);
        }
        else if (s == "delete") {
            std::cin >> ch >> y;
            BPT.Delete(Key(ch), y);
        }
        else if (s == "find") {
            std::cin >> ch;
            BPT.Find(Key(ch), v);
            if (v.empty()) std::cout << "null";
            else for (auto x:v) std::cout << x << ' ';
            std::cout << '\n';
        }
    }
}