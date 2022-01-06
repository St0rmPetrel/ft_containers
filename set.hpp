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
			typedef typename underlying_tree_type::const_iterator         iterator;
			typedef typename underlying_tree_type::const_iterator         const_iterator;
			typedef typename underlying_tree_type::const_reverse_iterator reverse_iterator;
			typedef typename underlying_tree_type::const_reverse_iterator const_reverse_iterator;

		private:
			underlying_tree_type _base;
			size_type            _size;

			key_compare    _comp;
			allocator_type _alloc;
		public:
			// Constuctors
			// empty (default)
			explicit set (const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type())
				: _base(alloc, comp), _comp(comp), _alloc(alloc), _size(0) { }
			// range
			template < class InputIterator >
			set (InputIterator first,
					typename ft::enable_if<
					!std::numeric_limits<InputIterator>::is_integer,
					InputIterator >::type last,
					const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type())
				: _base(alloc, comp), _comp(comp), _alloc(alloc), _size(0) {
				this->insert(first, last);
			}
			// copy
			set (const set& x) {
				this->insert(x.begin(), x.end());
			}

			// Destructor
			~set() { }

			set& operator= (const set& x) {
				this->clear();
				this->insert(x.begin(), x.end());
				return (*this);
			}
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
			size_type max_size() const {
				return (this->_alloc.max_size());
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
				--(this->_size);
			}
			void erase (iterator first, iterator last) {
				for (iterator it = first; it != last;) {
					this->erase(it++);
				}
			}
			void swap (set& x) {
				(this->_base).swap(x._base);
				std::swap(this->_size, x._size);
				std::swap(this->_comp, x._comp);
				std::swap(this->_alloc, x._alloc);
			}
			void clear () {
				erase(this->begin(), this->end());
			}

			// Observers
			key_compare key_comp() const {
				return this->_comp;
			}
			value_compare value_comp() const {
				return this->_comp;
			}

			// Operation
			iterator find (const value_type& val) const {
				return this->_base.find(val);
			}
			size_type count (const value_type& val) const {
				if (this->_base.find(val) != this->end()) {
					return 1;
				}
				return 0;
			}
			iterator lower_bound (const value_type& val) const {
				return this->_base.lower_bound(val);
			}
			iterator upper_bound (const value_type& val) const {
				return this->_base.upper_bound(val);
			}
			pair<iterator,iterator> equal_range (const value_type& val) const {
				return make_pair(_base.lower_bound(val), _base.upper_bound(val));
			}

			// Allacator
			allocator_type get_allocator() const {
				return this->_alloc;
			}
	};
}; /* namespace ft */

#endif /* SET_HPP */
