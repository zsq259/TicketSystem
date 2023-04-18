#ifndef TICKET_SYSTEM_B_PLUS_TREE_H
#define TICKET_SYSTEM_B_PLUS_TREE_H

#define USE_CACHE
//#define PRINT_CNT
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "utility.hpp"
#include "vector.hpp"
#include "LinkedHashMap.hpp"
using std::fstream;
using std::istream;
using std::ostream;
using sjtu::vector;

namespace sjtu {
template<class Key, class T>
class BPlusTree {
private:
    using value = sjtu::pair<Key, T>; 
    #define NODE true
    #define LEAF false
    const static int M = 159;
    const static int maxSize = M;
    const static int minSize = M >> 1;
    class node {
    private:
        int sum = 0, place = 0, next = 0;
        bool type;
        value keys[M + 2];
        int ch[M + 2] = {};
        friend class BPlusTree;
    public:
        node() = default;
        node(const node &other):sum(other.sum), place(other.place), next(other.next), type(other.type) {
            for (int i = 0; i < other.sum; ++i) keys[i] = other.keys[i];
            for (int i = 0; i <= other.sum; ++i) ch[i] = other.ch[i];
        }
        node &operator =(const node &other) {
            if (&other == this) return *this;
            sum = other.sum; place = other.place; next = other.next;
            type = other.type;
            for (int i = 0; i < other.sum; ++i) keys[i] = other.keys[i];
            for (int i = 0; i <= other.sum; ++i) ch[i] = other.ch[i];
            return *this;
        }
        void printKeys() {
            for (int i = 0; i < sum; ++i) std::cerr << "keys[" << i << "]= " << keys[i] << '\n';
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
        template<class U>
        mystream &operator<<(const vector<U> &a) {
            seekp(0);
            write(reinterpret_cast<const char *>(&a), sizeof(a));
            return *this;
        }
        void readNode(const int &o, node &a) {
            seekg(o);
            read(reinterpret_cast<char *>(&a), sizeof(a));
        }
        template<class U>
        void readVector(vector<U> &a) {
            seekg(0);
            read(reinterpret_cast<char *>(&a), sizeof(a));
        }
    };
    class cache {
    private:
        LinkedHashMap<node> m;
        mystream iofile;
        mystream bin;
        friend class BPlusTree;
        int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
    public:
        explicit cache(const char FileName_[], const char BinName_[], int &sum, int &root, vector<int> a) {
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
            bin.open(BinName_, fstream::in);
            flag = bin.is_open();
            bin.close();
            if (!flag) {
                bin.open(BinName_, fstream::out);
                bin.close();
            }
            bin.open(BinName_, fstream::in | fstream::out | fstream::binary);
            if (flag) {
                bin.seekg(0);
                bin.read(reinterpret_cast<char *>(&a), sizeof(a));
            }
        }
        void getNode(int x, node &a) { 
            if (!m.find(x, a)) {
                iofile.readNode(x, a);
                m.insert(x, a);
                node b;
                if (m.check(b)) iofile << b;
            }
        }
        void putNode(const node &a) {
            m.insert(a.place, a);
            node b;
            if (m.check(b)) iofile << b, ++c2;
            else ++c4;
        }
        ~cache() {
            node b;
            while (m.timePop(b)) iofile << b;
        }
    };
    int root, sum = 0;
    cache ca;
    vector<int> space;
public:
    explicit BPlusTree(const char FileName_[], const char BinName_[]):ca(FileName_, BinName_, sum, root, space) {}
    ~BPlusTree() {
        ca.iofile.seekp(0);
        ca.iofile.write(reinterpret_cast<const char *>(&root), sizeof(int));
    }
    int newNode(node &a) {
        if (space.size()) a.place = space.back(), space.pop_back();
        else a.place = sum * sizeof(a) + sizeof(int), ++sum;
        a.type = NODE;
        return a.place;
    }
    int Search(const node &a, const value &val) {
        for (int i = 0; i < a.sum; ++i) if (val < a.keys[i]) return i;
        return a.sum;
        
        /*
        int L = 0, R = a.sum - 1, o = a.sum;
        while (L <= R) {
            int mid = (L + R) >> 1;
            if (val < a.keys[mid]) o = mid, R = mid - 1;
            else L = mid + 1;
        }
        return o;
        */
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
    void findFa(const node &a, node &b) {
        int head = root, o = 0;
        while (head) {
            ca.getNode(head, b);
            o = Search(b, a.keys[a.sum - 1]);
            head = b.ch[o];
            if (head == a.place) return ;
        }
    }
    void Split(node &a) {
        node b, c;
        if (a.place == root) {
            root = newNode(b);
            b.ch[0] = a.place;
            b.sum = 0;
        }
        else findFa(a, b);
        newNode(c);
        c.next = a.next; a.next = c.place;
        c.type = a.type;
        c.sum = a.sum >> 1; a.sum -= c.sum;
        for (int i = 0; i < c.sum; ++i) c.keys[i] = a.keys[a.sum + i];
        if (a.type == NODE) {
            for (int i = 0; i <= c.sum; ++i) c.ch[i] = a.ch[a.sum + i], a.ch[a.sum + i] = 0;
        }
        int o = Search(b, c.keys[0]);
        b.ch[o] = c.place;
        if (a.type == NODE) insert(b, a.keys[a.sum - 1]), --a.sum;
        else insert(b, c.keys[0]);
        insertChild(b, o, a.place);
        ca.putNode(a); ca.putNode(b); ca.putNode(c);
        if (b.sum > maxSize) Split(b);
    }
    void Insert(const Key &key, const T &v) {
        node a;
        value val = value(key, v);
        if (!root) {
            root = newNode(a);
            a.place = root;
            a.type = LEAF;
            a.keys[0] = val;
            a.sum = 1;
            ca.putNode(a);
            return ;
        }
        int head = root, o = 0;
        while (head) {
            ca.getNode(head, a);
            if (a.type == LEAF) {
                insert(a, val);
                if (a.sum > maxSize) Split(a);
                else ca.putNode(a);
                break;
            }
            else {
                o = Search(a, val);
                head = a.ch[o];
            } 
        }
    }
    void Delete(node &a, int o) {
        --a.sum;
        for (int i = o; i < a.sum; ++i) a.keys[i] = a.keys[i + 1];
    }
    void deleteChild(node &a, int o) {
        for (int i = o; i < a.sum; ++i) a.ch[i] = a.ch[i + 1];
        a.ch[a.sum] = 0;
    }
    void Merge(node &b, node &a, node &c) {
        if (a.sum + c.sum < maxSize) {
            int o = Search(b, a.keys[a.sum - 1]);
            if (a.type == NODE) a.keys[a.sum] = b.keys[o], ++a.sum;
            deleteChild(b, o + 1);
            Delete(b, o);
            for (int i = 0; i < c.sum; ++i) a.keys[a.sum + i] = c.keys[i];
            if (a.type == NODE) for (int i = 0; i <= c.sum; ++i) a.ch[a.sum + i] = c.ch[i];
            a.next = c.next; a.sum += c.sum;
            ca.putNode(a); ca.putNode(b);

            space.push_back(c.place);

            if (a.sum > maxSize) Split(a);
            return ;
        }
        int o = Search(b, a.keys[a.sum - 1]);
        int sp = a.sum;
        value keys[M << 1];
        int ch[M << 1];
        for (int i = 0; i < a.sum; ++i) keys[i] = a.keys[i];
        if (a.type == NODE) keys[a.sum] = b.keys[o], ++sp;
        for (int i = 0; i < c.sum; ++i) keys[sp + i] = c.keys[i];
        sp += c.sum;
        if (a.type == NODE) {
            for (int i = 0; i <= a.sum; ++i) ch[i] = a.ch[i];
            for (int i = 0; i <= c.sum; ++i) ch[a.sum + 1 + i] = c.ch[i];
        }
        Delete(b, o);
        int p = sp >> 1, st = p + (a.type == NODE);
        a.sum = p;
        c.sum = sp - st;
        for (int i = 0; i < a.sum; ++i) a.keys[i] = keys[i];
        for (int i = 0; i < c.sum; ++i) c.keys[i] = keys[st + i];
        insert(b, keys[p]);
        if (a.type == NODE) {
            for (int i = 0; i <= a.sum; ++i) a.ch[i] = ch[i];
            for (int i = 0; i <= c.sum; ++i) c.ch[i] = ch[a.sum + 1 + i];
        }
        ca.putNode(a); ca.putNode(b); ca.putNode(c);
    }
    void Merge(node &a) {
        node b, c;
        if (a.place == root) return ;
        findFa(a, b);
        if (b.place == root && b.sum == 1 && !b.ch[1]) { space.push_back(root); root = a.place; return ;}
        int o = Search(b, a.keys[a.sum - 1]), size = 0;
        int flag = -1;
        if (o) ca.getNode(b.ch[o - 1], c), size = c.sum, flag = 0;
        if (o < b.sum && b.ch[o + 1]) {
            ca.getNode(b.ch[o + 1], c); flag = 1;
            if (o && c.sum < size) ca.getNode(b.ch[o - 1], c), flag = 0;
        }
        if (flag == -1) { ca.putNode(a); return ; }
        else if (flag) Merge(b, a, c);
        else Merge(b, c, a);
        ca.getNode(b.place, b);
        if (b.sum < minSize) Merge(b);
    }
    void Delete(const Key &key, const T &v) {
        if (!root) return ;
        value val = value(key, v);
        node a;
        int head = root, o = 0;
        while (head) {
            ca.getNode(head, a);
            o = Search(a, val);
            if (a.type == LEAF) {
                if (!o || a.keys[o - 1] != val) return ;
                Delete(a, o - 1);
                ca.putNode(a);
                if (a.sum < minSize) Merge(a);
                break;
            }
            else {
                head = a.ch[o];
            }
        }
    }
    void Find(const Key &key, vector<T> &array) {
        node a;
        array.clear();
        int head = root;
        bool o = false;
        while(head) {
            ca.getNode(head, a);
            if (a.type == LEAF) {
                for (int i = 0; i < a.sum; ++i) {
                    if (a.keys[i].first == key) array.push_back(a.keys[i].second);
                    else if (a.keys[i].first > key) return ;
                }
                head = a.next;
            }
            else {
                int o = a.sum;
                for (int i = 0; i < a.sum; ++i) if (key <= a.keys[i].first) { o = i; break; }
                head = a.ch[o];
            }
        }
        return ;
    }
};
}

#endif