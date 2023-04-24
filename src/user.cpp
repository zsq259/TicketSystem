#include "user.h"

BPlusTree<my_string, User> userdb("user.db", "user_bin.db");
map<my_string, bool> user_login;
vector<User> array;

void add_user (map<char, string> &m) {
    if (userdb.empty()) {
        userdb.Insert(m['u'], User(m['u'], m['p'], m['n'], m['m'], "10"));
        cout << "0\n";
        return ;
    }
    my_string curid(m['c']);
    if (user_login.find(curid) == user_login.end() || !user_login[curid]) { cout << "-1\n"; return ; }
    userdb.Find(curid, array);
    if (array.empty() || array[0].privilege <= stoi(m['g'])) { cout << "-1\n"; return ; }
    userdb.Find(m['u'], array);
    if (array.size()) { cout << "-1\n"; return ; }
    userdb.Insert(m['u'], User(m['u'], m['p'], m['n'], m['m'], m['g']));
    cout << "0\n";
}
void login (map<char, string> &m) {
    my_string id(m['u']);
    userdb.Find(id, array);
    if (array.empty()) { cout << "-1\n"; return ; }
    if (user_login.find(id) != user_login.end() && user_login[id]) { cout << "-1\n"; return ; }
    if (!array[0].check(m['p'])) { cout << "-1\n"; return ; }
    user_login[id] = true;
    cout << "0\n";
}
void logout (map<char, string> &m) {
    my_string id(m['u']);
    if (user_login.find(id) == user_login.end() || !user_login[id]) { cout << "-1\n"; return ; }
    user_login[id] = false;
    cout << "0\n";
}
void query_profile (map<char, string> &m) {
    my_string curid(m['c']), id(m['u']);
    if (user_login.find(curid) == user_login.end() || !user_login[curid]) { cout << "-1\n"; return ; }
    userdb.Find(id, array);
    if (array.empty()) { cout << "-1\n"; return ; }
    User now(array[0]);
}
void modify_profile (map<char, string> &m) { puts("5"); }