#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <sstream>
#include <string>
#include <stdexcept>
#include <limits>
#include "reverse_iterator.hpp"
#include "random_access_iterator.hpp"

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
					if (this->_size > this->max_size()) {
						throw std::length_error("cannot create ft::vector "
								"larger than max_size()");
					}
					this->_base = this->_alloc.allocate(this->_size);
					this->_capacity = this->_size;
					for (size_type i = 0; i < n; ++i) {
						this->_alloc.construct(this->_base + i, val);
					}
			} // fill
			template <class InputIterator>
				vector (InputIterator first, InputIterator last,
						const allocator_type& alloc = allocator_type())
				: _alloc(alloc) {
					this->_size = std::distance<InputIterator>(first, last);
					if (this->_size > this->max_size()) {
						throw std::length_error("cannot create ft::vector "
								"larger than max_size()");
					}
					this->_base = this->_alloc.allocate(this->_size);
					this->_capacity = this->_size;
					InputIterator it;
					size_type i;
					for (it = first, i = 0; it != last; ++it, ++i) {
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
				this->resize(0);
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
				if (n > this->max_size()) {
					throw std::length_error("cannot create ft::vector "
							"larger than max_size()");
				}
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

			void _range_check(size_type n) {
				if (n >= this->size()) {
					std::stringstream s;
					s << "vector::_range_check: n (which is " << n << ")";
					s << " >= this->size() (which is " << this->size() << ")";
					throw std::out_of_range(s.str());
				}
				return ;
			}
	}; /* class vector */
}; /* namespace ft */

#endif /* VECTOR_HPP */
