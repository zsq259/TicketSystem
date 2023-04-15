#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include "utility.hpp"

#include <climits>
#include <cstddef>
#include <memory>
#include <iostream>
using sjtu::pair;

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	T* head;
	int sz, sum;
	std::allocator<T> alloc;
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T* it = nullptr;
		vector* container = nullptr;
		friend class vector;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator(T* _it, vector* _container):it(_it), container(_container) {}
		iterator operator+(const int &n) const 
		{
			//TODO
			return iterator(it + n, container);
		}
		iterator operator-(const int &n) const 
		{
			//TODO
			return iterator(it - n, container);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			//TODO
			if (container != rhs.container) throw sjtu::invalid_iterator();
			return it - rhs.it;
		}
		iterator& operator+=(const int &n) 
		{
			//TODO
			it += n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			//TODO
			it -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tem = (*this);
			++it;
			return tem;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++it;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tem = (*this);
			--it;
			return tem;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--it;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *it;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const { return it == rhs.it && container == rhs.container; }
		bool operator==(const const_iterator &rhs) const { return it == rhs.it && container == rhs.container; }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
		bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/
		T* it;
		vector* container;
		friend class vector;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator(T* _it, vector* _container):it(_it), container(_container) {}
		const_iterator operator+(const int &n) const 
		{
			//TODO
			return const_iterator(it + n, container);
		}
		const_iterator operator-(const int &n) const 
		{
			//TODO
			return const_iterator(it - n, container);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const 
		{
			//TODO
			if (container != rhs.container) throw sjtu::invalid_iterator();
			return it - rhs.it;
		}
		const_iterator& operator+=(const int &n) 
		{
			//TODO
			it += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) 
		{
			//TODO
			it -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator tem = (*this);
			++it;
			return tem;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			++it;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator tem = (*this);
			--it;
			return tem;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			--it;
			return *this;
		}
		/**
		 * TODO *it
		 */
		const T operator*() const{
			return *it;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const { return it == rhs.it && container == rhs.container; }
		bool operator==(const const_iterator &rhs) const { return it == rhs.it && container == rhs.container; }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
		bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector():sz(0), sum(0) {}
	vector(const vector &other):sz(other.sz), sum(other.sum) {
		if (sz) {
			head = alloc.allocate(sz);
			for (int i = 0; i < sum; ++i) new(head + i) T{other.head[i]};
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for (int i = 0; i < sum; ++i) (head + i)->~T();
		if (sz) alloc.deallocate(head, sz);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other){
		if (&other == this) return *this;
		for (int i = 0; i < sum; ++i) (head + i)->~T();
		if (sz) alloc.deallocate(head, sz);
		sz = other.sz;
		sum = other.sum;
		if (sz) {
			head = alloc.allocate(sz);
			for (int i = 0; i < sum; ++i) new(head + i) T{other.head[i]};
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= sum) throw index_out_of_bound();
		return head[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= sum) throw index_out_of_bound();
		return head[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= sum) throw index_out_of_bound();
		return head[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos >= sum) throw index_out_of_bound();
		return head[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (!sum) throw container_is_empty();
		return head[0]; 
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (!sum) throw container_is_empty();
		return head[sum - 1];
	}	
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(head, this);
	}
	const_iterator cbegin() const {
		return const_iterator(head, const_cast<vector*>(this));
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(head + sum, this);
	}
	const_iterator cend() const {
		return const_iterator(head + sum, const_cast<vector*>(this));
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return !sum;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return sum;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (int i = 0; i < sum; ++i) (head + i)->~T();
		if (sz) alloc.deallocate(head, sz);
		sum = sz = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	void expand() {
		if (!sz) {
			sz = 4;
			head = alloc.allocate(4);
			//new(head) T{value};
			return ;
		}
		T* tmp = alloc.allocate(sz << 1);
		for (int i = 0; i < sum; ++i) new(tmp + i) T{head[i]};
		for (int i = 0; i < sum; ++i) (head + i)->~T();
		alloc.deallocate(head, sz);
		sz <<= 1;
		head = tmp; tmp = nullptr; 
	}
	iterator insert(iterator pos, const T &value) {
		int o = pos.it - head;
		if (pos.container != this || o < 0 || o > sz) throw invalid_iterator();
		if (sum == sz) expand();
		new(head + sum) T{head[sum - 1]};
		for (int i = sum - 1; i >= o; --i) head[i + 1] = head[i];
		head[o] = value;
		++sum;
		return iterator(head + o, this);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind < 0 || ind > sz) throw index_out_of_bound();
		if (sum == sz) expand();
		new(head + sum) T{head[sum - 1]};
		for (int i = sum - 1; i >= ind; --i) head[i + 1] = head[i];
		head[ind] = value;
		return iterator(head + ind, this);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		int o = pos.it - head;
		if (pos.container != this || o < 0 || o >= sum) throw invalid_iterator();
		for (int i = o + 1; i < sum; ++i) head[i - 1] = head[i];
		--sum;
		(head + sum)->~T();
		return iterator(head + o, this);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind < 0 || ind >= sum) throw index_out_of_bound();
		for (int i = ind + 1; i < sum; ++i) head[i - 1] = head[i];
		--sum;
		(head + sum)->~T();
		return iterator(head + ind, this);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (sum == sz) expand();
		new(head + sum) T{value};
		++sum;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		--sum;
		(head + sum)->~T();
	}
};


}

#endif
