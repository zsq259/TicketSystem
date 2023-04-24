/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

enum Color : bool { RED = false, BLACK = true };
struct nodebase {
	nodebase *ch[2] = {nullptr}, *fa = nullptr;
	bool c = RED;
	nodebase(nodebase *p = nullptr, bool c_ = RED) : fa(p),c(c_) { ch[0] = ch[1] = nullptr; }
	nodebase(const nodebase &rhs) = default;
	/*0:prev 1:next*/
	nodebase* getnode(int o) {
		if (!this->ch[o]) {
			nodebase* p = this;
			while (p) {
				if (p->fa && p == p->fa->ch[!o]) return p->fa;
				p = p->fa;
			}
			throw invalid_iterator();
		}
		else {
			nodebase* p = this->ch[o];
			while (p->ch[!o]) p = p->ch[!o];
			return p;
		}
		return nullptr;
	}
	void rotate(int op) {
		if (!ch[op]) return ;
		nodebase* o = ch[op];
		ch[op] = o->ch[!op]; 
		if (o->ch[!op]) o->ch[!op]->fa = this;
		o->ch[!op] = this;
		if (fa) fa->ch[side()] = o;
		o->fa = fa;
		fa = o;

	}
	void swap(nodebase *other) {
		if (other->fa == this) {
			int s0 = side(), s1 = other->side();
			fa->ch[s0] = other;
			other->fa = fa;
			nodebase *cc[2];
			cc[0] = ch[0], cc[1] = ch[1];
			ch[0] = other->ch[0];
			ch[1] = other->ch[1];
			if (other->ch[0]) other->ch[0]->fa = this;
			if (other->ch[1]) other->ch[1]->fa = this;
			fa = other;
			other->ch[s1] = this;
			other->ch[!s1] = cc[!s1];
			if (cc[!s1]) cc[!s1]->fa = other;
			std::swap(c, other->c);
			return ;
		}
				
		if (ch[0]) ch[0]->fa = other;
		if (ch[1]) ch[1]->fa = other;
		
		if (other->ch[0]) other->ch[0]->fa = this;
		if (other->ch[1]) other->ch[1]->fa = this;
		fa->ch[side()] = other;
		other->fa->ch[other->side()] = this;
		std::swap(fa, other->fa);
		std::swap(ch[0], other->ch[0]);
		std::swap(ch[1], other->ch[1]);
		std::swap(c, other->c);
	}
	int side() { return fa->ch[1] == this; }
};

