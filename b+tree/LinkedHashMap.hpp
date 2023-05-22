#ifndef TICKET_SYSTEM_LINKEDHASHMAP
#define TICKET_SYSTEM_LINKEDHASHMAP

namespace sjtu {
template<class T>
class LinkedHashMap {
private:
    const static int N = 10000;
    const static int maxSize = 64;
    struct Node {
        Node* next = nullptr;
        Node* timePre = nullptr, *timeNxt = nullptr;
        int key;
        T value;
        Node() = default;
        Node(int key_, const T &value_):key(key_), value(value_) {}
        Node &operator =(const Node &other) {
            if (&other == this) return *this;
            next = other.next;
            timePre = other.timePre; timeNxt = other.timeNxt;
            key = other.key;
            value = other.value;
        }
    };
    int sz = 0, cnt = 0;
    Node beg;
    Node* head[N + 1], *cur = &beg;
    friend class cache;
    
public:
    inline void insertTime(Node* p) {
        cur->timeNxt = p;
        p->timePre = cur;
        cur = p;
    }
    inline void removeTime(Node* p) {
        if (p == cur) cur = p->timePre;
        if (p->timePre) p->timePre->timeNxt = p->timeNxt;
        if (p->timeNxt) p->timeNxt->timePre = p->timePre;
        p->timePre = p->timeNxt = nullptr;
    }
    inline void updateTime(Node* p) {
        removeTime(p); insertTime(p);
    }
    bool find(int key, T &a) {
        int o = key % N;
        Node* p = head[o];
        //std::cerr << "key= " << key << '\n';
        while (p) {
            if (p->key == key) { a = p->value; updateTime(p); return true; }
            p = p->next;
        }
        return false;
    }
    void insert(int key, const T &a) {
        //std::cerr << "begin insert\n";
        #ifdef PRINT_CNT
        cnt = std::max(cnt, key);
        #endif
        int o = key % N;
        Node *p = head[o];
        while (p) {
            if (p->key == key) { p->value = a; updateTime(p); return ; }
            p = p->next;
        }
        p = new Node(key, a);
        p->next = head[o];
        head[o] = p;
        insertTime(p); ++sz;
        //std::cerr << "end insert\n";
    }
    void remove(Node* p) {
        Node* q = head[p->key % N];
        if (q == p) head[p->key % N] = p->next;
        else {
            while (q->next != p) q = q->next;
            q->next = p->next;
        }
        removeTime(p);
    }
    bool check (T &a) {
        //std::cerr << "opospsaoda\n";
        if (sz < maxSize) return false;
        Node* temp = beg.timeNxt;
        a = temp->value;
        remove(temp); delete temp;
        --sz;
        return true;
    }
    int size() { return sz; }
    bool timePop(T &a) {
        if (!sz) return false;
        Node *temp = beg.timeNxt;
        a = temp->value;
        beg.timeNxt = temp->timeNxt;
        delete temp;
        --sz;
        return true;
    }
    LinkedHashMap() {
        beg.timePre = beg.timeNxt = nullptr;
        for (int i = 0; i < N; ++i) head[i] = nullptr;
    }
    ~LinkedHashMap() {
        #ifdef PRINT_CNT
        std::cerr << "cnt=" << cnt << '\n';
        #endif
        Node *p = beg.timeNxt;
        Node *temp;
        while (p) {
            temp = p;
            p = p->timeNxt;
            delete temp;
        }
    }
};
}

#endif