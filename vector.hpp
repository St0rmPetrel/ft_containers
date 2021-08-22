#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>
#include <limits>
#include "reverse_iterator.hpp"
#include "random_access_iterator.hpp"

namespace ft {
	template<bool Cond, class T = void> struct enable_if {};
	template<class T> struct enable_if<true, T> { typedef T type; };

//	template<typename> struct is_integral_base: std::false_type {};
//
//	template<> struct is_integral_base<bool>: std::true_type {};
//	template<> struct is_integral_base<int>: std::true_type {};
//	template<> struct is_integral_base<short>: std::true_type {};
} //ft::enable_if for differ fill and range constructor in vector

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
					if (this->_size > this->_alloc.max_size()) {
						throw std::length_error("cannot create ft::vector "
								"larger than max_size()");
					}
					this->_base = this->_alloc.allocate(this->_size);
					this->_capacity = this->_size;
					for (size_type i = 0; i < n; ++i) {
						this->_alloc.construct(this->_base + i, val);
					}
			} // fill
			template < class InputIterator,
					 class = typename ft::enable_if<
						 !std::is_integral<InputIterator>::value>::type> // !!!
				vector (InputIterator first, InputIterator last,
						const allocator_type& alloc = allocator_type())
				: _alloc(alloc) {
					this->_size = std::distance<InputIterator>(first, last);
					if (this->_size > this->_alloc.max_size()) {
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
			// <<< ITERATORS <<<

			// >>> CAPACITY >>>
			size_type max_size() const {
				return (this->_alloc.max_size());
			}
			size_type size() const {
				return (this->_size);
			}
			size_type capacity() const {
				return (this->_capacity);
			}
			// <<< CAPACITY <<<

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

	}; /* class vector */
}; /* namespace ft */

#endif /* VECTOR_HPP */
