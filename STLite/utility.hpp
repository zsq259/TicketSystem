#ifndef SJTU_UTILITY_HPP
#define SJTU_UTILITY_HPP

#include <utility>
#include <iostream>
#include <cstring>
using std::string;

namespace sjtu {

template<class T1, class T2>
class pair {
public:
	T1 first;
	T2 second;
	constexpr pair() : first(), second() {}
	pair(const pair &other) = default;
	pair(pair &&other) = default;
	pair(const T1 &x, const T2 &y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(U1 &&x, U2 &&y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
	template<class U1, class U2>
	pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}
	pair &operator =(const pair &other) {
		if (&other == this) return *this;
		first = other.first; second = other.second;
		return *this;
	}
	bool operator <(const pair &other) const {
		return first != other.first? first < other.first : second < other.second;
	}
	bool operator ==(const pair &other) const { return first == other.first && second == other.second; }
	bool operator !=(const pair &other) const { return !((*this) == other); }
	bool operator <=(const pair &other) const { return (*this) < other || (*this) == other; }
};

template<class T1, class T2>
std::ostream &operator <<(std::ostream &os, const pair<T1, T2> &a) {
	os << a.first << ' ' << a.second;
	return os;
}

class my_string{
public:
    char key[33];
    my_string() {strcpy(key, ""); }
    my_string(const char key_[]) { strcpy(key, key_); }
	my_string(const string &s) { strcpy(key, s.data()); }
    my_string(const my_string &other) { strcpy(key, other.key); }
    my_string operator = (const my_string &other) { 
        if (&other == this) return *this; 
        strcpy(key, other.key); 
        return *this;    
    }
	my_string &operator + (const my_string &other) { strcat(key, other.key); return *this; }
	operator string() { return string(key); }
    bool operator <  (const my_string &other) const { return strcmp(key, other.key) <  0; }
    bool operator == (const my_string &other) const { return strcmp(key, other.key) == 0; }
    bool operator >  (const my_string &other) const { return strcmp(key, other.key) >  0; }
    bool operator != (const my_string &other) const { return strcmp(key, other.key) != 0; }
    bool operator <= (const my_string &other) const { return strcmp(key, other.key) <= 0; }
    bool operator >= (const my_string &other) const { return strcmp(key, other.key) >= 0; }
	friend std::ostream &operator<<(std::ostream &os, const my_string &a) { return os << a.key; }
};


}

#endif
