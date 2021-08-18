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
			//typedef random_acces_iterator ??

		private:
			//bla bla
	} /* class vector */
} /* namespace ft */

#endif /* VECTOR_HPP */
