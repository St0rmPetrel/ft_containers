#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <sstream>
#include <string>
#include <stdexcept>
#include <limits>
#include <iterator>
#include "reverse_iterator.hpp"
#include "random_access_iterator.hpp"

#include "utils.hpp"

namespace ft {
	template <class Type, class Allocator = std::allocator<Type> >
	class vector {
		public:
			typedef Type value_type;
			typedef Allocator allocator_type;
			typedef typename allocator_type::reference reference;
			typedef typename allocator_type::const_reference const_reference;
			typedef typename allocator_type::pointer pointer;
			typedef typename allocator_type::const_pointer const_pointer;

			typedef random_access_iterator<value_type> iterator;
			typedef random_access_iterator<const value_type> const_iterator;
			typedef reverse_iterator_adapter<iterator> reverse_iterator;
			typedef reverse_iterator_adapter<const_iterator>
				const_reverse_iterator;

			typedef ptrdiff_t difference_type;
			typedef size_t size_type;
		public:
			// >>> constructor >>>
			explicit vector (const allocator_type& alloc = allocator_type())
				: _alloc(alloc), _size(0), _capacity(0), _base(NULL) {
			} // default
			explicit vector (size_type n, const value_type& val = value_type(),
					const allocator_type& alloc = allocator_type())
				: _alloc(alloc), _size(n) {
					this->_alloc_size_check(this->_size);
					this->_base = this->_alloc.allocate(this->_size);
					this->_capacity = this->_size;
					for (size_type i = 0; i < n; ++i) {
						this->_alloc.construct(this->_base + i, val);
					}
			} // fill
		//	template < class InputIterator, typename ft::enable_if<
		//				 !ft::is_integral<InputIterator>::value>::type>
		//		vector (InputIterator first, InputIterator last,
		//				const allocator_type& alloc = allocator_type())
			template <class InputIterator>
				vector (InputIterator first,
						typename ft::enable_if< 
						!std::numeric_limits<InputIterator>::is_integer,
						InputIterator >::type last,
						const allocator_type& alloc = allocator_type())
				: _alloc(alloc) {
					this->_size = std::distance<InputIterator>(first, last);
					this->_alloc_size_check(this->_size);
					this->_base = this->_alloc.allocate(this->_size);
					this->_capacity = this->_size;
					InputIterator it;
					size_type i;
					for (it = first, i = 0; it != last; ++it, ++i) {
						//this->_alloc.construct(this->_base + i, (*it)());
						this->_alloc.construct(this->_base + i, *it);
					}
				} // range
			vector (const vector& x)
				: _alloc(allocator_type(x.get_allocator())),
				_size(x.size()), _capacity(x.capacity()) {
					this->_base = this->_alloc.allocate(this->_capacity);
					vector::const_iterator it;
					size_type i;
					for (it = x.begin(), i = 0; it != x.end(); ++it, ++i) {
						this->_alloc.construct(this->_base + i, *it);
					}
			} // copy
			// <<< constructor <<<

			// >>> destructor >>>
			~vector() {
				for (size_type i = 0; i < this->size(); ++i) {
					this->_alloc.destroy(this->_base + i);
				}
				this->_alloc.deallocate(this->_base, this->capacity());
				this->_base = NULL;
			}
			// <<< destructor <<<

			vector& operator= (const vector& x) {
				this->clear();
				this->reserve(x.size());
				vector::const_iterator it;
				size_type i;
				for (it = x.begin(), i = 0; it != x.end(); ++it, ++i) {
					this->_alloc.construct(this->_base + i, *it);
				}
				this->_size = x.size();
				return (*this);
			}


			// >>> ITERATORS >>>
			iterator begin() {
				return (vector::iterator(this->_base));
			}
			const_iterator begin() const {
				return (vector::const_iterator(this->_base));
			}
			iterator end() {
				return (vector::iterator(this->_base + this->size()));
			}
			const_iterator end() const {
				return (vector::const_iterator(this->_base + this->size()));
			}

			reverse_iterator rbegin() {
				return (reverse_iterator(this->end() - 1));
			}
			const_reverse_iterator rbegin() const {
				return (const_reverse_iterator(this->end() - 1));
			}
			reverse_iterator rend() {
				return (reverse_iterator(this->begin() - 1));
			}
			const_reverse_iterator rend() const {
				return (const_reverse_iterator(this->begin() - 1));
			}
			// <<< ITERATORS <<<

