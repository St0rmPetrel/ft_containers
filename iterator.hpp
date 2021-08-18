#ifndef ITERATOR_HPP
#define ITERATOR_HPP

namespace ft {
	// >>> iterator_category >>>
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag {};
	struct bidirectional_iterator_tag {};
	struct random_access_iterator_tag {};
	// <<< iterator_category <<<

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
			typedef typename random_access_iterator_tag iterator_category;
	}

	template <class T>
	class iterator_traits<const T*> {
		public:
			typedef typename ptrdiff_t difference_type;
			typedef typename T value_type;
			typedef typename T* pointer;
			typedef typename T& reference;
			typedef typename random_access_iterator_tag iterator_category;
	}

	// <<< iterator_traits <<<
} /* namespace ft */

#endif /* ITERATOR_HPP */
