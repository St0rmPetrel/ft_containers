#ifndef MAP_HPP
# define MAP_HPP

#include <functional>
#include <memory>
#include <iterator>
#include <limits>

#include "rb_tree.hpp"
#include "pair.hpp"
#include "utils.hpp"

namespace ft {
	template <  class Key,                                         // map::key_type
			 	class T,                                           // map::mapped_type
				class Compare = std::less<Key>,                    // map::key_compare
				class Alloc = std::allocator<pair<const Key,T> > > // map::allocator_type
	class map {
		public:
			typedef Key                                      key_type;
			typedef T                                        mapped_type;
			typedef pair<const key_type,mapped_type>         value_type;
			typedef Compare                                  key_compare;
		public:
			class value_compare {
				protected:
					friend class map;

					Compare comp;
					// constructed with map's comparison object
					value_compare (Compare c) : comp(c) { }
				public:
					typedef bool       result_type;
					typedef value_type first_argument_type;
					typedef value_type second_argument_type;
 
					bool operator() (const value_type& x, const value_type& y) const {
						return comp(x.first, y.first);
					}
			}; /* value_compare */
		public:
			typedef Alloc                                    allocator_type;
			typedef typename allocator_type::reference       reference;
			typedef typename allocator_type::const_reference const_reference;
			typedef typename allocator_type::pointer         pointer;
			typedef typename allocator_type::const_pointer   const_pointer;
			typedef ptrdiff_t                                difference_type;
			typedef size_t                                   size_type;
		private:
			typedef RBTree<value_type, value_compare, allocator_type> underlying_tree_type;
		public:
			typedef typename underlying_tree_type::iterator               iterator;
			typedef typename underlying_tree_type::const_iterator         const_iterator;
			typedef typename underlying_tree_type::const_reverse_iterator reverse_iterator;
			typedef typename underlying_tree_type::reverse_iterator       const_reverse_iterator;
		private:
			underlying_tree_type _base;
			size_type            _size;

			key_compare    _key_comp;
			value_compare  _value_comp;
			allocator_type _alloc;
		public:
			// Constuctors
			// empty
			explicit map (const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type())
				: _base(alloc, value_compare(comp)),
				_key_comp(comp),
				_value_comp(value_compare(comp)),
				_alloc(alloc),
				_size(0) { }
			// range
			template <class InputIterator>
			map (InputIterator first, InputIterator last,
					const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type()) {
				this->insert(first, last);
			}
			// copy
			map (const map& x) {
				this->insert(x.begin(), x.end());
			}

			// Destructor
			~map() { }

			map& operator= (const map& x) {
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
			// Element access
			mapped_type& operator[] (const key_type& k) {
				iterator it = this->find(k);

				if (it != this->_base.end()) {
					return it->second;
				}
				return (this->insert(make_pair(k, mapped_type())).first)->second;
			}

			// Modifiers
			// insert
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
			// with hint (just a gab)
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
			void erase (iterator position) {
				this->erase(position->first);
				--(this->_size);
			}
			size_type erase (const key_type& k) {
				if (this->find(k) == this->end()) {
					return 0;
				}
				_base.delete_node(make_pair(k, mapped_type()));
				return 1;
			}
			void erase (iterator first, iterator last) {
				for (iterator it = first; it != last;) {
					this->erase(it++);
				}
			}

			void swap (map& x) {
				(this->_base).swap(x._base);
				std::swap(this->_size, x._size);
				std::swap(this->_key_comp, x._key_comp);
				std::swap(this->_value_comp, x._value_comp);
				std::swap(this->_alloc, x._alloc);
			}

			void clear() {
				erase(this->begin(), this->end());
			}

			// Observers
			key_compare key_comp() const {
				return (this->_key_comp);
			}
			value_compare value_comp() const {
				return (this->_value_comp);
			}
			// Operations
			iterator find (const key_type& k) {
				return this->_base.find(make_pair(k, mapped_type()));
			}
			const_iterator find (const key_type& k) const {
				return this->_base.find(make_pair(k, mapped_type()));
			}
			size_type count (const key_type& k) const {
				if (this->find(k) != this->end()) {
					return 1;
				}
				return 0;
			}
			iterator lower_bound (const key_type& k) {
				return this->_base.lower_bound(make_pair(k, mapped_type()));
			}
			const_iterator lower_bound (const key_type& k) const {
				return this->_base.lower_bound(make_pair(k, mapped_type()));
			}
			iterator upper_bound (const key_type& k) {
				return this->_base.upper_bound(make_pair(k, mapped_type()));
			}
			const_iterator upper_bound (const key_type& k) const {
				return this->_base.upper_bound(make_pair(k, mapped_type()));
			}
			pair<const_iterator,const_iterator> equal_range (const key_type& k) const {
				return make_pair(this->lower_bound(k), this->upper_bound(k));
			}
			pair<iterator,iterator>             equal_range (const key_type& k) {
				return make_pair(this->lower_bound(k), this->upper_bound(k));
			}
			// Allocator
			allocator_type get_allocator() const {
				return (this->_alloc);
			}

	}; /* class map */
}; /*namespace ft */

#endif /* MAP_HPP */
