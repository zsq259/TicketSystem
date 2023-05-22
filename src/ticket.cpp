#include "ticket.h"
#include "train.h"
#include "user.h"

BPlusTree<my_string, Train> stationdb("station.db", "station_bin.db");
BPlusTree<my_string, Order> orderdb("order.db", "order_bin.db");
BPlusTree<int, Order> waitdb("wait.db", "wait_bin.db");
vector<Train> sArray, tArray;
vector<int> v;
vector<Order> orderArray, waitArray;
static SeatFile seats("seat.db");

void cleanTicket() {
    (&stationdb)->~BPlusTree<my_string, Train>();
    std::filesystem::remove("station.db");
    std::filesystem::remove("station_bin.db");
    new (&stationdb) BPlusTree<my_string, Train>("station.db", "station_bin.db");

    (&orderdb)->~BPlusTree<my_string, Order>();
    std::filesystem::remove("order.db");
    std::filesystem::remove("order_bin.db");
    new (&orderdb) BPlusTree<my_string, Order>("order.db", "order_bin.db");

    (&waitdb)->~BPlusTree<int, Order>();
    std::filesystem::remove("wait.db");
    std::filesystem::remove("wait_bin.db");
    new (&waitdb) BPlusTree<int, Order>("wait.db", "wait_bin.db");

    (&seats)->~SeatFile();
    std::filesystem::remove("seat.db");
    new (&seats) SeatFile("seat.db");
}

void stationAdd(const my_string &a, const Train &b) {
    stationdb.Insert(a, b);
}

void stationDel(const my_string &a, const Train &b) {
    stationdb.Delete(a, b);
}

bool findPlace(int &st, int &ed, const Train &a, const my_string &S, const my_string &T) {
    int cnt = 0;
    for (int j = 0; j < a.stationNum; ++j) {
        if (a.stations[j] == S) st = j, ++cnt;
        if (a.stations[j] == T) ed = j, ++cnt;
    }
    return cnt == 2 && st < ed;
}

bool compare(vector<int> &a, int *val, int p, int ret, const my_string &tmp) {
    return val[p] > ret || (val[p] == ret && sArray[a[p]].id > tmp);
}

void sort(vector<int> &a, int l, int r, int *val) {
    if (r - l <= 1) return ;
    int p1 = l, p2 = r - 1;
    int ret = val[l];
    my_string tmp = sArray[a[l]].id;
    while (p1 < p2) {
        while (p2 > p1 && compare(a, val, p2, ret, tmp)) --p2;
        std::swap(a[p1], a[p2]), std::swap(val[p1], val[p2]);
        while (p1 < p2 && !compare(a, val, p1, ret, tmp)) ++p1;
        std::swap(a[p1], a[p2]), std::swap(val[p1], val[p2]);
    }
    sort(a, l, p1, val);
    sort(a, p1 + 1, r, val);
}

void printTravel(const Train &a, DateTime &O, const my_string &S, const my_string &T, 
                 int st, int ed, int date, int price, int time) {
    DateTrainSeat p;
    int seatSum = 114514;
    cout << a.id << ' ';
    cout << S << ' ';
    O.print();
    cout << " -> ";
    O += time;
    cout << T << ' ';
    O.print();
    cout << ' ' << price << ' ';
    seats.read(a.place, date, p);
    for (int j = st; j < ed; ++j) seatSum = std::min(seatSum, p[j]);
    cout << seatSum << '\n';
}

bool checkTrain(const Train &a, int D, const my_string &S, const my_string &T, int st, int ed) {
    if (!a.released) return false;
    if (st >= ed) return false;
    DateTime O(Date(D), Time(a.startTime));
    int date = D;
    for (int j = 0; j < st; ++j) O += a.stopoverTimes[j] + a.travelTimes[j];
    date -= (int)O.date - date;
    if (date < a.startSaleDate || date > a.endSaleDate) return false;
    return true;
}

