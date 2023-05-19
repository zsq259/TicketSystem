#include "ticket.h"
#include "train.h"
#include "user.h"

BPlusTree<my_string, Train> stationdb("station.db", "station.bin");
BPlusTree<my_string, Order> orderdb("order.db", "order.bin");
BPlusTree<int, Order> waitdb("wait.db", "wait.bin");
vector<Train> sArray, tArray;
vector<int> v;
vector<Order> orderArray, waitArray;
static SeatFile seats("seat.db");

void stationAdd(const my_string &a, const Train &b) {
    stationdb.Insert(a, b);
}

void findPlace(int &st, int &ed, const Train &a, const my_string &S, const my_string &T) {
    for (int j = 0; j < a.stationNum; ++j) {
        if (a.stations[j] == S) st = j;
        if (a.stations[j] == T) ed = j;
    }
}

void sort(vector<int> &a, int l, int r, int *val) {
    int n = r - l;
    if (n <= 1) return ;
    int p1 = l, p2 = r - 1;
    int ret = val[l];
    while (p1 < p2) {
        while (p2 > p1 && val[p2] > ret) --p2;
        std::swap(a[p1], a[p2]), std::swap(val[p1], val[p2]);
        while (p1 < p2 && val[p1] < ret) ++p1;
        std::swap(a[p1], a[p2]), std::swap(val[p1], val[p2]);
    }
    sort(a, l, p1, val);
    sort(a, p1 + 1, r, val);
}

int query_ticket (string (&m)[256]) {
    if (!m['p'].size()) m['p'] = "time";
    int op = (m['p'] == "time");
    stationdb.Find(m['s'], sArray);
    stationdb.Find(m['t'], tArray);
    int p1 = 0, p2 = 0, k1 = sArray.size(), k2 = tArray.size();
    v.clear();
    int tot = 0, date = Date(m['d']);
    
    while (p1 < k1 && p2 < k2) {
        if (sArray[p1].id == tArray[p2].id) v.push_back(p1), ++tot, ++p1, ++p2;
        else if (sArray[p1].id < tArray[p2].id) ++p1;
        else ++p2;
    }
    
    int *val = new int[tot], *price = new int[tot];
    
    
    for (int i = 0; i < tot; ++i) {
        int x = v[i]; val[i] = 0;
        int st = 0, ed = 0;
        findPlace(st, ed, sArray[x], m['s'], m['t']);
        
        for (int j = st; j < ed; ++j) {
            if (!op) val[i] += sArray[x].prices[j];
            else val[i] += sArray[x].travelTimes[j];
        }
        if (op) for (int j = st; j < ed - 1; ++j) val[i] += sArray[x].stopoverTimes[j];
    }

    sort(v, 0, tot, val);
    int *b = new int[tot];
    int tt = tot;
    for (int i = 0; i < tot; ++i) {
        int x = v[i], st = 0, ed = 0;
        DateTime O(Date(m['d']), Time(sArray[x].startTime));
        date = Date(m['d']);
        findPlace(st, ed, sArray[x], m['s'], m['t']);
        for (int j = 0; j < st; ++j) O += sArray[x].stopoverTimes[j] + sArray[x].travelTimes[j];
        date -= (int)O.date - date;

        if (date < sArray[x].startSaleDate || date > sArray[x].endSaleDate) { --tt; continue; } 
    }
    cout << tt << '\n';
    if (!tt) return 0;
    DateTrainSeat p;
    for (int i = 0; i < tot; ++i) {
        int x = v[i]; b[i] = 0;
        int seatSum = 114514, st = 0, ed = 0, pri = 0;
        findPlace(st, ed, sArray[x], m['s'], m['t']);
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

        if (date < sArray[x].startSaleDate || date > sArray[x].endSaleDate) continue;

        O.date = m['d'];

        cout << sArray[x].id << ' ';
        cout << m['s'] << ' ';
        O.print();
        cout << " -> ";
        if (!op) O += b[i], pri = val[i];
        else O += val[i], pri = b[i];
        cout << m['t'] << ' ';
        O.print();
        cout << ' ' << pri << ' ';
        
        seats.read(sArray[x].place, date, p);
        for (int j = st; j < ed; ++j) seatSum = std::min(seatSum, p[j]);
        cout << seatSum << '\n';
    }
    delete []val; delete []price; delete []b;
    return 0;
}

