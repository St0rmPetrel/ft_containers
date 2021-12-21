#ifndef SET_HPP
#define SET_HPP

#include <functional>
#include <memory>
#include <iterator>

#include "rb_tree.hpp"
#include "pair.hpp"

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
			typedef Alloc                                    allocator_type;
			typedef typename allocator_type::reference       reference;
			typedef typename allocator_type::const_reference const_reference;
			typedef typename allocator_type::pointer         pointer;
			typedef typename allocator_type::const_pointer   const_pointer;
			typedef ptrdiff_t                                difference_type;
			typedef size_t                                   size_type;
		private:
			typedef RBTree<key_type, key_compare, allocator_type> underlying_tree_type;
		public:
			// Прекол в том что const_iterator и iterator по факту тоже самое
			// поэтому iterator можно за typedef от const_iterator
			//
			// В STL итератор реалезован внутри красно черного дерева
			// и есть только const итератор
			typedef typename underlying_tree_type::const_iterator iterator;
			typedef typename underlying_tree_type::const_iterator const_iterator;

			//};
		private:
			underlying_tree_type _base;
			size_type            _size;

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
			pair<iterator,bool> insert (const value_type& val) {
				pair<iterator,bool> ret;
				iterator            it;

				it = this->_base.find(val);
				if (it != this->_base.end()) {
					return make_pair(it, false);
				}
				return make_pair(this->_base.insert_node(val), true);
			}
	};
}; /* namespace ft */

#endif /* SET_HPP */
