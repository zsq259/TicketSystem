#include "ticket.h"
#include "train.h"
#include "user.h"

BPlusTree<my_string, TrainStation> stationdb("station.db", "station_bin.db");
BPlusTree<my_string, Order> orderdb("order.db", "order_bin.db");
BPlusTree<int, int> waitdb("wait.db", "wait_bin.db");
extern BPlusTree<my_string, int> traindb;
vector<TrainStation> sArray, tArray;
vector<pair<int, int> > v;
vector<Order> orderArray;
vector<int> waitArray;
FileStore<Order> waits("waits.db");
extern SeatFile seats;
extern FileStore<Train> trains;


void cleanTicket() {
    (&stationdb)->~BPlusTree<my_string, TrainStation>();
    std::filesystem::remove("station.db");
    std::filesystem::remove("station_bin.db");
    new (&stationdb) BPlusTree<my_string, Train>("station.db", "station_bin.db");

    (&orderdb)->~BPlusTree<my_string, Order>();
    std::filesystem::remove("order.db");
    std::filesystem::remove("order_bin.db");
    new (&orderdb) BPlusTree<my_string, Order>("order.db", "order_bin.db");

    (&waitdb)->~BPlusTree<int, int>();
    std::filesystem::remove("wait.db");
    std::filesystem::remove("wait_bin.db");
    new (&waitdb) BPlusTree<int, int>("wait.db", "wait_bin.db");

    (&waits)->~FileStore<Order>();
    std::filesystem::remove("waits.db");
    new (&trains) FileStore<Order>("waits.db");
}

bool findPlace(int &st, int &ed, const Train &a, const my_string &S, const my_string &T) {
    int cnt = 0;
    for (int j = 0; j < a.stationNum; ++j) {
        if (a.stations[j] == S) st = j, ++cnt;
        if (a.stations[j] == T) ed = j, ++cnt;
    }
    return cnt == 2 && st < ed;
}

bool compare(vector<pair<int, int>> &a, int *val, int p, int ret, const my_string &tmp) {
    return val[p] > ret || (val[p] == ret && sArray[a[p].first].id > tmp);
}

void sort(vector<pair<int, int> > &a, int l, int r, int *val) {
    if (r - l <= 1) return ;
    int p1 = l, p2 = r - 1;
    int ret = val[l];
    my_string tmp = sArray[a[l].first].id;
    while (p1 < p2) {
        while (p2 > p1 && compare(a, val, p2, ret, tmp)) --p2;
        std::swap(a[p1], a[p2]), std::swap(val[p1], val[p2]);
        while (p1 < p2 && !compare(a, val, p1, ret, tmp)) ++p1;
        std::swap(a[p1], a[p2]), std::swap(val[p1], val[p2]);
    }
    sort(a, l, p1, val);
    sort(a, p1 + 1, r, val);
}

class Travel {
    TrainBase *a;
    DateTime O; // start time
    my_string S, T;
    int st, ed, date, price, time;
    public:
    Travel() {}
    ~Travel() {}
    Travel(TrainBase *a_, const DateTime &O_, const my_string S_, const my_string T_, 
           int st_, int ed_, int date_, int price_, int time_):
           a(a_), O(O_), S(S_), T(T_), st(st_), ed(ed_), date(date_), price(price_), time(time_) {}
    Travel &operator=(const Travel &other) = default;
    void print() {
        cout << a->id << ' ';
        cout << S << ' ';
        O.print();
        cout << " -> ";
        O += time;
        cout << T << ' ';
        O.print();
        cout << ' ' << price << ' ';
        int seatSum = 114514;
        DateTrainSeat p;
        seats.read(a->place, date, p);
        for (int j = st; j < ed; ++j) seatSum = std::min(seatSum, p[j]);
        cout << seatSum << '\n';
    }
};