enum my_type {my_true_type, my_false_type};

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type; 
	
	
	class node : public nodebase{
	public:
		value_type o;
		node() {}
		node(const value_type &o_, bool c_ = RED): o(o_), nodebase(nullptr, c_) {}
		node(const node &other):o(other.o), nodebase(other) {} 
		
	};
	nodebase* root = nullptr;
	size_t sz = 0;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	struct type_traits;
	class const_iterator;
	class iterator {
	private:
		/**
		
		 * TODO add data membech[1]
		 *   just add whatever you want.
		 */
		
		node* p = nullptr;
		const map* container = nullptr;
		friend class map;
		friend struct type_traits;
		//friend class const_iterator;
	public:
		const static my_type iterator_assignable = my_true_type;
		iterator() {
			// TODO
		}
		iterator(node* p_, const map* con_):p(p_), container(con_) {}
		iterator(const iterator &other):p(other.p), container(other.container) {
			// TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			if (!p) throw invalid_iterator();
			try {
				p = (node*)p->getnode(1);
			}
			catch(...){ throw; }
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if (!p) throw invalid_iterator();
			try {
				p = (node*)p->getnode(1);
			}
			catch(...) { throw; }
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = *this;
			if (!p) throw invalid_iterator();
			try {
				p = (node*)p->getnode(0);
			}
			catch(...){ throw; }
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if (!p) throw invalid_iterator();
			try {
				p = (node*)p->getnode(0);
			}
			catch(...) { throw; }
			return *this;
		}
		/**
		 * a operator to check whether two iteratoch[1] are same (pointing to the same memory).
		 */
		value_type & operator*() const { return p->o; }
		bool operator==(const iterator &rhs) const { 
			return p == rhs.p;
		}
		bool operator==(const const_iterator &rhs) const { 
			return p == rhs.p;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const { return !((*this) == rhs); }
		bool operator!=(const const_iterator &rhs) const { return !((*this) == rhs); }

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept { return &(p->o); }
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data membech[1].
			
			node* p = nullptr;
			const map* container = nullptr;
			friend class map;
			friend struct type_traits;
		public:
			const static my_type iterator_assignable = my_false_type;
			const_iterator() {
				// TODO
			}
			const_iterator(node* p_, const map* con_):p(p_), container(con_) {}
			const_iterator(const const_iterator &other):p(other.p), container(other.container) {
				// TODO
			}
			const_iterator(const iterator &other):p(other.p), container(other.container) {
				// TODO
			}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
			/**
		 	* TODO iter++
			*/
			const_iterator operator++(int) {
				const_iterator tmp = *this;
				if (!p) throw invalid_iterator();
				try {
					p = (node*)p->getnode(1);
				}
				catch(...){ throw; }
				return tmp;
			}
			/**
			* TODO ++iter
			*/
			const_iterator & operator++() {
				if (!p) throw invalid_iterator();
				try {
					p = (node*)p->getnode(1);
				}
				catch(...) { throw; }
				return *this;
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int) {
				const_iterator tmp = *this;
				if (!p) throw invalid_iterator();
				try {
					p = (node*)p->getnode(0);
				}
				catch(...){ throw; }
				return tmp;
			}
			/**
			* TODO --iter
			*/
			const_iterator & operator--() {
				if (!p) throw invalid_iterator();
				try {
					p = (node*)p->getnode(0);
				}
				catch(...) { throw; }
				return *this;
			}
			/**
			* a operator to check whether two iteratoch[1] are same (pointing to the same memory).
			*/
			value_type & operator*() const { return p->o; }
			bool operator==(const iterator &rhs) const { 
				return p == rhs.p;
			}
			bool operator==(const const_iterator &rhs) const { 
				return p == rhs.p;
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const { return !((*this) == rhs); }
			bool operator!=(const const_iterator &rhs) const { return !((*this) == rhs); }

			/**
			* for the support of it->first. 
			* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			*/
			value_type* operator->() const noexcept { return &(p->o); }
	};
	/**
	 * TODO two constructoch[1]
	 */
	map() { root = new nodebase(nullptr, BLACK); }

	void copynode(nodebase* &a, const nodebase* const &b) {
		a = new node(((node*)b)->o, b->c);
		if (b->ch[0]) copynode(a->ch[0], b->ch[0]), a->ch[0]->fa = a;
		if (b->ch[1]) copynode(a->ch[1], b->ch[1]), a->ch[1]->fa = a;
	}

	map(const map &other):sz(other.sz) {
		root = new nodebase(nullptr, BLACK);
		if (other.root->ch[0]) {
			copynode(root->ch[0], other.root->ch[0]);
			root->ch[0]->fa = root;
		}
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if (&other != this) {
			clear();
			sz = other.sz;
			if (other.root->ch[0]) {
				copynode(root->ch[0], other.root->ch[0]);
				root->ch[0]->fa = root;
			}
		}
		return *this;
	}
	/**
	 * TODO Destructoch[1]
	 */
	~map() { this->clear(); delete root; }
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	nodebase* findnode(nodebase* p, const Key &key) const {
		while (p) {
			bool o1 = Compare()(key, ((node*)p)->o.first), o2 = Compare()(((node*)p)->o.first, key);
			if (!(o1 || o2)) return p;
			if (p->ch[0] && o1) p = p->ch[0];
			else if (p->ch[1] && o2) p = p->ch[1];
			else throw index_out_of_bound();
		}
		throw index_out_of_bound();
	}
	
	T & at(const Key &key) {
		try {
			return ((node*)findnode(root->ch[0], key))->o.second;
		}
		catch(...) {throw ;}
	}
	const T & at(const Key &key) const {
		try {
			return ((node*)findnode(root->ch[0], key))->o.second;
		}
		catch(...) {throw ;}
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		return insert(value_type(key, T())).first->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		try {
			return ((node*)findnode(root->ch[0], key))->o.second;
		}
		catch(...) {throw ;}
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		nodebase* p = root;
		while (p->ch[0]) p = p->ch[0];
		return iterator((node*)p, this);
	}
	const_iterator cbegin() const {
		nodebase* p = root;
		while (p->ch[0]) p = p->ch[0];
		return const_iterator((node*)p, this);
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() { return iterator((node*)root, this); }
	const_iterator cend() const { return const_iterator((node*)root, this); }
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const { return !sz; }
	/**
	 * returns the number of elements.
	 */
	size_t size() const { return sz; }
	/**
	 * cleach[1] the contents
	 */
	
	void deletenode(nodebase* p) {
		if (!p) return ;
		deletenode(p->ch[0]);
		deletenode(p->ch[1]);
		p->ch[0] = p->ch[1] = nullptr;
		delete (node*)p;
	}

	void clear() {
		deletenode(root->ch[0]);
		sz = 0; root->ch[0] = nullptr;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
		
	void insertmaintain(nodebase *p) {		
		if (!p || p == root->ch[0] || p->fa->c == BLACK) return ;
		if (p->fa == root->ch[0]) { p->fa->c = BLACK; return ; }
		nodebase *q = p->fa, *g = p->fa->fa;
		if (g->ch[!q->side()] && g->ch[!q->side()]->c == RED) {
			q->c = BLACK;
			g->ch[!q->side()]->c = BLACK;
			g->c = RED;
			insertmaintain(g);
			return ;
		}
		int o1 = p->side();
		if (p->side() ^ q->side()) {
			q->rotate(p->side());
			p = p->ch[p->side()];
		}
		g->rotate(p->side());
		g = g->fa;
		g->c = BLACK; 
		if (g->ch[!p->side()]) g->ch[!p->side()]->c = RED;
	}

	pair<iterator, bool> insert(const value_type &value) {
		nodebase *p = root->ch[0], *q = root;
		bool op = false;
		while (p) {
			bool o1 = Compare()(((node*)p)->o.first, value.first), o2 = Compare()(value.first, ((node*)p)->o.first);
			if (!(o1 || o2)) return pair<iterator, bool>(iterator((node*)p, this), false);
			if (o2) q = p, p = p->ch[0], op = false;
			else q = p, p = p->ch[1], op = true;
		}
		p = new node(value, RED); 
		++sz;
		p->fa = q; 
		op? q->ch[1] = p:q->ch[0] = p;
		insertmaintain(p);
		return pair<iterator, bool>(iterator((node*)p, this), true);
	}

	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */

	void erasemaintain(nodebase *p) {
		if (p == root->ch[0]) return ;
		nodebase *q = p->fa->ch[!p->side()];
		if (q && q->c == RED) {
			p->fa->rotate(!p->side());
			p->fa->fa->c = BLACK;
			p->fa->c = RED;
			erasemaintain(p);
			return ;
		}
		else if (q) {
			if ((!q->ch[0] || q->ch[0]->c == BLACK) && (!q->ch[1] || q->ch[1]->c == BLACK)) {
				if (p->fa->c == RED) { p->fa->c = BLACK; q->c = RED; return ;}
				else { q->c = RED; erasemaintain(p->fa); return ; }
			}
			if (q->ch[p->side()] && q->ch[p->side()]->c == RED) {
				q->rotate(p->side());
				q->c = RED;
				q->fa->c = BLACK;
				q = p->fa->ch[!p->side()];
			}
			if (q->ch[!p->side()] && q->ch[!p->side()]->c == RED) {
				p->fa->rotate(!p->side());
				std::swap(p->fa->c, q->c);
				q->ch[!p->fa->side()]->c = BLACK;
			}
		}
	}

	void erase(nodebase *p) {
		if (p == root->ch[0] && sz == 1) { delete (node*)p; root->ch[0] = nullptr; return ; }
		if (p->ch[0] && p->ch[1]) {
			nodebase *q = p->getnode(0);
			p->swap(q);
		}
		if (!(p->ch[0] || p->ch[1])) {
			if (p->c == BLACK) erasemaintain(p);
			p->fa->ch[p->side()] = nullptr;
			delete (node*)p;
		}
		else {
			nodebase *q = p->ch[0]? p->ch[0]:p->ch[1];
			q->fa = p->fa;
			p->fa->ch[p->side()] = q;
			if (p->c == BLACK) {
				if (q->c == RED) q->c = BLACK;
				else erasemaintain(q);
			}
			delete (node*)p;
		}
	}

	void erase(iterator pos) {
		if (pos.container != this || pos.p == root) throw exception();
		nodebase *p = root->ch[0];
		bool op = false;
		while (p) {
			bool o1 = Compare()(((node*)p)->o.first, pos->first), o2 = Compare()(pos->first, ((node*)p)->o.first);
			if (!(o1 || o2)) { erase(p); --sz; return ; }
			else {
				if (o1) p = p->ch[1], op = true;
				else p = p->ch[0], op = false;
			}
		}
		throw exception();
	}
	/**
	 * Returns the number of elements with key 
	 *   that Compare()s equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		try{
			findnode(root->ch[0], key);
			return 1;
		}
		catch(...){ return 0; }
		
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		try{
			return iterator((node*)findnode(root->ch[0], key), this);
		}
		catch(...){ return this->end();}
	}
	const_iterator find(const Key &key) const {
		try{
			return iterator((node*)findnode(root->ch[0], key), this);
		}
		catch(...){ return this->cend();}
	}
	
};

template<class T>
struct type_traits {
	const static my_type type = T::iterator_assignable;
};

}

#endif