int query_transfer (string (&m)[256]) { return 0; }

void getTravel(const Train &a, DateTime &O, DateTime &o, 
              int &price, int &seat, int &date, int &num, int &st, int &ed, 
              const my_string &S, const my_string &T, DateTrainSeat &p) {
    findPlace(st, ed, a, S, T);
    
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
    
    if (!sArray.size() || !sArray[0].released) return -1;
    
    DateTime O(Date(m['d']), Time(sArray[0].startTime));
    DateTime o;
    int st = 0, ed =0, price = 0, seat = 114514, date = Date(m['d']), num = stoi(m['n']);
    DateTrainSeat p;
    
    //std::cerr << "objk\n";
    
    getTravel(sArray[0], O, o, price, seat, date, num, st, ed, m['f'], m['t'], p);
    
    if (date < sArray[0].startSaleDate || date > sArray[0].endSaleDate) return -1;
    

    if (seat >= num) {
        Order a("[success]", m['i'], m['u'], m['f'], m['t'], stoi(m[0]), date, O.date, O.time, o.date, o.time, price, num);
        for (int i = st; i < ed; ++i) p[i] -= num;
        seats.write(sArray[0].place, date, p);
        orderdb.Insert(m['u'], a);
        cout << 1ll * price * num << '\n';
    }
    else {
        if (!m['q'].size() || m['q'] == "false") return -1;
        Order a("[pending]", m['i'], m['u'], m['f'], m['t'], stoi(m[0]), date, O.date, O.time, o.date, o.time, price, num);
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
        int price = 0, seat = 114514, date = Date(waitArray[i].sDate), num = waitArray[i].num;
        getTravel(sArray[0], O, o, price, seat, date, num, st, ed, waitArray[i].from, waitArray[i].to, p);
        if (date < sArray[0].startSaleDate || date > sArray[0].endSaleDate) continue;
        if (seat < num) continue;
        for (int i = st; i < ed; ++i) p[i] -= num;
        seats.write(sArray[0].place, date, p);
        waitdb.Delete(waitArray[i].id, waitArray[i]);
        orderdb.Delete(waitArray[i].userid, waitArray[i]);
        waitArray[i].statue = "[success]";
        orderdb.Insert(waitArray[i].userid, waitArray[i]);        
    }
}

int refund_ticket (string (&m)[256]) { 
    my_string id(m['u']);
    if (!isLogin(id)) return -1;
    int n = m['n'].size()? stoi(m['n']):1;
    orderdb.Find(id, orderArray);
    n = orderArray.size() - n;
    if (n < 0) return -1;
    orderdb.Delete(id, orderArray[n]);
    if (orderArray[n].statue.key[1] == 's') {
        DateTrainSeat p;
        findTrain(orderArray[0].trainid, sArray);
        seats.read(sArray[0].place, orderArray[n].date, p);
        int st = 0, ed = 0;
        findPlace(st, ed, sArray[0], orderArray[n].from, orderArray[n].to);
        for (int i = st; i < ed; ++i) p[i] += orderArray[n].num;
        seats.write(sArray[0].place, orderArray[0].date, p);
        checkWait();
    }
    else if (orderArray[n].statue.key[1] == 'p') {
        waitdb.Find(orderArray[n].id, waitArray);
        waitdb.Delete(orderArray[n].id, waitArray[0]);
    }
    else return 0;
    orderArray[n].statue = "[refunded]";
    orderdb.Insert(id, orderArray[n]);
    return 0;
}