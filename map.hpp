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
		public:
			class iterator {
				// Need to test how it actualy work
			}; /* iterator */
			class const_iterator {
				// Need to test how it actualy work
			}; /* const_iterator */
		public:
			typedef reverse_iterator_adapter<iterator>       reverse_iterator;
			typedef reverse_iterator_adapter<const_iterator> const_reverse_iterator;
			typedef ptrdiff_t                                difference_type;
			typedef size_t                                   size_type;
		private:
			typedef RBTree<value_type, value_compare, allocator_type> underlying_tree_type;
		private:
			underlying_tree_type _base;
		public:
			// Constuctors

			// Destructor
	}; /* class map */
}; /*namespace ft */

#endif /* MAP_HPP */
