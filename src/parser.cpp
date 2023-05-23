#include "terminal.h"
#include "parser.h"

extern Terminal t;

int Terminal::clean (string (&m)[256]) {
    cleanUser();
    cleanTrain();
    cleanTicket();
    return 0;
}

inline int num(char c) { return c == '_'? 26: c - 'a'; }

void Terminal::init() {
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

void Terminal::cut(const char (&str)[4005]) {
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

bool Terminal::find() {
    int p = 0, len = s[1].size();
    for (int i = 0; i < len; ++i) p = ch[p][num(s[1][i])];
    cout << s[0] << ' ';
    if (ed[p] < 15) {
        m[0] = s[0].substr(1, s[0].size() - 2);
        //std::cerr << s[0] << ' ' << s[1] << '\n';
        //if (m[0] == "1097") while(1);
    
        int op = (this->*func[ed[p]])(m);
        if (op != 0 || out[ed[p]]) cout << op << '\n';
        return true; 
    }
    else { cout << "bye\n"; return false; }
}