#ifndef SET_HPP
#define SET_HPP

#include <functional>
#include <memory>
#include <iterator>
#include <limits>

#include "rb_tree.hpp"
#include "pair.hpp"
#include "utils.hpp"

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
			typedef typename underlying_tree_type::const_iterator         iterator;
			typedef typename underlying_tree_type::const_iterator         const_iterator;
			typedef typename underlying_tree_type::const_reverse_iterator reverse_iterator;
			typedef typename underlying_tree_type::const_reverse_iterator const_reverse_iterator;

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
			set (InputIterator first,
					typename ft::enable_if<
					!std::numeric_limits<InputIterator>::is_integer,
					InputIterator >::type last,
					const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type())
				: _comp(comp), _alloc(alloc), _size(0) {
				this->insert(first, last);
			}
			// copy
			set (const set& x) {
			}
			// Destructor
			~set() {}
		public:
			// iterators
			iterator begin() const {
				return (this->_base).begin();
			}
			iterator end() const {
				return (this->_base).end();
			}
			reverse_iterator rbegin() const {
				return (this->_base).rbegin();
			}
			reverse_iterator rend() const {
				return (this->_base).rend();
			}
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
			// Insert
			// single element
			pair<iterator,bool> insert (const value_type& val) {
				pair<iterator,bool> ret;
				iterator            it;

				it = this->_base.find(val);
				if (it != this->_base.end()) {
					return make_pair(it, false);
				}
				ret = make_pair(this->_base.insert_node(val), true);
				++(this->_size);
				return ret;
			}
			// with hint, just a Gab
			iterator insert (iterator position, const value_type& val) {
				return (this->insert(val)).first;
			}
			// range
			template <class InputIterator>
			void insert (InputIterator first,
					typename ft::enable_if<
					!std::numeric_limits<InputIterator>::is_integer,
					InputIterator >::type last) {
				for (InputIterator it = first; it != last; ++it) {
					this->insert(*it);
				}
			}
			// Erase
			size_type erase (const value_type& val) {
				if (_base.find(val) == _base.end()) {
					return 0;
				}
				_base.delete_node(val);
				return 1;
			}
			void erase (iterator position) {
				this->erase(*position);
			}
			void erase (iterator first, iterator last) {
				for (iterator it = first; it != last;) {
					this->erase(it++);
				}
			}

			// Operation
			iterator find (const value_type& val) const {
				return this->_base.find(val);
			}
	};
}; /* namespace ft */

#endif /* SET_HPP */
