#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft {
	template< class T, class Container = ft::vector<T> >
	class stack {
	public:
		typedef T         value_type;
		typedef Container container_type;
		typedef size_t    size_type;
	protected: 
		container_type c;
	public:
		// Constructors
		stack() { }
		explicit stack(const container_type& container) : c(container) { }
		// Destructor
		~stack() { }

		bool empty() const {
			return c.empty();
		}
		size_type size() const {
			return c.size();
		}
		value_type& top() {
			return c.back();
		}
		const value_type& top() const {
			return c.back();
		}
		void push (const value_type& val) {
			return c.push_back(val);
		}
		void pop() {
			return c.pop_back();
		}

		bool operator== (const stack<T,Container>& rhs) const {
			return this->c == rhs.c;
		}
		bool operator!= (const stack<T,Container>& rhs) const {
			return !(this->c == rhs.c);
		}
		bool operator< (const stack<T,Container>& rhs) const {
			return this->c < rhs.c;
		}
		bool operator<= (const stack<T,Container>& rhs) const {
			return !(this->c <= rhs.c);
		}
		bool operator> (const stack<T,Container>& rhs) const {
			return this->c > rhs.c;
		}
		bool operator>= (const stack<T,Container>& rhs) const {
			return !(this->c >= rhs.c);
		}
	};

} /*namespace ft */

#endif /* SET_HPP */
