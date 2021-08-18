#ifndef VECTOR_HPP
#define VECTOR_HPP

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

			class iterator :
				public std::iterator<std::random_access_iterator_tag,
					value_type> {
			};
			class const_iterator :
				public std::iterator<std::random_access_iterator_tag,
					const value_type> {
			};
			typedef reverse_iterator<iterator> reverse_iterator;
			typedef reverse_iterator<const_iterator> const_reverse_iterator;

			typedef iterator_traits<iterator>::difference_type difference_type;
			typedef size_t size_type;

		private:
			//bla bla
	}; /* class vector */
}; /* namespace ft */

#endif /* VECTOR_HPP */
