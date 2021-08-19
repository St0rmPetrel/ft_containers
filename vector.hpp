#ifndef VECTOR_HPP
#define VECTOR_HPP

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
					if (n > this->_alloc.max_size()) {
						throw std::length_error("cannot create ft::vector "
								"larger than max_size()");
					}
					this->_base = this->_alloc.allocate(n);
					for (size_type i = 0; i < n; ++i) {
						_alloc.construct(this->_base + i, val);
					}
			} // fill
			template <class InputIterator>
				vector (InputIterator first, InputIterator last,
						const allocator_type& alloc = allocator_type()) {
				} // range
			vector (const vector& x) {
			} // copy
			// <<< constructor <<<

			// >>> CAPACITY >>>
			size_type max_size() const {
				return (this->_alloc.max_size());
			}
			// <<< CAPACITY <<<

		private:
			allocator_type _alloc;
			size_type _size;
			size_type _capacity;
			pointer   _base;

	}; /* class vector */
}; /* namespace ft */

#endif /* VECTOR_HPP */
