#ifndef MAP_HPP
# define MAP_HPP

#include "rb_tree.hpp"
#include "pair.hpp"

namespace ft {
	template <  class Key,                                     // map::key_type
			 	class T,                                       // map::mapped_type
				class Compare = less<Key>,                     // map::key_compare
				class Alloc = allocator<pair<const Key,T> >    // map::allocator_type
	class map {
		public:
			typedef Key                                      key_type;
			typedef T                                        mapped_type;
			typedef pair<const key_type,mapped_type>         value_type;
			typedef Compare                                  key_compare;
		public:
			class value_compare {
				protected:
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

			key_compare    _comp;
			allocator_type _alloc;
		public:
			// Constuctors
			// empty
			explicit map (const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type()) {
			}
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

	}; /* class map */
}; /*namespace ft */

#endif /* MAP_HPP */
