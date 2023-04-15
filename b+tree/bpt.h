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
        void readNode(int x, node &a) { iofile.readNode(x, a); }
        void writeNode(const node &a) {
            iofile << a;
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
            //ca.writeNode(a);
        }
        else ca.readNode(a.fa, b);
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
                ca.readNode(c.ch[i], d);
                d.fa = c.place;
                ca.writeNode(d);
            }
        }
        int o = Search(b, c.keys[0]);
        b.ch[o] = c.place;
        if (a.type == NODE) insert(b, a.keys[a.sum - 1]), --a.sum;
        else insert(b, c.keys[0]);
        insertChild(b, o, a.place);
        ca.writeNode(a); ca.writeNode(b); ca.writeNode(c);
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
            ca.writeNode(a);
            return ;
        }
        int head = root, o = 0;
        node a, b;
        while (head) {
            //std::cerr << "head=" << head << '\n';
            ca.readNode(head, a);
            if (a.type == LEAF) {
                insert(a, val);
                if (a.sum == maxSize) Split(a);
                else ca.writeNode(a);
                break;
            }
            else {
                o = Search(a, val);
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
            ca.readNode(head, a);
            if (a.type == LEAF) {
                for (int i = 0; i < a.sum; ++i) {
                    if (a.keys[i].first == key) array.push_back(a.keys[i].second);
                }
                head = a.next;
            }
            else head = a.ch[0];
        }
        return ;
    }
};

#endif