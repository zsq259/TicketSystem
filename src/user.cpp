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
    userdb.Find(curid, array);
    if (array.empty()) { cout << "-1\n"; return ; }
    if (array[0].privilege < now.privilege || (array[0].privilege == now.privilege && array[0] != now)) { cout << "-1\n"; return ; }
    now.print();
}
void modify_profile (map<char, string> &m) {
    my_string curid(m['c']), id(m['u']);
    if (user_login.find(curid) == user_login.end() || !user_login[curid]) { cout << "-1\n"; return ; }
    userdb.Find(id, array);
    if (array.empty()) { cout << "-1\n"; return ; }
    User now(array[0]);
    userdb.Find(curid, array);
    if (array.empty()) { cout << "-1\n"; return ; }
    if (array[0].privilege < now.privilege || (array[0].privilege == now.privilege && array[0] != now)) { cout << "-1\n"; return ; }
    if (m.find('g') != m.end() && array[0].privilege <= stod(m['g'])) { cout << "-1\n"; return ; }
    if (!userdb.Delete(id, now)) puts("sbhastinmydd");
    // o(m[]):string -> my_string
    if (m.find('g') != m.end()) now.privilege = stod(m['g']);
    if (m.find('p') != m.end()) now.changeP(m['p']);
    if (m.find('n') != m.end()) now.changeN(m['n']);
    if (m.find('m') != m.end()) now.changeM(m['m']);
    userdb.Insert(id, now);
    now.print();
}