int query_ticket (string (&m)[256]) {
    int op = (!m['p'].size() || m['p'] == "time");
    my_string S(m['s']), T(m['t']);
    stationdb.Find(S, sArray);
    stationdb.Find(T, tArray);    
    int p1 = 0, p2 = 0, k1 = sArray.size(), k2 = tArray.size();
    v.clear();
    int tot = 0, date = Date(m['d']);
    
    while (p1 < k1 && p2 < k2) {
        if (sArray[p1].id == tArray[p2].id) {
            int st = 0, ed = sArray[p1].stationNum;
            if (!findPlace(st, ed, sArray[p1], S, T)) { ++p1, ++p2; continue; }
            if (!checkTrain(sArray[p1], date, S, T, st, ed)) { ++p1, ++p2; continue; }
            v.push_back(p1), ++tot;
            ++p1, ++p2;
        }
        else if (sArray[p1].id < tArray[p2].id) ++p1;
        else ++p2;
    }

    
    tArray.clear();

    int *val = new int[tot];
    
    for (int i = 0; i < tot; ++i) {
        int x = v[i]; val[i] = 0;
        int st = 0, ed = 0;
        findPlace(st, ed, sArray[x], S, T);
        
        for (int j = st; j < ed; ++j) {
            if (!op) val[i] += sArray[x].prices[j];
            else val[i] += sArray[x].travelTimes[j];
        }
        if (op) for (int j = st; j < ed - 1; ++j) val[i] += sArray[x].stopoverTimes[j];
    }

    //if (tot > 1000) std::cerr << "tot=" << tot << '\n';

    sort(v, 0, tot, val);

    int *b = new int[tot];
    cout << tot << '\n';
    for (int i = 0; i < tot; ++i) {
        int x = v[i]; b[i] = 0;
        int st = 0, ed = 0;
        findPlace(st, ed, sArray[x], S, T);
        for (int j = st; j < ed; ++j) {
            if (!op) b[i] += sArray[x].travelTimes[j];
            else b[i] += sArray[x].prices[j];
        }
        if (!op) for (int j = st; j < ed - 1; ++j) b[i] += sArray[x].stopoverTimes[j];

        DateTime O(Date(m['d']), Time(sArray[x].startTime));
        date = Date(m['d']);

        for (int j = 0; j < st; ++j) O += sArray[x].stopoverTimes[j] + sArray[x].travelTimes[j];
        int tmp = date;
        date -= (int)O.date - tmp;

        //if (date < sArray[x].startSaleDate || date > sArray[x].endSaleDate) continue;

        O.date = m['d'];
        if(!op) printTravel(sArray[x], O, S, T, st, ed, date, val[i], b[i]);
        else printTravel(sArray[x], O, S, T, st, ed, date, b[i], val[i]);

        
    }
    
    delete []val; delete []b;
    sArray.clear(); v.clear();

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

void showTravel(const Train &a, int date, const my_string &S, const my_string &T) { //date: start from first station
    int st = 0, ed = 0;
    findPlace(st, ed, a, S, T);
    int price = 0, time = 0;
    DateTime O(Date(date), Time(a.startTime));
    for (int j = 0; j < st; ++j) O += a.stopoverTimes[j] + a.travelTimes[j];
    for (int j = st; j < ed; ++j) time += a.travelTimes[j], price += a.prices[j];
    for (int j = st; j < ed - 1; ++j) time += a.stopoverTimes[j];

    printTravel(a, O, S, T, st, ed, date, price, time);

}

int query_transfer (string (&m)[256]) {    
    int op = (!m['p'].size() || m['p'] == "time");
    int D = Date(m['d']), flag = 0;
    my_string S(m['s']), T(m['t']);
    
    stationdb.Find(S, sArray);
    stationdb.Find(T, tArray);
    int k1 = sArray.size(), k2 = tArray.size();
    
    if (!k1 || !k2) { cout << "0\n"; return 0; }
    int p1 = -1, p2 = -1, bprice = 0, btime = 0, trans = 0, d1 = 0, d2 = 0;
    for (int i = 0; i < k1; ++i) {
        int s1 = 0, t1 = sArray[i].stationNum - 1;
        findPlace(s1, t1, sArray[i], S, T);
        t1 = sArray[i].stationNum - 1;
        if (!checkTrain(sArray[i], D, S, sArray[i].stations[t1], s1, t1)) continue;
        DateTime O(Date(D), Time(sArray[i].startTime));
        int date = D, time = 0, price = 0;
        for (int j = 0; j < s1; ++j) O += sArray[i].travelTimes[j] + sArray[i].stopoverTimes[j];
        date -= (int)O.date - date;
        O.date = D;
        DateTime SO = O; 
        for (int j = s1 + 1; j <= t1; ++j) {
            my_string R(sArray[i].stations[j]);
            price += sArray[i].prices[j - 1];
            
            O += sArray[i].travelTimes[j - 1]; 
            for (int k = 0; k < k2; ++k) {
                if (sArray[i].id == tArray[k].id || !tArray[k].released) continue;
                int s2 = -1, t2 = tArray[k].stationNum - 1;
                if (!findPlace(s2, t2, tArray[k], R, T)) continue;
                int pri = price, da = O.date;
                
                DateTime o(da, tArray[k].startTime);
                for (int l = 0; l < s2; ++l) o += tArray[k].travelTimes[l] + tArray[k].stopoverTimes[l];
                int tmp = da;
                da -= o.date - da;
                o.date = tmp;
                if (o - O < 0) {
                    ++da;
                    o.date = o.date + 1;
                }
                if (da < tArray[k].startSaleDate) {
                    o.date = o.date + tArray[k].startSaleDate - da;
                    da = tArray[k].startSaleDate;
                }
                
                else if (da > tArray[k].endSaleDate) continue;
                for (int l = s2; l < t2; ++l) {
                    pri += tArray[k].prices[l];
                    o += tArray[k].travelTimes[l];
                }
                for (int l = s2; l < t2 - 1; ++l) o += tArray[k].stopoverTimes[l];
                if (p1 == -1 || cmp(op, p1, p2, bprice, btime, i, j, pri, o - SO)) {
                    p1 = i, p2 = k, flag = 1;
                    trans = j;
                    bprice = pri; btime = o - SO;
                    d1 = date; d2 = da;
                }
            }
            O += sArray[i].stopoverTimes[j - 1];
        }
    }
    
    if (!flag) { cout << "0\n"; return 0; }
    showTravel(sArray[p1], d1, S, sArray[p1].stations[trans]);
    showTravel(tArray[p2], d2, sArray[p1].stations[trans], T);
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

    if (!isLogin(m['u'])) return -1;
    findTrain(m['i'], sArray);
    my_string S(m['f']), T(m['t']);
    int num = stoi(m['n']);
    if (!sArray.size() || !sArray[0].released || num > sArray[0].seatNum) return -1;
    
    DateTime O(Date(m['d']), Time(sArray[0].startTime));
    DateTime o;
    int st = -1, ed = -1, price = 0, seat = 114514, date = Date(m['d']);
    DateTrainSeat p;
    if (!findPlace(st, ed, sArray[0], S, T)) return -1;
    if (!checkTrain(sArray[0], Date(m['d']), S, T, st, ed)) return -1;
    getTravel(sArray[0], O, o, price, seat, date, num, st, ed, S, T, p);
    

    if (seat >= num) {
        Order a("[success]", m['i'], m['u'], S, T, stoi(m[0]), date, O.date, O.time, o.date, o.time, price, num);
        for (int i = st; i < ed; ++i) p[i] -= num;
        seats.write(sArray[0].place, date, p);
        orderdb.Insert(m['u'], a);
        cout << 1ll * price * num << '\n';
    }
    else {
        if (!m['q'].size() || m['q'] == "false") return -1;
        Order a("[pending]", m['i'], m['u'], S, T, stoi(m[0]), date, O.date, O.time, o.date, o.time, price, num);
        waitdb.Insert(a.id, a);
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
    int st, ed, price, seat, date, num;
    for (int i = 0, k = waitArray.size(); i < k; ++i) {
        findTrain(waitArray[i].trainid, sArray);
        DateTime O(Date(waitArray[i].sDate), Time(sArray[0].startTime));
        DateTime o;
        st = 0, ed = -1;
        int price = 0, seat = 114514, date = Date(waitArray[i].sDate), num = waitArray[i].num;
        if (!findPlace(st, ed, sArray[0], waitArray[i].from, waitArray[i].to)) continue;
        if (ed < st) continue;
        getTravel(sArray[0], O, o, price, seat, date, num, st, ed, waitArray[i].from, waitArray[i].to, p);
        if (date < sArray[0].startSaleDate || date > sArray[0].endSaleDate) continue;
        if (seat < num) continue;
        for (int i = st; i < ed; ++i) p[i] -= num;
        seats.write(sArray[0].place, date, p);
        waitdb.Delete(waitArray[i].id, waitArray[i]);
        orderdb.Delete(waitArray[i].userid, waitArray[i]);
        string str = "[success]";
        waitArray[i].statue = str;
        orderdb.Insert(waitArray[i].userid, waitArray[i]);        
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
    if (orderArray[n].statue.key[1] == 's') {
        DateTrainSeat p;
        findTrain(orderArray[n].trainid, sArray);
        seats.read(sArray[0].place, orderArray[n].date, p);
        int st = 0, ed = 0;
        findPlace(st, ed, sArray[0], orderArray[n].from, orderArray[n].to);
        for (int i = st; i < ed; ++i) p[i] += orderArray[n].num;
        seats.write(sArray[0].place, orderArray[n].date, p);
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