#ifndef TICKET_SYSTEM_TERMINAL_H
#define TICKET_SYSTEM_TERMINAL_H
#include <iostream>
#include "user.h"
#include "train.h"
#include "ticket.h"
using std::string;

class Terminal{
    private:
    
    BPlusTree<my_string, User> userdb = BPlusTree<my_string, User>("user.db", "user_bin.db");
    sjtu::map<my_string, bool> user_login;
    vector<User> array;

    BPlusTree<size_t, int> traindb = BPlusTree<size_t, int>("train.db", "train_bin.db");
    Train A;
    FileStore<Train> trains = FileStore<Train>("trains.db");
    SeatFile seats = SeatFile("seat.db");
    vector<int> rbq;
    std::hash<std::string> hashstr;
    BPlusTree<size_t, TrainStation> stationdb = BPlusTree<size_t, TrainStation>("station.db", "station_bin.db");
    BPlusTree<size_t, Order> orderdb = BPlusTree<size_t, Order>("order.db", "order_bin.db");
    BPlusTree<int, int> waitdb = BPlusTree<int, int>("wait.db", "wait_bin.db");
    vector<TrainStation> sArray, tArray;
    vector<pair<int, int> > v;
    vector<Order> orderArray;
    vector<int> waitArray;
    FileStore<Order> waits = FileStore<Order>("waits.db");

    static const int N = 100005;
    int ch[N][27], tot = 0, ed[N];
    string m[256];
    string s[24];
    string a[16] = { "add_user", "login", "logout", "query_profile", "modify_profile", 
                    "add_train", "delete_train", "release_train", "query_train", 
                    "query_ticket", "query_transfer", "buy_ticket", "query_order", 
                    "refund_ticket", "clean", "exit" };
    int (Terminal::*func[15])(string (&m)[256]) = { &Terminal::add_user, &Terminal::login, &Terminal::logout, &Terminal::query_profile, &Terminal::modify_profile, 
                                            &Terminal::add_train, &Terminal::delete_train, &Terminal::release_train, &Terminal::query_train, 
                                            &Terminal::query_ticket, &Terminal::query_transfer, &Terminal::buy_ticket, &Terminal::query_order, 
                                            &Terminal::refund_ticket, &Terminal::clean };
    int out[15] = {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1};
    char str[4005];
    public:
    friend class SeatFile;
    friend class Travel;
    void init();
    void cut(const char (&str)[4005]);
    bool find();
    void work() {
        init();
        while(1) {
            if (!std::cin.getline(str, 4000)) break;
            cut(str);
            if (!find()) break;
        }
    }
    
    int clean (string (&m)[256]);
    void cleanUser();
    bool isLogin(const my_string &id);
    int add_user (string (&m)[256]);
    int login (string (&m)[256]);
    int logout (string (&m)[256]);
    int query_profile (string (&m)[256]);
    int modify_profile (string (&m)[256]);    
    void cleanTrain();
    int add_train (string (&m)[256]);
    int delete_train (string (&m)[256]);
    int release_train (string (&m)[256]);
    int query_train (string (&m)[256]);
    
    void cleanTicket();
    int query_ticket (string (&m)[256]);
    int query_transfer (string (&m)[256]);
    int buy_ticket (string (&m)[256]);
    int query_order (string (&m)[256]);
    int refund_ticket (string (&m)[256]);

    bool findPlace(int &st, int &ed, const Train &a, const my_string &S, const my_string &T);
    bool compare(vector<pair<int, int>> &a, int *val, int p, int ret, const my_string &tmp);
    void sort(vector<pair<int, int> > &a, int l, int r, int *val);
    bool cmp(int op, int p1, int p2, int bp, int bt, int i, int j, int pri, int ti);
    void getTravel(const Train &a, DateTime &O, DateTime &o, 
              int &price, int &seat, int &date, int &num, int &st, int &ed, 
              const my_string &S, const my_string &T, DateTrainSeat &p);
    void checkWait();
};

#endif