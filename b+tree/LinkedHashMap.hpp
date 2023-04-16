#ifndef TICKET_SYSTEM_LINKEDHASHMAP
#define TICKET_SYSTEM_LINKEDHASHMAP

template<class T>
class LinkedHashMap {
private:
    const static int N = 259;
    const static int maxSize = 124;
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
        if (p == cur) cur = p->timePre;
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
        node* temp = beg.timeNxt;
        a = temp->value;
        remove(temp); delete temp;
        --size;
        return true;
    }
    LinkedHashMap() {
        beg.timePre = beg.timeNxt = nullptr;
        for (int i = 0; i < N; ++i) head[i] = nullptr;
    }
    ~LinkedHashMap() {
        node *p = beg.timeNxt;
        node *temp;
        while (p) {
            temp = p;
            p = p->timeNxt;
            delete temp;
        }
    }
};

#endif