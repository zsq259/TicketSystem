#ifndef TICKET_SYSTEM_USER_H
#define TICKET_SYSTEM_USER_H
#include <cstring>
#include <iostream>
#include "../b+tree/bpt.hpp"
#include "../STLite/map.hpp"
using std::cout;
using std::string;
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
    bool operator <  (const User &other) const { return username <  other.username; }
    bool operator >  (const User &other) const { return username >  other.username; }
    bool operator <= (const User &other) const { return username <= other.username; }
    bool operator >= (const User &other) const { return username >= other.username; }
    bool operator == (const User &other) const { return username == other.username; }
    bool operator != (const User &other) const { return username != other.username; }
    bool check(const my_string &pw) { return password == pw; }
    void changeP(const my_string &o) { password = o; }
    void changeN(const my_string &o) { name = o; } 
    void changeM(const my_string &o) { mailAddr = o; }
    void print() {
        cout << username << ' ' << name << ' ' << mailAddr << ' ' << privilege << '\n';
    }
    friend std::ostream &operator<<(std::ostream &os, const User &a) {
        return os << a.username << ' ' << a.name << ' ' << a.mailAddr << ' ' << a.privilege;
    } 
};

bool isLogin(const my_string &id);

int add_user (string (&m)[256]);
int login (string (&m)[256]);
int logout (string (&m)[256]);
int query_profile (string (&m)[256]);
int modify_profile (string (&m)[256]);

#endif