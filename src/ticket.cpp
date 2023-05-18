#include "ticket.h"
#include "train.h"

BPlusTree<my_string, Train> stationdb("station.db", "station.bin");
vector<Train> sArray, tArray;
vector<int> v;
static SeatFile seats("seat.db");

void stationAdd(const my_string &a, const Train &b) {
    stationdb.Insert(a, b);
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
    int op = (m['p'] == "time");
    stationdb.Find(m['s'], sArray);
    stationdb.Find(m['t'], tArray);
    int p1 = 0, p2 = 0, k1 = sArray.size(), k2 = tArray.size();
    v.clear();
    int tot = 0, date = Date(m['d']);
    while (p1 < k1 && p2 < k2) {
        if (sArray[p1].id == tArray[p2].id) v.push_back(p1), ++tot;
        else if (sArray[p1].id < tArray[p2].id) ++p1;
        else ++p2;
    }
    int *val = new int[tot], *price = new int[tot];
    DateTrainSeat p;
    for (int i = 0; i < tot; ++i) {
        int x = v[i]; val[i] = 0;
        int st = 0, ed = 0;
        for (int j = 0; j <= sArray[x].stationNum; ++j) {
            if (sArray[x].stations[j] == m['s']) st = j;
            if (sArray[x].stations[j] == m['t']) ed = j;
        }
        
        if (!op) { for (int j = st; j < ed; ++j) val[i] += sArray[x].prices[j]; }
        else {
            for (int j = st; j < ed; ++j) val[i] += sArray[x].stopoverTimes[i] + sArray[x].travelTimes[i];
        }
    }
    sort(v, 0, tot, val);
    for (int i = 0; i < tot; ++i) {
        int x = v[i];
        seats.read(sArray[x].place, date, p);
        int seatSum = 114514;
        
    }
    return 0;
}

int query_transfer (string (&m)[256]) { puts("11"); return 0; }
int buy_ticket (string (&m)[256]) { puts("12"); return 0; }
int query_order (string (&m)[256]) { puts("13"); return 0; }
int refund_ticket (string (&m)[256]) { puts("14"); return 0; }