#ifndef TICKET_SYSTEM_USER_H
#define TICKET_SYSTEM_USER_H
#include <cstring>
#include <iostream>
#include "../b+tree/bpt.hpp"
#include "../STLite/map.hpp"
using std::cout;
using std::string;
using sjtu::map;
using sjtu::my_string;
using sjtu::BPlusTree;

class User{
private:
    my_string username, password, name, mailAddr;
    
public:
    int privilege;
    User():username(""), password(""), name(""), mailAddr(""), privilege(0) {}
    explicit User(const string &un_, const string &pw_, const string &n_, const string &m_, const string &p_):
        username(un_), password(pw_), name(n_), mailAddr(m_), privilege(stoi(p_)) {}
    explicit User(const User &other): 
        username(other.username), password(other.password), name(other.name), mailAddr(other.mailAddr), privilege(other.privilege) {}
    User &operator = (const User &other) {
        if (&other == this) return *this;
        username = other.username;
        password = other.password;
        name = other.name;
        mailAddr = other.mailAddr;
        privilege = other.privilege;
        return *this;
    }
    bool operator < (const User &other) const { return username < other.username; }
    bool check(const my_string &pw) { return password == pw; }
};

void add_user (map<char, string> &m);
void login (map<char, string> &m);
void logout (map<char, string> &m);
void query_profile (map<char, string> &m);
void modify_profile (map<char, string> &m);

#endif