			// >>> CAPACITY >>>
			size_type size() const {
				return (this->_size);
			}
			size_type max_size() const {
				return (this->_alloc.max_size());
			}
			void resize (size_type n, value_type val = value_type()) {
				if (n > this->capacity()) {
					this->reserve(n);
				}
				for (size_type i = this->size(); i < n; ++i) {
					this->_alloc.construct(this->_base + i, val);
				}
				for (size_type i = n; i < this->size(); ++i) {
					this->_alloc.destroy(this->_base + i);
				}
				this->_size = n;
				return ;
			}
			size_type capacity() const {
				return (this->_capacity);
			}
			bool empty() const {
				return (this->size() == 0);
			}
			void reserve (size_type n) {
				if (n <= this->_capacity) {
					return ;
				}
				this->_alloc_size_check(n);
				pointer new_base = this->_alloc.allocate(n);
				for (size_type i = 0; i < this->size(); ++i) {
					this->_alloc.construct(new_base + i, *(this->_base + i));
					this->_alloc.destroy(this->_base + i);
				}
				this->_alloc.deallocate(this->_base, this->capacity());
				this->_base = new_base;
				this->_capacity = n;
				return ;
			}
			// <<< CAPACITY <<<

			// >>> ELEMEMNT ACCESS >>>
			reference operator[] (size_type n) {
				return (*(this->_base + n));
			}
			const_reference operator[] (size_type n) const {
				return (*(this->_base + n));
			}
			reference at (size_type n) {
				this->_range_check(n);
				return (*(this->_base + n));
			}
			const_reference at (size_type n) const {
				this->_range_check(n);
				return (*(this->_base + n));
			}
			reference front() {
				return (*(this->begin()));
			}
			const_reference front() const {
				return (*(this->begin()));
			}
			reference back() {
				return (*(this->end() - 1));
			}
			const_reference back() const {
				return (*(this->end() - 1));
			}
			// <<< ELEMEMNT ACCESS <<<

			// >>> MODIFIERS >>>
			template <class InputIterator>
				void assign (InputIterator first, 
					typename ft::enable_if<
					!std::numeric_limits<InputIterator>::is_integer,
					InputIterator >::type last) {
					this->clear();
					this->reserve(std::distance<InputIterator>(first, last));
					InputIterator it;
					size_type i;
					for (it = first, i = 0; it != last; ++it, ++i) {
						this->_alloc.construct(this->_base + i, *it);
					}
				} // range
			void assign (size_type n, const value_type& val) {
				this->clear();
				this->reserve(n);
				for (size_type i = 0; i < n; ++i) {
					this->_alloc.construct(this->_base + i, val);
				}
			} // fill
			void push_back (const value_type& val) {
				this->_tail_extension();
				this->_alloc.construct(&(*this->end()), val);
				++(this->_size);
			}
			void pop_back() {
				this->_alloc.destroy(&(*this->rbegin()));
				--(this->_size);
			}
				// > insert >
			iterator insert (iterator position, const value_type& val) {
				iterator new_position = this->_right_shift_extension(position, 1);
				this->_alloc.construct(&(*new_position), val);
				return (new_position);
			} // single element
			void insert (iterator position, size_type n,
					const value_type& val) {
				iterator new_position = this->_right_shift_extension(position, n);
				for (size_type i = 0; i < n; ++i) {
					this->_alloc.construct(&(*new_position) + i, val);
				}
			} // fill
			template <class InputIterator>
				void insert (iterator position, InputIterator first,
					typename ft::enable_if<
					!std::numeric_limits<InputIterator>::is_integer,
					InputIterator >::type last) {
					iterator new_position = this->_right_shift_extension(position,
							std::distance<InputIterator>(first, last));
					for (InputIterator it = first; it != last; ++it) {
						this->_alloc.construct(&(*new_position) +
								std::distance(first, it), *it);
					}
				} // range
				// < insert <
			iterator erase (iterator position) {
				return (this->_left_shift_erase(position, position + 1));
			}
			iterator erase (iterator first, iterator last) {
				return (this->_left_shift_erase(first, last));
			}
			void swap (vector& x) {
				std::swap(this->_alloc, x._alloc);
				std::swap(this->_size, x._size);
				std::swap(this->_capacity, x._capacity);
				std::swap(this->_base, x._base);
			}
			void clear() {
				this->resize(0);
			}
			// <<< MODIFIERS <<<