int query_ticket (string (&m)[256]) {
    int op = (!m['p'].size() || m['p'] == "time");
    my_string S(m['s']), T(m['t']);
    stationdb.Find(S, sArray);
    stationdb.Find(T, tArray);    
    int p1 = 0, p2 = 0, k1 = sArray.size(), k2 = tArray.size();
    v.clear();
    int tot = 0, D = Date(m['d']);
    while (p1 < k1 && p2 < k2) {
        if (sArray[p1].id == tArray[p2].id) {
            if (sArray[p1].kth >= tArray[p2].kth) { ++p1, ++p2; continue; }
            DateTime O = DateTime(D, sArray[p1].startTime) + (sArray[p1].arrivetime + sArray[p1].stoptime);
            int da = D;
            da -= O.date - da;
            if (da < sArray[p1].startSaleDate || da > sArray[p1].endSaleDate) { ++p1, ++p2; continue; }
            v.push_back(pair(p1, p2)), ++tot;
            ++p1, ++p2;
        }
        else if (sArray[p1].id < tArray[p2].id) ++p1;
        else ++p2;
    }
    int *val = new int[tot], *b = new int[tot];
    
    for (int i = 0; i < tot; ++i) {
        int p1 = v[i].first, p2 = v[i].second; 
        if (!op) val[i] = tArray[p2].price - sArray[p1].price;
        else val[i] = tArray[p2].arrivetime - sArray[p1].arrivetime - sArray[p1].stoptime;
    }
    sort(v, 0, tot, val);
    cout << tot << '\n';
    for (int i = 0; i < tot; ++i) {
        int p1 = v[i].first, p2 = v[i].second; 
        if (op) b[i] = tArray[p2].price - sArray[p1].price;
        else b[i] = tArray[p2].arrivetime - sArray[p1].arrivetime - sArray[p1].stoptime;
        DateTime O(D, sArray[p1].startTime);
        int date = D;
        //O += (tArray[p2].arrivetime - sArray[p1].arrivetime + sArray[p1].stoptime);
        O += sArray[p1].arrivetime + sArray[p1].stoptime;
        date -= (int)O.date - date;
        O.date = D;
        if(!op) Travel(&sArray[p1], O, S, T, sArray[p1].kth, tArray[p2].kth, date, val[i], b[i]).print();
        else Travel(&sArray[p1], O, S, T, sArray[p1].kth, tArray[p2].kth, date, b[i], val[i]).print();
    }
    delete []val; delete []b;
    sArray.clear(); tArray.clear();
    return 0;
}

bool cmp(int op, int p1, int p2, int bp, int bt, int i, int j, int pri, int ti) {
    if (op) {
        if (bt ^ ti) return ti < bt;
        if (bp ^ pri) return pri < bp;
    }
    else {
        if (bp ^ pri) return pri < bp;
        if (bt ^ ti) return ti < bt;
    }
    if (sArray[p1].id != sArray[i].id) return sArray[i].id < sArray[p1].id;
    return tArray[j].id < tArray[p2].id;
}

