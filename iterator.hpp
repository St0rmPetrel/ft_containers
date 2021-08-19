#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cstddef>
#include <iterator>

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
	};

	template <class T>
	class iterator_traits<T*> {
		public:
			typedef std::ptrdiff_t difference_type;
			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef typename std::random_access_iterator_tag iterator_category;
	};

	template <class T>
	class iterator_traits<const T*> {
		public:
			typedef std::ptrdiff_t difference_type;
			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef typename std::random_access_iterator_tag iterator_category;
	};
	// <<< iterator_traits <<<
} /* namespace ft */

#endif /* ITERATOR_HPP */
