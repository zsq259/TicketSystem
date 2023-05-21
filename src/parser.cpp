#include "parser.h"

int clean (string (&m)[256]) {
    cleanUser();
    cleanTrain();
    cleanTicket();
    return 0;
}

const int N = 100005;
int ch[N][27], tot = 0, ed[N];
string m[256];
string s[24];
string a[16] = { "add_user", "login", "logout", "query_profile", "modify_profile", 
                 "add_train", "delete_train", "release_train", "query_train", 
                 "query_ticket", "query_transfer", "buy_ticket", "query_order", 
                 "refund_ticket", "clean", "exit" };
int (*func[15])(string (&m)[256]) = { add_user, login, logout, query_profile, modify_profile, 
                                           add_train, delete_train, release_train, query_train, 
                                           query_ticket, query_transfer, buy_ticket, query_order, 
                                           refund_ticket, clean };
int out[15] = {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1};

inline int num(char c) { return c == '_'? 26: c - 'a'; }

void init() {
    int len = 0, p = 0, op = 0;
    for (int o = 0; o < 16; ++o) {
        len = a[o].size(), p = 0;
        for (int i = 0; i < len; ++i) {
            op = num(a[o][i]);
            if (!ch[p][op]) ch[p][op] = ++tot;
            p = ch[p][op];
        }
        ed[p] = o;
    }
}

void cut(const char (&str)[4005]) {
    int n = 0, p = 0, len = strlen(str);
    s[n].clear();
    while (p < len && str[p] == ' ') ++p;
    for (; p < len; ++p) {
        if (str[p] != ' ') s[n] += str[p];
        else if (p + 1 < len && str[p + 1] != ' ') ++n, s[n].clear();
    }    
    for (int i = 'a'; i <= 'z'; ++i) m[i].clear();
    for (int i = 2; i <= n; i += 2) {
        m[s[i][1]] = s[i + 1];
    }
}

bool find() {
    int p = 0, len = s[1].size();
    for (int i = 0; i < len; ++i) p = ch[p][num(s[1][i])];
    cout << s[0] << ' ';
    if (ed[p] < 15) {
        m[0] = s[0].substr(1, s[0].size() - 2);
        //if (m[0] == "1097") while(1);
    
        int op = func[ed[p]](m);
        if (op != 0 || out[ed[p]]) cout << op << '\n';
        return true; 
    }
    else { cout << "bye\n"; return false; }
}