#ifndef TICKET_SYSTEM_B_PLUS_TREE_H
#define TICKET_SYSTEM_B_PLUS_TREE_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "utility.hpp"
#include "vector.hpp"
using std::fstream;
using std::istream;
using std::ostream;
using sjtu::vector;

template<class T>
class LinkedHashMap {
private:
    const static int N = 228;
    const static int maxSize = 114;
    struct node {
        node* next = nullptr;
        node* timePre = nullptr, *timeNxt = nullptr;
        int key;
        T value;
        node() = default;
        node(int key_, const T &value_):key(key_), value(value_) {}
        node &operator =(const node &other) {
            if (&other == this) return *this;
            next = other.next;
            timePre = other.timePre; timeNxt = other.timeNxt;
            key = other.key;
            value = other.value;
        }
    };
    int size = 0;
    node beg;
    node* head[N + 1], *cur = &beg;
    
public:
    inline void insertTime(node* p) {
        cur->timeNxt = p;
        p->timePre = cur;
        cur = p;
    }
    inline void removeTime(node* p) {
        if (p->timePre) p->timePre->timeNxt = p->timeNxt;
        if (p->timeNxt) p->timeNxt->timePre = p->timePre;
        p->timePre = p->timeNxt = nullptr;
    }
    inline void updateTime(node* p) {
        removeTime(p); insertTime(p);
    }
    bool find(int key, T &a) {
        int o = key % N;
        node* p = head[o];
        //std::cerr << "key= " << key << '\n';
        while (p) {
            if (p->key == key) { a = p->value; updateTime(p); return true; }
            p = p->next;
        }
        return false;
    }
    void insert(int key, const T &a) {
        //std::cerr << "begin insert\n";
        int o = key % N;
        node *p = head[o], *q = nullptr;
        if (!p) {
            p = head[o] = new node(key, a);
            insertTime(p); ++size;
            //std::cerr << "end insert\n";
            return ;
        }
        while (p) {
            if (p->key == key) { p->value = a; updateTime(p); return ; }
            q = p; p = p->next;
        }
        p = new node(key, a);
        q->next = p;
        insertTime(p); ++size;
        //std::cerr << "end insert\n";
    }
    void remove(node* p) {
        node* q = head[p->key % N];
        if (q == p) head[p->key % N] = p->next;
        else {
            while (q->next != p) q = q->next;
            q->next = p->next;
        }
        removeTime(p);
    }
    bool check (T &a) {
        //std::cerr << "opospsaoda\n";
        if (size < maxSize) return false;
        a = beg.next->value;
        remove(beg.next); delete(beg.next);
        --size;
        return true;
    }
    LinkedHashMap() {
        for (int i = 0; i < N; ++i) head[i] = nullptr;
    }
    ~LinkedHashMap() {
        node* p = &beg;
        while (p->timeNxt) {
            removeTime(p->timeNxt);
            delete p->timeNxt;
        }
    }
};

