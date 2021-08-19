#ifndef VECTOR_HPP
#define VECTOR_HPP

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
			typedef reverse_iterator_adapter<const_iterator> const_reverse_iterator;
			//typedef reverse_iterator<const_iterator> const_reverse_iterator;

			typedef ptrdiff_t difference_type;
			typedef size_t size_type;

		private:
			size_type _size;
			size_type _capacity;
			size_type _max_size;
			pointer   _base;
			//bla bla
	}; /* class vector */
}; /* namespace ft */

#endif /* VECTOR_HPP */
