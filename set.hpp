#ifndef SET_HPP
#define SET_HPP

#include <functional>
#include <memory>

#include "rb_tree.hpp"

namespace ft {
	template <  class T,                          // set::key_type/value_type
				class Compare = std::less<T>,     // set::key_compare/value_compare
				class Alloc = std::allocator<T> > // set::allocator_type
	class set {
		public:
			typedef T                                        key_type;
			typedef T                                        value_type;
			typedef Compare                                  key_compare;
			typedef Compare                                  value_compare;
			typedef Allocator                                allocator_type;
			typedef typename allocator_type::reference       reference;
			typedef typename allocator_type::const_reference const_reference;
			typedef typename allocator_type::pointer         pointer;
			typedef typename allocator_type::const_pointer   const_pointer;
			typedef ptrdiff_t                                difference_type;
			typedef size_t                                   size_type;
		private:
			RBTree<key_type, value_type, key_compare, allocator_type> _base;
			size_type                                                 _size;

			key_compare    _comp;
			allocator_type _alloc;
		public:
			// Constuctors
			// empty
			explicit set (const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type())
				: _comp(comp), _alloc(alloc), _size(0) {
			}
			// range
			template < class InputIterator >
			set (InputIterator first, InputIterator last,
					const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type()) {
				// Make it after iterators will have done
			}
			// copy
			set (const set& x) {
			}
			// Destructor
			~set() {}
		public:
			// Capacity
			size_type size() const {
				return (this->_size);
			}
			bool empty() const {
				if (this->size() == 0) {
					return true;
				}
				return false;
			}
			// Modifiers
			// For make insert, need to do pair and iterator - cool
	};
}; /* namespace ft */

#endif /* SET_HPP */
