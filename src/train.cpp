#include "train.h"
#include "ticket.h"
#include <filesystem>

BPlusTree<size_t, int> traindb("train.db", "train_bin.db");
extern BPlusTree<size_t, TrainStation> stationdb;
Train A;
FileStore<Train> trains("trains.db");
SeatFile seats("seat.db");
vector<int> rbq;
std::hash<std::string> hashstr;
int SeatFile::sum;
//int cnt1 = 0, cnt2 = 0;

void cleanTrain() {
    (&traindb)->~BPlusTree<size_t, int>();
    std::filesystem::remove("train.db");
    std::filesystem::remove("train_bin.db");
    new (&traindb) BPlusTree<size_t, int>("train.db", "train_bin.db");

    (&seats)->~SeatFile();
    std::filesystem::remove("seat.db");
    new (&seats) SeatFile("seat.db");

    (&trains)->~FileStore<Train>();
    std::filesystem::remove("trains.db");
    new (&trains) FileStore<Train>("trains.db");
}


int add_train (string (&m)[256]) {
    int pla = -1;
    traindb.Find(hashstr(m['i']), pla);
    if (pla != -1) return -1;
    Train a;
    a.id = m['i']; 
    if (rbq.size()) a.place = rbq.back(), rbq.pop_back();
    else a.place = ++SeatFile::sum;
    a.stationNum = stoi(m['n']);
    a.seatNum = stoi(m['m']);
    a.startTime = Time(m['x']);
    a.type = m['y'][0];
    a.released = 0;
    int tot = 0;
    string str;
    for (int i = 0, k = m['s'].size(); i < k; ++i) {
        if (m['s'][i] == '|') a.stations[tot] = str, ++tot, str.clear();
        else str += m['s'][i];
    }
    a.stations[tot] = str; tot = 0; str.clear();

    for (int i = 0, k = m['p'].size(); i < k; ++i) {
        if (m['p'][i] == '|') a.prices[tot] = stoi(str), ++tot, str.clear();
        else str += m['p'][i];
    }
    a.prices[tot] = stoi(str); tot = 0; str.clear();


    for (int i = 0, k = m['t'].size(); i < k; ++i) {
        if (m['t'][i] == '|') a.travelTimes[tot] = stoi(str), ++tot, str.clear();
        else str += m['t'][i];
    }
    a.travelTimes[tot] = stoi(str); tot = 0; str.clear();

    if (a.stationNum > 2) {
        for (int i = 0, k = m['o'].size(); i < k; ++i) {
            if (m['o'][i] == '|') a.stopoverTimes[tot] = stoi(str), ++tot, str.clear();
            else str += m['o'][i];
        }
        a.stopoverTimes[tot] = stoi(str); tot = 0; str.clear();
    }

    for (int i = 0, k = m['d'].size(); i < k; ++i) {
        if (m['d'][i] == '|') a.startSaleDate = Date(str), ++tot, str.clear();
        else str += m['d'][i];
    }

    a.endSaleDate = Date(str);
        
    traindb.Insert(hashstr(m['i']), a.place);
    trains.write(a.place, a);
    DateTrainSeat p;
    for (int i = 0; i < a.stationNum; ++i) p[i] = a.seatNum;
    TrainSeat P;
    for (int i = Date("06-01"), k = Date("08-31"); i <= k; ++i) P[i] = p;

    seats.write(a.place, P);
    //++cnt1;
    return 0;
}

int delete_train (string (&m)[256]) { 
    int pla = -1;
    traindb.Find(hashstr(m['i']), pla);
    if (pla == -1) return -1;
    trains.read(pla, A);
    if (A.released) return -1;
    traindb.Delete(hashstr(m['i']), A.place);
    rbq.push_back(A.place);
    //++cnt2;
    return 0;
}
int release_train (string (&m)[256]) {
    my_string id(m['i']);
    int pla = -1;
    traindb.Find(hashstr(m['i']), pla);
    
    if (pla == -1) return -1;
    trains.read(pla, A);
    if (A.released) return -1;
    traindb.Delete(hashstr(m['i']), A.place);
    A.released = true;
    traindb.Insert(hashstr(m['i']), A.place);
    trains.write(A.place, A);
    int time = 0, price = 0;
    TrainStation b(A);
    for (int i = 0; i < A.stationNum; ++i) {
        b.kth = i; b.price = price; b.arrivetime = time;
        if (i && i != A.stationNum - 1) b.stoptime = A.stopoverTimes[i - 1];
        else b.stoptime = 0; 
        stationdb.Insert(hashstr(A.stations[i]), b);
        price += A.prices[i];
        time += b.stoptime + A.travelTimes[i];
    }
    return 0;
}
int query_train (string (&m)[256]) {

    //return 0;
    int pla = -1;
    traindb.Find(hashstr(m['i']), pla);
    if (pla == -1) return -1;
    trains.read(pla, A);
    int day = Date(m['d']);
    if (day < A.startSaleDate || day > A.endSaleDate) return -1;
    std::cout << A.id << ' ' << A.type << '\n';
    DateTrainSeat p;
    DateTime O(Date(m['d']), Time(A.startTime));
    int price = 0;
    seats.read(A.place, O.date, p);

    for (int i = 0; i < A.stationNum; ++i) {
        cout << A.stations[i] << ' ';


        if (!i) cout << string(Date(0)) << ' ' << string(Time(-1)) << " -> ";
        else { O.print(); cout << " -> "; }

        

        if (i) O += A.stopoverTimes[i - 1];
        
        
        if (i != A.stationNum - 1) O.print();
        else cout << string(Date(0)) << ' ' << string(Time(-1));
        
        
        
        cout << ' ' << price << ' ';
        if (i == A.stationNum - 1) cout << "x\n";
        else cout << p[i] << '\n';
        
        

        price += A.prices[i];
        O += A.travelTimes[i];
        
    }
    return 0;
}