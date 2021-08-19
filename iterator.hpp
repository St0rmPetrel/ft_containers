#ifndef ITERATOR_HPP
#define ITERATOR_HPP

namespace ft {
	// >>> iterator_traits >>>
	template <class Iterator>
	class iterator_traits {
		public:
			typedef typename Iterator::difference_type difference_type;
			typedef typename Iterator::value_type value_type;
			typedef typename Iterator::pointer pointer;
			typedef typename Iterator::reference reference;
			typedef typename Iterator::iterator_category iterator_category;
	}

	template <class T>
	class iterator_traits<T*> {
		public:
			typedef typename ptrdiff_t difference_type;
			typedef typename T value_type;
			typedef typename T* pointer;
			typedef typename T& reference;
			typedef typename std::random_access_iterator_tag iterator_category;
	}

	template <class T>
	class iterator_traits<const T*> {
		public:
			typedef typename ptrdiff_t difference_type;
			typedef typename T value_type;
			typedef typename T* pointer;
			typedef typename T& reference;
			typedef typename std::random_access_iterator_tag iterator_category;
	}
	// <<< iterator_traits <<<


	// >>> reverse_iterator >>>
	template <class Iterator>
	class reverse_iterator {
		public: //Member types
			typedef Iterator iterator_type;
			typedef iterator_traits<Iterator>::iterator_category iterator_category;
			typedef iterator_traits<Iterator>::value_type value_type;
			typedef iterator_traits<Iterator>::difference_type difference_type;
			typedef iterator_traits<Iterator>::pointer pointer;
			typedef iterator_traits<Iterator>::reference reference;
		public:
			//>> (constuctor) >>
			reverse_baseerator() : _base() {} //default
			explicit reverse_baseerator (iterator_type it) :
				_base(it) {} //init
			template <class Iter>
				reverse_baseerator(const reverse_baseerator<Iter>& rev_base) :
				   _base(rev_base.base()) {} //copy
			//<< (constuctor) <<

			iterator_type base() const {
				return this->_base;
			}
			reference operator*() const {
				return *(this->_base);
			}

			reverse_baseerator operator+ (difference_type n) const {
				this->_base = this->base() - n;
				return (*this);
			}
			reverse_baseerator& operator++() {
				--(this->_base);
				return (*this);
			} // pre-increment
			reverse_baseerator operator++(int) { // post-increment
				reverse_baseerator temp = *this;
				++(*this);
				return temp;
			}
			reverse_baseerator& operator+= (difference_type n) {
				this->_base -= n;
				return (*this);
			}

			reverse_baseerator operator- (difference_type n) const {
				this->_base = this->base() + n;
				return (*this);
			}
			reverse_baseerator& operator--() {
				++(this->_base);
				return (*this);
			} // pre-decrement
			reverse_baseerator operator--(int) { // post-decrement
				reverse_baseerator temp = *this;
				--(*this);
				return temp;
			}
			reverse_baseerator& operator-= (difference_type n) {
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
	} // <<< class reverse_iterator <<<

	// >>> Non-member function overloads for reverse_iterator >>>
		// >> relational operators (reverse_iterator) >>
	template <class Iterator>
		bool operator== (const reverse_iterator<Iterator>& lhs,
				const reverse_iterator<Iterator>& rhs) {
			return (lhs.base() == rhs.base());
		}
	template <class Iterator>
		bool operator!= (const reverse_iterator<Iterator>& lhs,
				const reverse_iterator<Iterator>& rhs) {
			return (lhs.base() != rhs.base());
		}
	template <class Iterator>
		bool operator< (const reverse_iterator<Iterator>& lhs,
				const reverse_iterator<Iterator>& rhs) {
			return (lhs.base() > rhs.base());
		}
	template <class Iterator>
		bool operator<= (const reverse_iterator<Iterator>& lhs,
				const reverse_iterator<Iterator>& rhs) {
			return (lhs.base() >= rhs.base());
		}
	template <class Iterator>
		bool operator> (const reverse_iterator<Iterator>& lhs,
				const reverse_iterator<Iterator>& rhs) {
			return (lhs.base() < rhs.base());
		}
	template <class Iterator>
		bool operator>= (const reverse_iterator<Iterator>& lhs,
				const reverse_iterator<Iterator>& rhs) {
			return (lhs.base() <= rhs.base());
		}
		// << relational operators (reverse_iterator) <<
	template <class Iterator>
		reverse_iterator<Iterator> operator+ (
				typename reverse_iterator<Iterator>::difference_type n,
				const reverse_iterator<Iterator>& rev_it) {
			return (rev_it + n);
		}// Addition operator
	 template <class Iterator>
		 typename reverse_iterator<Iterator>::difference_type operator- (
				 const reverse_iterator<Iterator>& lhs,
				 const reverse_iterator<Iterator>& rhs) {
			 return (lhs.base() - rhs.base());
		 } // Subtraction operator
	// <<< Non-member function overloads for reverse_iterator <<<
} /* namespace ft */

#endif /* ITERATOR_HPP */
