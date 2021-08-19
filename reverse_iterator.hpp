#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include "iterator.hpp"

namespace ft {
	// >>> reverse_iterator_adapter >>>
	template <class Iterator>
	class reverse_iterator_adapter :
		public std::iterator<typename Iterator::iterator_category, 
			typename Iterator::value_type> {
		public: //Member types
			typedef Iterator iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category
				iterator_category;
			typedef typename iterator_traits<Iterator>::value_type
				value_type;
			typedef typename iterator_traits<Iterator>::difference_type
				difference_type;
			typedef typename iterator_traits<Iterator>::pointer pointer;
			typedef typename iterator_traits<Iterator>::reference reference;
		public:
			//>> (constuctor) >>
			reverse_iterator_adapter() : _base() {} //default
			explicit reverse_iterator_adapter (iterator_type it) :
				_base(it) {} //init
			template <class Iter>
				reverse_iterator_adapter(const reverse_iterator_adapter<Iter>&
						rev_base) : _base(rev_base.base()) {} //copy
			//<< (constuctor) <<

			iterator_type base() const {
				return this->_base;
			}
			reference operator*() const {
				return *(this->_base);
			}

			reverse_iterator_adapter operator+ (difference_type n) const {
				return reverse_iterator_adapter(this->base() - n);
			}
			reverse_iterator_adapter& operator++() {
				--(this->_base);
				return (*this);
			} // pre-increment
			reverse_iterator_adapter operator++(int) { // post-increment
				reverse_iterator_adapter temp = *this;
				++(*this);
				return temp;
			}
			reverse_iterator_adapter& operator+= (difference_type n) {
				this->_base -= n;
				return (*this);
			}

			reverse_iterator_adapter operator- (difference_type n) const {
				return reverse_iterator_adapter(this->base() + n);
			}
			reverse_iterator_adapter& operator--() {
				++(this->_base);
				return (*this);
			} // pre-decrement
			reverse_iterator_adapter operator--(int) { // post-decrement
				reverse_iterator_adapter temp = *this;
				--(*this);
				return temp;
			}
			reverse_iterator_adapter& operator-= (difference_type n) {
				this->_base += n;
				return (*this);
			}

			pointer operator->() const {
				return &(operator*());
			}
			reference operator[] (difference_type n) const {
				return (this->base()[-n-1]);
			}
		private:
			iterator_type _base;
	}; // <<< class reverse_iterator_adapter <<<

	// >>> Non-member function overloads for reverse_iterator_adapter >>>
		// >> relational operators (reverse_iterator_adapter) >>
	template <class Iterator>
		bool operator== (const reverse_iterator_adapter<Iterator>& lhs,
				const reverse_iterator_adapter<Iterator>& rhs) {
			return (lhs.base() == rhs.base());
		}
	template <class Iterator>
		bool operator!= (const reverse_iterator_adapter<Iterator>& lhs,
				const reverse_iterator_adapter<Iterator>& rhs) {
			return (lhs.base() != rhs.base());
		}
	template <class Iterator>
		bool operator< (const reverse_iterator_adapter<Iterator>& lhs,
				const reverse_iterator_adapter<Iterator>& rhs) {
			return (lhs.base() > rhs.base());
		}
	template <class Iterator>
		bool operator<= (const reverse_iterator_adapter<Iterator>& lhs,
				const reverse_iterator_adapter<Iterator>& rhs) {
			return (lhs.base() >= rhs.base());
		}
	template <class Iterator>
		bool operator> (const reverse_iterator_adapter<Iterator>& lhs,
				const reverse_iterator_adapter<Iterator>& rhs) {
			return (lhs.base() < rhs.base());
		}
	template <class Iterator>
		bool operator>= (const reverse_iterator_adapter<Iterator>& lhs,
				const reverse_iterator_adapter<Iterator>& rhs) {
			return (lhs.base() <= rhs.base());
		}
		// << relational operators (reverse_iterator_adapter) <<
	template <class Iterator>
		reverse_iterator_adapter<Iterator> operator+ (
				typename reverse_iterator_adapter<Iterator>::difference_type n,
				const reverse_iterator_adapter<Iterator>& rev_it) {
			return (rev_it + n);
		}// Addition operator
	 template <class Iterator>
		 typename reverse_iterator_adapter<Iterator>::difference_type
		 operator- ( const reverse_iterator_adapter<Iterator>& lhs,
				 const reverse_iterator_adapter<Iterator>& rhs) {
			 return (lhs.base() - rhs.base());
		 } // Subtraction operator
	// <<< Non-member function overloads for reverse_iterator_adapter <<<
	// <<< reverse_iterator_adapter <<<
}

#endif /* REVERSE_ITERATOR_HPP */