			// >>> ALLOCATOR >>>
			allocator_type get_allocator() const {
				return (this->_alloc);
			}
			// <<< ALLOCATOR <<<

		private:
			allocator_type _alloc;
			size_type _size;
			size_type _capacity;
			pointer   _base;

			void _range_check(size_type n) const {
				if (n >= this->size()) {
					std::stringstream s;
					s << "vector::_range_check: n (which is " << n << ")";
					s << " >= this->size() (which is " << this->size() << ")";
					throw std::out_of_range(s.str());
				}
				return ;
			}

			void _alloc_size_check(size_type n) const {
				if (n > this->max_size()) {
					throw std::length_error("cannot create ft::vector "
							"larger than max_size()");
				}
				return ;
			}

			void _tail_extension() {
				if (this->size() < this->capacity()) {
					return ;
				}
				if (!this->size()) {
					this->reserve(1);
					return ;
				}
				this->reserve(2 * (this->size()));
				return ;
			}

			iterator _right_shift_extension(iterator position, size_type n) {
				size_type new_size = this->size() + n;
				if ((!n) || ((position == this->end()) &&
						new_size <= this->capacity())) {
					return position;
				}
				if (new_size <= this->capacity()) {
					for (reverse_iterator rit = this->rbegin();
							rit != reverse_iterator(position - 1); ++rit) {
						this->_alloc.construct(&(*rit) + n, *rit);
						this->_alloc.destroy(&(*rit));
					} // shift right part on 'n' elements
					this->_size = new_size;
					return position;
				}
				this->_alloc_size_check(new_size);
				pointer new_base = this->_alloc.allocate(new_size);
				for (iterator it = this->begin(); it < position; ++it) {
					this->_alloc.construct(new_base + (it - this->begin())
								, *(it));
					this->_alloc.destroy(&(*it));
				} //copy left part in new_base
				for (iterator it = position; it < this->end(); ++it) {
					this->_alloc.construct(new_base + n +
							(it - this->begin()), *it);
					this->_alloc.destroy(&(*it));
				} // copy right part in new_base
				this->_alloc.deallocate(this->_base, this->capacity());
				iterator new_position = new_base + (position - this->begin());
				this->_base = new_base;
				this->_capacity = new_size;
				this->_size = new_size;
				return (new_position);
			}

			iterator _left_shift_erase(iterator first, iterator last) {
				for (iterator it = first; it < last; ++it) {
					this->_alloc.destroy(&(*it));
				}
				for (iterator it = last; it < this->end(); ++it) {
					this->_alloc.construct(&(*(first + (it - last))), *it);
					this->_alloc.destroy(&(*it));
				}
				this->_size -= last - first;
				return (first + 1);
			}
	}; /* class vector */
	/* relation operators for vector */
	template <class T, class Alloc>
		bool operator== (const vector<T,Alloc>& lhs,
				const vector<T,Alloc>& rhs) {
			if (lhs.size() != rhs.size()) {
				return (false);
			}
			return true;
		}
	template <class T, class Alloc>
		bool operator!= (const vector<T,Alloc>& lhs,
				const vector<T,Alloc>& rhs) {
			return (!(lhs == rhs));
		}
	template <class T, class Alloc>
		bool operator< (const vector<T,Alloc>& lhs,
				const vector<T,Alloc>& rhs) {
			return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
						rhs.begin(), rhs.end()));
		}
	template <class T, class Alloc>
		bool operator> (const vector<T,Alloc>& lhs,
				const vector<T,Alloc>& rhs) {
			return (!(ft::lexicographical_compare(lhs.begin(), lhs.end(),
						rhs.begin(), rhs.end())));
		}
	template <class T, class Alloc>
		bool operator<= (const vector<T,Alloc>& lhs,
				const vector<T,Alloc>& rhs) {
			return (!(lhs > rhs));
		}
	template <class T, class Alloc>
		bool operator>= (const vector<T,Alloc>& lhs,
				const vector<T,Alloc>& rhs) {
			return (!(lhs < rhs));
		}
	/* relation operators for vector */
	template <class T, class Alloc>
		void swap (vector<T,Alloc>& x, vector<T,Alloc>& y) {
			x.swap(y);
		}
}; /* namespace ft */

#endif /* VECTOR_HPP */