template<class Key, class T>
class BPlusTree {
private:
    using value = sjtu::pair<Key, T>; 
    #define NODE true
    #define LEAF false
    const static int M = 228;
    const static int maxSize = M;
    const static int minSize = M >> 1;
    class node {
    private:
        int sum = 0, place = 0, fa = 0, next = 0;
        bool type;
        value keys[M + 1];
        int ch[M + 1] = {};
        friend class BPlusTree;
    public:
        node() = default;
        node(const node &other):sum(other.sum), place(other.place), fa(other.fa), next(other.next), type(other.type) {
            for (int i = 0; i < other.sum; ++i) keys[i] = other.keys[i];
            for (int i = 0; i <= other.sum; ++i) ch[i] = other.ch[i];
        }
        node &operator =(const node &other) {
            if (&other == this) return *this;
            sum = other.sum; place = other.place; fa = other.fa; next = other.next;
            type = other.type;
            for (int i = 0; i < other.sum; ++i) keys[i] = other.keys[i];
            for (int i = 0; i <= other.sum; ++i) ch[i] = other.ch[i];
            return *this;
        }
    };
    class mystream: public fstream{
    public:
        mystream &operator<<(const node &a) {
            if(!a.place) return *this;
            seekp(a.place);
            write(reinterpret_cast<const char *>(&a), sizeof(a));
            return *this;
        }
        void readNode(const int &o, node &a) {
            seekg(o);
            read(reinterpret_cast<char *>(&a), sizeof(a));
        }
    };
    class cache {
    private:
        LinkedHashMap<node> m;
        mystream iofile;
        friend class BPlusTree;
    public:
        void init(const char FileName_[], int &sum, int &root) {
            iofile.open(FileName_, fstream::in);
            bool flag = iofile.is_open();
            iofile.close();
            if (!flag) {
                iofile.open(FileName_, fstream::out);
                iofile.close();
            }
            iofile.open(FileName_, fstream::in | fstream::out | fstream::binary);
            if (!flag) {
                root = 0;
                iofile.seekp(0);
                iofile.write(reinterpret_cast<const char *>(&root), sizeof(int));
                iofile.seekg(0);
            }
            else {
                iofile.seekg(0);
                iofile.read(reinterpret_cast<char *>(&root), sizeof(int));
                iofile.seekg(0, mystream::end);
                sum = ((int)iofile.tellg() - sizeof(int)) / sizeof(node);
            }
        }
        void getNode(int x, node &a) { 
            
            if (!m.find(x, a)) {
                iofile.readNode(x, a);
                m.insert(x, a);
                node b;
                if (m.check(b)) iofile << b;
            }
            
            //iofile.readNode(x, a);
        }
        void putNode(const node &a) {
            
            m.insert(a.place, a);
            node b;
            if (m.check(b)) iofile << b;
            
            //iofile << a;
        }
    };
    int root;
    cache ca;
    int sum = 0;
public:
    explicit BPlusTree(const char FileName_[]){
        ca.init(FileName_, sum, root);
    }
    ~BPlusTree() {
        ca.iofile.seekp(0);
        ca.iofile.write(reinterpret_cast<const char *>(&root), sizeof(int));
    }
    int newNode(node &a) {
        a.place = sum * sizeof(a) + sizeof(int);
        a.type = NODE;
        ++sum;
        return a.place;
    }
    int Search(const node &a, const value &val) {
        for (int i = 0; i < a.sum; ++i) if (val < a.keys[i]) return i;
        return a.sum;
    }
    void insert(node &a, const value &val) {
        int o = Search(a, val);
        for (int i = a.sum; i > o; --i) a.keys[i] = a.keys[i - 1];
        a.keys[o] = val;
        ++a.sum;
    }
    void insertChild(node &a, int o, int p) {
        for (int i = a.sum; i >= o; --i) a.ch[i + 1] = a.ch[i];
        a.ch[o] = p;  
    }
    void Split(node &a) {
        node b, c, d; 
        if (a.place == root) {
            root = newNode(b);
            a.fa = root;
            b.ch[0] = a.place;
            b.sum = 0;
        }
        else ca.getNode(a.fa, b);
        newNode(c);
        c.fa = a.fa; 
        c.next = a.next; a.next = c.place;
        c.type = a.type;
        c.sum = a.sum >> 1; a.sum -= c.sum;
        for (int i = 0; i < c.sum; ++i) c.keys[i] = a.keys[a.sum + i];
        if (a.type == NODE) {
            for (int i = 0; i <= c.sum; ++i) c.ch[i] = a.ch[a.sum + i], a.ch[a.sum + i] = 0;
            for (int i = 0; i <= c.sum; ++i) {
                if (!c.ch[i]) continue;
                ca.getNode(c.ch[i], d);
                d.fa = c.place;
                ca.putNode(d);
            }
        }
        int o = Search(b, c.keys[0]);
        b.ch[o] = c.place;
        if (a.type == NODE) insert(b, a.keys[a.sum - 1]), --a.sum;
        else insert(b, c.keys[0]);
        insertChild(b, o, a.place);
        ca.putNode(a); ca.putNode(b); ca.putNode(c);
        if (b.sum == maxSize) Split(b);
    }
    void Insert(const Key &key, const T &v) {
        //std::cerr << "root=" << root << '\n';
        value val = value(key, v);
        if (!root) {
            node a;
            root = newNode(a);
            a.place = root;
            a.type = LEAF;
            a.keys[0] = val;
            a.sum = 1;
            ca.putNode(a);
            return ;
        }
        int head = root, o = 0;
        node a, b;
        while (head) {
            //std::cerr << "head=" << head << '\n';
            ca.getNode(head, a);
            if (a.type == LEAF) {
                insert(a, val);
                if (a.sum == maxSize) Split(a);
                else ca.putNode(a);
                break;
            }
            else {
                int o = Search(a, val);
                head = a.ch[o];
            } 
        }
    }
    void Find(const Key &key, vector<T> &array) {
        array.clear();
        int head = root;
        bool o = false;
        node a;
        while(head) {
            ca.getNode(head, a);
            if (a.type == LEAF) {
                for (int i = 0; i < a.sum; ++i) {
                    if (a.keys[i].first == key) array.push_back(a.keys[i].second);
                }
                head = a.next;
            }
            else {
                int o = a.sum;
                for (int i = 0; i < a.sum; ++i) if (key <= a.keys[i].first) { o = i; break; }
                head = a.ch[o];
                head = a.ch[0];
            }
        }
        return ;
    }
};

#endif