int query_transfer (string (&m)[256]) {    
    int op = (!m['p'].size() || m['p'] == "time");
    my_string S(m['s']), T(m['t']);
    stationdb.Find(S, sArray);
    stationdb.Find(T, tArray);
    int k1 = sArray.size(), k2 = tArray.size();
    if (!k1 || !k2) { cout << "0\n"; return 0; }
    int D = Date(m['d']), flag = 0;
    int p1 = -1, p2 = -1, bprice = 0, btime = 0;
    Travel bS, bT;
    my_string trans;
    for (int i = 0; i < k1; ++i) {
        int pla = 0;;
        traindb.Find(sArray[i].id, pla);
        Train A;
        trains.read(pla, A);
        int s1 = sArray[i].kth, t1 = A.stationNum - 1;
        DateTime O(D, A.startTime);
        O += (sArray[i].arrivetime + sArray[i].stoptime);
        int date = D;
        date -= O.date - date;
        if (date < A.startSaleDate || date > A.endSaleDate) continue;
        O.date = D;
        DateTime SO = O; // get on train A
        for (int k = 0; k < k2; ++k) {
            if (tArray[k].id == A.id) continue;
            traindb.Find(tArray[k].id, pla);
            Train B;
            trains.read(pla, B);
            for (int j = s1 + 1; j <= t1; ++j) {
                my_string R(A.stations[j]);
                int s2 = -1, t2 = tArray[k].kth;
                for (int l = 0; l < B.stationNum; ++l) if (B.stations[l] == R) s2 = l;
                if (s2 == -1 || s2 >= t2) continue;
                int pri = 0, ti = 0;
                O = SO;
                for (int l = s1; l < j; ++l) pri += A.prices[l], ti += A.travelTimes[l];
                for (int l = s1; l < j - 1; ++l) ti += A.stopoverTimes[l];
                O += ti; // arrive R
                int da = O.date, time = 0, price = pri;
                DateTime o(O.date, B.startTime);
                for (int l = 0; l < s2; ++l) time += B.travelTimes[l] + B.stopoverTimes[l]; 
                o += time;
                da -= o.date.x - da;
                o.date = O.date;
                
                if (o - O < 0) {
                    ++da; 
                    ++o.date.x;
                }
                
                if (da < B.startSaleDate) {                    
                    o.date.x += (B.startSaleDate - da);
                    da = B.startSaleDate;
                }
                else if (da > B.endSaleDate) continue;
                DateTime so = o; time = 0;
                for (int l = s2; l < t2; ++l) {
                    price += B.prices[l];
                    time += B.travelTimes[l];
                }
                for (int l = s2; l < t2 - 1; ++l) time += B.stopoverTimes[l];
                o += time;
                if (!flag || cmp(op, p1, p2, bprice, btime, i, j, price, o - SO)) {
                    p1 = i, p2 = k, flag = 1;
                    trans = R;
                    bprice = price; btime = o - SO;
                    bS = Travel(&sArray[i], SO, S, R, s1, j, date, pri, ti);
                    bT = Travel(&tArray[k], so, R, T, s2, t2, da, price - pri, time);
                }
            }
        }
        
    }
    if (!flag) { cout << "0\n"; return 0; }
    bS.print(); bT.print();
    return 0;
}

void getTravel(const Train &a, DateTime &O, DateTime &o, 
              int &price, int &seat, int &date, int &num, int &st, int &ed, 
              const my_string &S, const my_string &T, DateTrainSeat &p) {
    
    for (int j = 0; j < st; ++j) O += a.stopoverTimes[j] + a.travelTimes[j];
    int tmp = date;
    date -= (int)O.date - tmp; 
    O.date = tmp;
    o = O;
    
    for (int i = st; i < ed; ++i) o += a.travelTimes[i];
    for (int i = st; i < ed - 1; ++i) o += a.stopoverTimes[i];
    
    seats.read(a.place, date, p);

    for (int i = st; i < ed; ++i) {
        price += a.prices[i];
        seat = std::min(seat, p[i]);
    }
}

