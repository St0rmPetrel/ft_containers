#ifndef RANDOM_ACCESS_ITERATOR_HPP
#define RANDOM_ACCESS_ITERATOR_HPP

#include "iterator.hpp"

namespace ft {
	template < typename ValueType >
	class random_access_iterator :
		public std::iterator<std::random_access_iterator_tag, ValueType> {
		public:
			typedef ValueType value_type;
			typedef ptrdiff_t difference_type;
			typedef ValueType* pointer;
			typedef ValueType& reference;
			typedef std::random_access_iterator_tag iterator_category;
		public:
			random_access_iterator() : _base(NULL) {}
			random_access_iterator(pointer ptr) : _base(ptr) {}
			random_access_iterator (const random_access_iterator& src)
				: _base(src.base()) {}
			random_access_iterator& operator= (
					const random_access_iterator& src) {
				if (this != &src)
					this->_base = src.base();
				return (*this);
			}
			~random_access_iterator() {}

			pointer base() const {
				return this->_base;
			}

			reference operator*() const {
				return *(this->_base);
			}
			random_access_iterator operator+ (difference_type n) const {
				return random_access_iterator(this->_base + n);
			}
			random_access_iterator& operator++() {
				++(this->_base);
				return (*this);
			} // pre-increment
			random_access_iterator operator++(int) { // post-increment
				random_access_iterator temp = *this;
				++(*this);
				return temp;
			}
			random_access_iterator& operator+= (difference_type n) {
				this->_base += n;
				return (*this);
			}

			random_access_iterator operator- (difference_type n) const {
				return random_access_iterator(this->base() - n);
			}
			random_access_iterator& operator--() {
				--(this->_base);
				return (*this);
			} // pre-decrement
			random_access_iterator operator--(int) { // post-decrement
				random_access_iterator temp = *this;
				--(*this);
				return temp;
			}
			random_access_iterator& operator-= (difference_type n) {
				this->_base -= n;
				return (*this);
			}

			pointer operator->() const {
				return &(operator*());
			}
			reference operator[] (difference_type n) const {
				return (*(this->base() + n));
			}
		protected:
			pointer _base;
	};
	template < typename ValueType >
	class const_random_access_iterator
		: public random_access_iterator<const ValueType> {
			public:
			typedef ValueType* pointer;
			typedef const ValueType const_ValueType;

			const_random_access_iterator()
				: random_access_iterator<const_ValueType>() {}
			const_random_access_iterator(
					const random_access_iterator<ValueType>& src)
				: random_access_iterator<const_ValueType>(src.base()) {}
			const_random_access_iterator<const_ValueType>(pointer ptr)
				: random_access_iterator<const_ValueType>(ptr) {}
			const_random_access_iterator (const const_random_access_iterator&
					src)
				: random_access_iterator<const_ValueType>(src.base()) {}
			const_random_access_iterator& operator= (
					const const_random_access_iterator& src) {
				if (this != &src)
					this->_base = src.base();
				return (*this);
			}
			~const_random_access_iterator() {}
	};

	// >>> Non-member function overloads for random_access_iterator >>>
		// >> relational operators (random_access_iterator) >>
	template < class ValueType >
		bool operator== (const random_access_iterator<ValueType>& lhs,
				const random_access_iterator<ValueType>& rhs) {
			return (lhs.base() == rhs.base());
		}
	template < class ValueType >
		bool operator!= (const random_access_iterator<ValueType>& lhs,
				const random_access_iterator<ValueType>& rhs) {
			return (lhs.base() != rhs.base());
		}
	template < class ValueType >
		bool operator< (const random_access_iterator<ValueType>& lhs,
				const random_access_iterator<ValueType>& rhs) {
			return (lhs.base() < rhs.base());
		}
	template < class ValueType >
		bool operator<= (const random_access_iterator<ValueType>& lhs,
				const random_access_iterator<ValueType>& rhs) {
			return (lhs.base() <= rhs.base());
		}
	template < class ValueType >
		bool operator> (const random_access_iterator<ValueType>& lhs,
				const random_access_iterator<ValueType>& rhs) {
			return (lhs.base() > rhs.base());
		}
	template < class ValueType >
		bool operator>= (const random_access_iterator<ValueType>& lhs,
				const random_access_iterator<ValueType>& rhs) {
			return (lhs.base() >= rhs.base());
		}
		// << relational operators (random_access_iterator) <<

	template < class ValueType >
		random_access_iterator<ValueType> operator+ (
				typename random_access_iterator<ValueType>::difference_type n,
				const random_access_iterator<ValueType>& rda_it) {
			return (rda_it + n);
		}// Addition operator
	 template <class ValueType >
		 typename random_access_iterator<ValueType>::difference_type operator- (
				 const random_access_iterator<ValueType>& lhs,
				 const random_access_iterator<ValueType>& rhs) {
			 return (lhs.base() - rhs.base());
		 } // Subtraction operator
	// <<< Non-member function overloads for reverse_iterator <<<
} /* namespace ft */

#endif /* RANDOM_ACCESS_ITERATOR_HPP */
