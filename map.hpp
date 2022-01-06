#ifndef MAP_HPP
# define MAP_HPP

#include <functional>
#include <memory>

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
			}
			// copy
			map (const map& x) {
			}

			// Destructor
			~map() { }
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
			// Allocator
			allocator_type get_allocator() const {
				return (this->_alloc);
			}

	}; /* class map */
}; /*namespace ft */

#endif /* MAP_HPP */
