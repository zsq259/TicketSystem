#include "user.h"

BPlusTree<my_string, User> userdb("user.db", "user_bin.db");
sjtu::map<my_string, bool> user_login;
vector<User> array;

bool isLogin(const my_string &id) { return user_login.find(id) != user_login.end() && user_login[id]; }

int add_user (string (&m)[256]) {
    if (userdb.empty()) {
        userdb.Insert(m['u'], User(m['u'], m['p'], m['n'], m['m'], "10"));
        return 0;
    }
    my_string curid(m['c']);
    if (!isLogin(curid) || !user_login[curid]) { return -1; }
    userdb.Find(curid, array);
    if (array.empty() || array[0].privilege <= stoi(m['g'])) { return -1; }
    userdb.Find(m['u'], array);
    if (array.size()) { return -1; }
    userdb.Insert(m['u'], User(m['u'], m['p'], m['n'], m['m'], m['g']));
    return 0;
}
int login (string (&m)[256]) {
    my_string id(m['u']);
    userdb.Find(id, array);
    if (array.empty()) { return -1; }
    if (isLogin(id) && user_login[id]) { return -1; }
    if (!array[0].check(m['p'])) { return -1; }
    user_login[id] = true;
    return 0;
}
int logout (string (&m)[256]) {
    my_string id(m['u']);
    if (!isLogin(id) || !user_login[id]) { return -1; }
    user_login[id] = false;
    return 0;
}
int query_profile (string (&m)[256]) {
    my_string curid(m['c']), id(m['u']);
    if (!isLogin(curid) || !user_login[curid]) { return -1; }
    userdb.Find(id, array);
    if (array.empty()) { return -1; }
    User now(array[0]);
    userdb.Find(curid, array);
    if (array.empty()) { return -1; }
    if (array[0].privilege < now.privilege || (array[0].privilege == now.privilege && array[0] != now)) { return -1; }
    now.print();
    return 0;
}
int modify_profile (string (&m)[256]) {
    my_string curid(m['c']), id(m['u']);
    if (!isLogin(curid) || !user_login[curid]) { return -1; }
    userdb.Find(id, array);
    if (array.empty()) { return -1; }
    User now(array[0]);
    userdb.Find(curid, array);
    if (array.empty()) { return -1; }
    if (array[0].privilege < now.privilege || (array[0].privilege == now.privilege && array[0] != now)) { return -1; }
    if (!m['g'].empty() && array[0].privilege <= stod(m['g'])) { return -1; }
    if (!userdb.Delete(id, now)) puts("sbhastinmydd");
    // o(m[]):string -> my_string
    if (!m['g'].empty()) now.privilege = stod(m['g']);
    if (!m['p'].empty()) now.changeP(m['p']);
    if (!m['n'].empty()) now.changeN(m['n']);
    if (!m['m'].empty()) now.changeM(m['m']);
    userdb.Insert(id, now);
    now.print();
    return 0;
}