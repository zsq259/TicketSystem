#include "train.h"
#include "ticket.h"
#include <filesystem>

BPlusTree<my_string, Train> traindb("train.db", "train_bin.db");
vector<Train> trainArray;
static SeatFile seats("seat.db");
int SeatFile::sum;

void cleanTrain() {
    (&traindb)->~BPlusTree<my_string, Train>();
    std::filesystem::remove("train.db");
    std::filesystem::remove("train_bin.db");
    new (&traindb) BPlusTree<my_string, Train>("train.db", "train_bin.db");

    (&seats)->~SeatFile();
    std::filesystem::remove("seat.db");
    new (&seats) SeatFile("seat.db");
}

void findTrain(const my_string &a, vector<Train> &v) {
    traindb.Find(a, v);
}

int add_train (string (&m)[256]) {
    

    traindb.Find(m['i'], trainArray);
    if (trainArray.size()) return -1;
    
    Train a;
    a.id = m['i']; a.place = ++SeatFile::sum;
    a.stationNum = stoi(m['n']);
    a.seatNum = stoi(m['m']);
    a.startTime = Time(m['x']);
    a.type = m['y'][0];
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
    
    
    
    for (int i = 0; i < a.stationNum; ++i) {
        stationAdd(a.stations[i], a);
    }
    traindb.Insert(a.id, a);

    DateTrainSeat p;
    for (int i = 0; i < a.stationNum; ++i) p[i] = a.seatNum;
    TrainSeat P;
    for (int i = Date("06-01"), k = Date("08-31"); i <= k; ++i) P[i] = p;

    seats.write(a.place, P);
    return 0;
}

int delete_train (string (&m)[256]) { 
    
    traindb.Find(m['i'], trainArray);
    //return 0;
    
    if (!trainArray.size()) return -1;
    if (trainArray[0].released) return -1;
    traindb.Delete(m['i'], trainArray[0]);
    for (int i = 0; i < trainArray[0].stationNum; ++i) {
        stationDel(trainArray[0].stations[i], trainArray[0]);
    }

    return 0;
}
int release_train (string (&m)[256]) {

    my_string id(m['i']);
    traindb.Find(id, trainArray);
    if (!trainArray.size()) return -1;
    if (trainArray[0].released) return -1;
    traindb.Delete(id, trainArray[0]);
    for (int i = 0; i < trainArray[0].stationNum; ++i) {
        //std::cerr << "i=" << i << trainArray[0].stations[i] << '\n';
        stationDel(trainArray[0].stations[i], trainArray[0]);
    }
    trainArray[0].released = true;
    traindb.Insert(id, trainArray[0]);
    for (int i = 0; i < trainArray[0].stationNum; ++i) stationAdd(trainArray[0].stations[i], trainArray[0]);
    return 0;
}
int query_train (string (&m)[256]) {

    //return 0;

    traindb.Find(m['i'], trainArray);
    if (!trainArray.size()) return -1;
    int day = Date(m['d']);
    if (day < trainArray[0].startSaleDate || day > trainArray[0].endSaleDate) return -1;
    std::cout << trainArray[0].id << ' ' << trainArray[0].type << '\n';
    DateTrainSeat p;
    DateTime O(Date(m['d']), Time(trainArray[0].startTime));
    int price = 0;
    seats.read(trainArray[0].place, O.date, p);

    for (int i = 0; i < trainArray[0].stationNum; ++i) {
        cout << trainArray[0].stations[i] << ' ';


        if (!i) cout << Date(0).s << ' ' << Time(-1).s << " -> ";
        else { O.print(); cout << " -> "; }

        

        if (i) O += trainArray[0].stopoverTimes[i - 1];
        
        
        if (i != trainArray[0].stationNum - 1) O.print();
        else cout <<Date(0).s << ' ' << Time(-1).s;
        
        
        
        cout << ' ' << price << ' ';
        if (i == trainArray[0].stationNum - 1) cout << "x\n";
        else cout << p[i] << '\n';
        
        

        price += trainArray[0].prices[i];
        O += trainArray[0].travelTimes[i];
        
    }
    return 0;
}