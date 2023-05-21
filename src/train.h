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
    std::string s;
    Date() {}
    Date(std::string s_):s(s_) {}
    Date(int x) {  
        if (!x) s =  "xx-xx";
        else if (x <= 30) s =  "06-" + string(x < 10? "0" : "") + std::to_string(x);
        else if (x <= 61) s =  "07-" + string(x < 40? "0" : "") + std::to_string(x - 30);
        else if (x <= 92) s =  "08-" + string(x < 71? "0" : "") + std::to_string(x - 61);
        else s =  "09-" + string(x < 102? "0" : "") + std::to_string(x - 92);
    }
    operator int() const { 
        int mm = 0, nn = 0;
        mm = s[1] - '0';
        nn = (s[3] - '0') * 10 + s[4] - '0';
        if (mm == 6) return nn;
        if (mm == 7) return nn + 30;
        if (mm == 8) return nn + 61;
        if (mm == 9) return nn + 92;
        return 0;
    }
    const void print() const { cout << s; }
};

class Time {
    public:
    std::string s;
    Time() {}
    Time(std::string s_):s(s_) {}
    Time(int x) {
        if (x == -1)  s =  "xx:xx";
        else s = string(x / 60 < 10? "0" : "") + std::to_string(x / 60) + ":" + string(x % 60 < 10? "0" : "") + std::to_string(x % 60);
    }
    operator int() const {
        int mm = 0, nn = 0;
        mm = (s[0] - '0') * 10 + s[1] - '0';
        nn = (s[3] - '0') * 10 + s[4] - '0';
        return mm * 60 + nn;
    }
    const void print() const { cout << s; }
};

class DateTime {
    public:
    Date date;
    Time time;
    DateTime():date(), time() {}
    DateTime(Date a, Time b):date(a), time(b) {}
    DateTime(string a, string b):date(a), time(b) {}
    const void print() const { date.print(); cout << ' '; time.print(); }
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

class Train {
    private:
    static const int S = 102;
    public:
    my_string id;
    my_string stations[S];
    int stationNum, seatNum, startTime, startSaleDate, endSaleDate;
    int prices[S], travelTimes[S], stopoverTimes[S];
    char type;
    bool released = false;
    int place;
    Train() {}
    Train(const Train &other): 
        id(other.id), released(other.released), place(other.place),  
        stationNum(other.stationNum), seatNum(other.seatNum), 
        startTime(other.startTime), type(other.type), 
        startSaleDate(other.startSaleDate), endSaleDate(other.endSaleDate) {
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
        id = other.id; released = other.released; place = other.place;
        stationNum = other.stationNum; seatNum = other.seatNum; 
        startTime = other.startTime; type = other.type; 
        startSaleDate = other.startSaleDate; endSaleDate = other.endSaleDate;
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
void findTrain(const my_string &a, vector<Train> &v);

int add_train (string (&m)[256]);
int delete_train (string (&m)[256]);
int release_train (string (&m)[256]);
int query_train (string (&m)[256]);

#endif