int buy_ticket (string (&m)[256]) {
    my_string id(m['i']), uid(m['u']);
    if (!isLogin(uid)) return -1;
    int pla = -1;
    traindb.Find(id, pla);
    if (pla == -1) return -1;
    Train A;
    trains.read(pla, A);
    int num = stoi(m['n']);    
    if (!A.released || num > A.seatNum) return -1;
    my_string S(m['f']), T(m['t']);
    int D = Date(m['d']);
    

    int st = -1, ed = -1, price = 0, seat = 114514, date = D;
    
    for (int i = 0; i < A.stationNum; ++i) {
        if (A.stations[i] == S) st = i;
        if (A.stations[i] == T) ed = i;
    }
    
    if (st == -1 || ed == -1 || st >= ed) return -1;
    
    DateTime O(D, A.startTime);
    for (int j = 0; j < st; ++j) O += A.stopoverTimes[j] + A.travelTimes[j];
    date -= O.date - date;
    if (date < A.startSaleDate || date > A.endSaleDate) return -1;
    O.date = D;
    DateTime o(O);
    for (int i = st; i < ed; ++i) o += A.travelTimes[i];
    for (int i = st; i < ed - 1; ++i) o += A.stopoverTimes[i];
    
    DateTrainSeat p;
    seats.read(A.place, date, p);

    for (int i = st; i < ed; ++i) {
        price += A.prices[i];
        seat = std::min(seat, p[i]);
    }
    
    if (seat >= num) {
        Order a("[success]", m['i'], m['u'], S, T, stoi(m[0]), date, price, num, O, o);
        for (int i = st; i < ed; ++i) p[i] -= num;
        seats.write(A.place, date, p);
        orderdb.Insert(m['u'], a);
        cout << 1ll * price * num << '\n';
    }
    else {
        if (!m['q'].size() || m['q'] == "false") return -1;
        Order a("[pending]", m['i'], m['u'], S, T, stoi(m[0]), date, price, num, O, o);
        waitdb.Insert(a.id, a.id);
        waits.write(a.id, a);
        orderdb.Insert(a.userid, a);
        cout << "queue\n";
    }
    return 0;
}

int query_order (string (&m)[256]) {
    my_string id(m['u']);
    if (!isLogin(id)) return -1;
    orderdb.Find(id, orderArray);
    cout << orderArray.size() << '\n';
    for (int i = orderArray.size() - 1; i >= 0; --i) orderArray[i].print();
    return 0;
}

void checkWait() {
    waitdb.FindAll(waitArray);
    DateTrainSeat p;
    Train A;
    Order B;
    int st, ed, price, seat, date, num;
    for (int i = 0, k = waitArray.size(); i < k; ++i) {
        int pla = 0;
        waits.read(waitArray[i], B);
        traindb.Find(B.trainid, pla);
        trains.read(pla, A);
        DateTime O(B.O.date, A.startTime);
        DateTime o;
        st = 0, ed = -1;
        int price = 0, seat = 114514, date = Date(B.O.date), num = B.num;
        if (!findPlace(st, ed, A, B.from, B.to)) continue;
        if (ed < st) continue;
        getTravel(A, O, o, price, seat, date, num, st, ed, B.from, B.to, p);
        if (date < A.startSaleDate || date > A.endSaleDate) continue;
        if (seat < num) continue;
        for (int i = st; i < ed; ++i) p[i] -= num;
        seats.write(A.place, date, p);
        waitdb.Delete(B.id, B.id);
        orderdb.Delete(B.userid, B);
        string str = "[success]";
        B.statue = str;
        orderdb.Insert(B.userid, B);        
    }
    waitArray.clear();
}

int refund_ticket (string (&m)[256]) { 
    my_string id(m['u']);
    if (!isLogin(id)) return -1;
    int n = m['n'].size()? stoi(m['n']):1;
    orderdb.Find(id, orderArray);
    n = orderArray.size() - n;
    if (n < 0 || orderArray[n].statue.key[1] == 'r') return -1;
    orderdb.Delete(id, orderArray[n]);
    Train A;
    if (orderArray[n].statue.key[1] == 's') {
        DateTrainSeat p;
        int pla = 0;
        traindb.Find(orderArray[n].trainid, pla);
        trains.read(pla, A);
        seats.read(A.place, orderArray[n].date, p);
        int st = 0, ed = 0;
        for (int i = 0; i < A.stationNum; ++i) {
        if (A.stations[i] == orderArray[n].from) st = i;
        if (A.stations[i] == orderArray[n].to) ed = i;
    }
        for (int i = st; i < ed; ++i) p[i] += orderArray[n].num;
        seats.write(A.place, orderArray[n].date, p);
        checkWait();
    }
    else if (orderArray[n].statue.key[1] == 'p') {
        waitdb.Find(orderArray[n].id, waitArray);
        waitdb.Delete(orderArray[n].id, waitArray[0]);
    }
    string str = "[refunded]";
    orderArray[n].statue = str;
    orderdb.Insert(id, orderArray[n]);    
    return 0;
}