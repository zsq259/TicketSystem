#ifndef TICKET_SYSTEM_TRAIN_H
#define TICKET_SYSTEM_TRAIN_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "../STLite/map.hpp"
#include "user.h"
using namespace sjtu;
using std::fstream;

class Date {
    public:
    int x = 0;
    Date() {}
    Date(int x_):x(x_) {}
    Date(std::string s) {
        int mm = 0, nn = 0;
        mm = s[1] - '0';
        nn = (s[3] - '0') * 10 + s[4] - '0';
        if (mm == 6) x = nn;
        if (mm == 7) x = nn + 30;
        if (mm == 8) x = nn + 61;
        if (mm == 9) x = nn + 92;
    }
    operator string() const {  
        if (!x) return "xx-xx";
        else if (x <= 30) return "06-" + string(x < 10? "0" : "") + std::to_string(x);
        else if (x <= 61) return "07-" + string(x < 40? "0" : "") + std::to_string(x - 30);
        else if (x <= 92) return "08-" + string(x < 71? "0" : "") + std::to_string(x - 61);
        else return "09-" + string(x < 102? "0" : "") + std::to_string(x - 92);
    }
    operator int() const { 
        return x;
    }
    const void print() const { cout << string(); }
};

class Time {
    public:
    int x = -1;
    Time() {}
    Time(int x_):x(x_) {}
    Time(std::string s) {
        int mm = 0, nn = 0;
        mm = (s[0] - '0') * 10 + s[1] - '0';
        nn = (s[3] - '0') * 10 + s[4] - '0';
        x = mm * 60 + nn;
    }
    operator string() const {
        if (x == -1)  return "xx:xx";
        else return string(x / 60 < 10? "0" : "") + std::to_string(x / 60) + ":" + string(x % 60 < 10? "0" : "") + std::to_string(x % 60);
    }
    operator int() const {
        return x;
    }
    const void print() const { cout << string(); }
};

class DateTime {
    public:
    Date date;
    Time time;
    DateTime():date(), time() {}
    DateTime(Date a, Time b):date(a), time(b) {}
    DateTime(string a, string b):date(a), time(b) {}
    DateTime(const DateTime &other):date(other.date), time(other.time) {}
    const void print() const { cout << (&date)->operator string() <<  ' ' << string(time); }
    DateTime &operator+ (int x) {
        // puts("\n\nstart");
        // print();
        int t = time;
        int d = date, h = t / 60, m = t % 60;
        // cout << "\nx= " << x << ' ' << t << ' ' << d << ' ' << h << ' ' << m << '\n';
        m += x;
        h += m / 60; m %= 60;
        d += h / 24; h %= 24;
        date = d; time = h * 60 + m;
        // print();
        // puts("end\n\n");
        return *this;
    }
    int operator-(const DateTime &other) {
        return ((int)date - (int)other.date) * 24 * 60 + (int)time - (int)other.time;
    }
    DateTime &operator+= (int x) {
        return (*this) + x;
    }
};

class TrainBase {
    public:
    my_string id;
    int seatNum, startTime, startSaleDate, endSaleDate;
    int place;
    TrainBase() {}
    TrainBase(const TrainBase &other): 
        id(other.id), place(other.place),  
        seatNum(other.seatNum), 
        startTime(other.startTime),
        startSaleDate(other.startSaleDate), endSaleDate(other.endSaleDate) {
    }
    TrainBase &operator = (const TrainBase &other) {
        if (&other == this) return *this;
        id = other.id; place = other.place;
        seatNum = other.seatNum; 
        startTime = other.startTime;
        startSaleDate = other.startSaleDate; endSaleDate = other.endSaleDate;
        return *this;
    }
    bool operator <  (const TrainBase &other) const { return id <  other.id; }
    bool operator >  (const TrainBase &other) const { return id >  other.id; }
    bool operator <= (const TrainBase &other) const { return id <= other.id; }
    bool operator >= (const TrainBase &other) const { return id >= other.id; }
    bool operator == (const TrainBase &other) const { return id == other.id; }
    bool operator != (const TrainBase &other) const { return id != other.id; }

};

class TrainStation: public TrainBase {
    public:
    int kth, price, arrivetime, stoptime;
    TrainStation():TrainBase() {}
    TrainStation(const TrainBase &other):TrainBase(other) {}
    TrainStation(const TrainStation &other):TrainBase(other), 
                                            kth(other.kth), 
                                            price(other.price), 
                                            arrivetime(other.arrivetime),
                                            stoptime(other.stoptime) {}
    TrainStation &operator= (const TrainStation &other) {
        if (&other == this) return *this;
        TrainBase::operator= (other);
        kth = other.kth;
        price = other.price;
        arrivetime = other.arrivetime;
        stoptime = other.stoptime;
        return *this;
    }
    bool operator <  (const TrainStation &other) const { return id <  other.id; }
    bool operator >  (const TrainStation &other) const { return id >  other.id; }
    bool operator <= (const TrainStation &other) const { return id <= other.id; }
    bool operator >= (const TrainStation &other) const { return id >= other.id; }
    bool operator == (const TrainStation &other) const { return id == other.id; }
    bool operator != (const TrainStation &other) const { return id != other.id; }
};

class Train: public TrainBase {
    private:
    static const int S = 102;
    public:
    my_string stations[S];
    int stationNum;
    int prices[S], travelTimes[S], stopoverTimes[S];
    char type;
    bool released = false;
    Train() {}
    Train(const Train &other): 
        TrainBase(other), released(other.released),  
        stationNum(other.stationNum), 
        type(other.type) {
        //for (int i = 0; i < stationNum; ++i) stations[i] = other.stations[i];
        memcpy(stations, other.stations, sizeof(stations));
        memcpy(prices, other.prices, sizeof(prices));
        memcpy(travelTimes, other.travelTimes, sizeof(travelTimes));
        memcpy(stopoverTimes, other.stopoverTimes, sizeof(stopoverTimes));
        // memcpy(prices, other.prices, sizeof(int) * stationNum);
        // memcpy(travelTimes, other.travelTimes, sizeof(int) * stationNum);
        // memcpy(stopoverTimes, other.stopoverTimes, sizeof(int) * stationNum);
    }
    Train &operator = (const Train &other) {
        if (&other == this) return *this;
        TrainBase::operator= (other);
        released = other.released;
        stationNum = other.stationNum;
        type = other.type; 
        //for (int i = 0; i < stationNum; ++i) stations[i] = other.stations[i];
        memcpy(stations, other.stations, sizeof(stations));
        memcpy(prices, other.prices, sizeof(prices));
        memcpy(travelTimes, other.travelTimes, sizeof(travelTimes));
        memcpy(stopoverTimes, other.stopoverTimes, sizeof(stopoverTimes));
        // memcpy(prices, other.prices, sizeof(int) * stationNum);
        // memcpy(travelTimes, other.travelTimes, sizeof(int) * stationNum);
        // memcpy(stopoverTimes, other.stopoverTimes, sizeof(int) * stationNum);
        return *this;
    }
    bool operator <  (const Train &other) const { return id <  other.id; }
    bool operator >  (const Train &other) const { return id >  other.id; }
    bool operator <= (const Train &other) const { return id <= other.id; }
    bool operator >= (const Train &other) const { return id >= other.id; }
    bool operator == (const Train &other) const { return id == other.id; }
    bool operator != (const Train &other) const { return id != other.id; }
};

template<class T>
class FileStore {
private:
    fstream io;
    public:
    explicit FileStore(const char FileName_[]) {
        io.open(FileName_, fstream::in);
        bool flag = io.is_open();
        io.close();
        if (!flag) {
            io.open(FileName_, fstream::out);
            io.close();
        }
        io.open(FileName_, fstream::in | fstream::out | fstream::binary);
    }
    ~FileStore() {}
    void read(int place, T &a) {
        io.seekg((place - 1) * sizeof(T));
        io.read(reinterpret_cast<char *>(&a), sizeof(a));
    }
    void write(int place, const T &a) {
        io.seekg((place - 1) * sizeof(T));
        io.write(reinterpret_cast<const char *>(&a), sizeof(a));
    }
};

class DateTrainSeat {
    private:
    static const int S = 102;
    int s[S] = {};
    public:
    DateTrainSeat() {}
    explicit DateTrainSeat(const DateTrainSeat &other) { memcpy(s, other.s, sizeof(s)); }
    explicit DateTrainSeat(const int* const p) { memcpy(s, p, sizeof(s)); }
    DateTrainSeat &operator=(const DateTrainSeat &other) { 
        if (this == &other) return *this;
        memcpy(s, other.s, sizeof(s)); 
        return *this;
    }
    int &operator[] (int x) { return s[x]; }
    const int operator[] (int x) const { return s[x]; }
    
};

class TrainSeat {
    private:
    static const int S = 96;
    DateTrainSeat s[S] = {};
    public:
    TrainSeat() = default;
    DateTrainSeat &operator[] (int x) { return s[x]; }
};

class SeatFile {
    private:
    fstream io;
    public:
    static int sum;
    explicit SeatFile(const char FileName_[]) {
        //std::cerr << sizeof(TrainSeat) << ' ' << sizeof(DateTrainSeat) << '\n';

        io.open(FileName_, fstream::in);
        bool flag = io.is_open();
        io.close();
        if (!flag) {
            io.open(FileName_, fstream::out);
            io.close();
        }
        io.open(FileName_, fstream::in | fstream::out | fstream::binary);
        if (!flag) {
            sum = 0;
            io.seekp(0);
            io.write(reinterpret_cast<const char *>(&sum), sizeof(int));
            io.seekg(0);
        }
        else {
            io.seekg(0);
            io.read(reinterpret_cast<char *>(&sum), sizeof(int));
        }
    }
    ~SeatFile() {
        io.seekp(0);
        io.write(reinterpret_cast<const char *>(&sum), sizeof(int));
    }
    void read(int place, int date, DateTrainSeat &a) {
        io.seekg(sizeof(int) + (place - 1) * sizeof(TrainSeat) + (date) * sizeof(DateTrainSeat));
        io.read(reinterpret_cast<char *>(&a), sizeof(a));
    }
    void read(int place, TrainSeat &a) {
        io.seekg(sizeof(int) + (place - 1) * sizeof(TrainSeat));
        io.read(reinterpret_cast<char *>(&a), sizeof(a));
    }
    void write(int place, int date, const DateTrainSeat &a) {
        io.seekp(sizeof(int) + (place - 1) * sizeof(TrainSeat) + (date) * sizeof(DateTrainSeat));
        io.write(reinterpret_cast<const char *>(&a), sizeof(a));
    }
    void write(int place, const TrainSeat &a) {
        io.seekg(sizeof(int) + (place - 1) * sizeof(TrainSeat));
        io.write(reinterpret_cast<const char *>(&a), sizeof(a));
    }
};

void cleanTrain();

int add_train (string (&m)[256]);
int delete_train (string (&m)[256]);
int release_train (string (&m)[256]);
int query_train (string (&m)[256]);

#endif