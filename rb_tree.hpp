#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <functional>
#include <memory>

#include <iostream>

namespace ft {
	template <  class Key,
			 	class T = Key,
				class Compare = std::less<Key>,
				class AllocatorKey = std::allocator<Key>,
				class AllocatorT = std::allocator<T> >
	class RBTree {
		public:
			typedef Key          key_type;
			typedef T            value_type;
			typedef Compare      key_compare;
			typedef AllocatorKey key_allocator_type;
			typedef AllocatorT   value_allocator_type;
		public:
			enum Color { Red, Black };
			typedef struct TreeNode {
				key_type    *key;
				value_type  *data;

				Color       c;
				TreeNode    *left;
				TreeNode    *right;
				TreeNode    *p;

				TreeNode()
					: key(NULL),
					data(NULL),
					left(NULL),
					right(NULL),
					p(NULL),
					c(Red) {}
				~TreeNode() {}
			} node_type;

			typedef class Iterator {
				public:
					typedef node_type*       pointer;
				public:
					Iterator() : _base(NULL) {}
					Iterator(pointer ptr) : _base(ptr) {}
					Iterator(const Iterator& src) : _base(src.base()) {}

					Iterator& operator=(const Iterator& src) {
						if (this != &src) {
							this->_base = src.base();
						}
						return (*this);
					}
					~Iterator() {}

					pointer base() const {
						return this->_base;
					}
					// pre-increment
					Iterator operator++() {
						//this->base = //next node
					}
					// post-increment
					Iterator operator++(int) {
						Iterator temp = *this;
						++(*this);
						return temp;
					}
				private:
					pointer _base;
			} iterator;

		private:
			node_type*           _root;
			node_type*           TNULL;

			key_allocator_type   _key_alloc;
			value_allocator_type _value_alloc;
			key_compare          _key_cmp;

		public:
			explicit RBTree(
					const key_allocator_type& key_alloc = key_allocator_type(),
					const value_allocator_type& value_alloc = value_allocator_type(),
					const key_compare& key_cmp = key_compare())
				: _key_alloc(key_alloc),
				_value_alloc(value_alloc),
				_key_cmp(key_cmp) {
				this->TNULL = new TreeNode;
				this->TNULL->c = Black;

				this->_root = TNULL;
			}
			~RBTree() {
				this->_inorder_tree_delete(this->_root);
				delete TNULL;
			}

			void insert_node(const key_type& k, value_type* data = NULL) {
				node_type* new_node = _create_node(k, data);

				node_type* y = NULL;
				node_type* x = this->_root;

				while (x != TNULL) {
					y = x;
					if (_key_cmp(*(new_node->key), *(x->key))) {
					//if (*(new_node->key) < *(x->key)) {
						x = x->left;
					} else {
						x = x->right;
					}
				}

				new_node->p = y;
				if (y == NULL) {
					this->_root = new_node;
				//} else if (*(new_node->key) < *(y->key)) {
				} else if (_key_cmp(*(new_node->key), *(y->key))) {
					y->left = new_node;
				} else {
					y->right = new_node;
				}

				if (new_node->p == NULL) {
					new_node->c = Black;
					return;
				}

				if (new_node->p->p == NULL) {
					return;
				}

				_insert_fixup(new_node);
			}
			void delete_node(const key_type& k) {
				node_type* z = _search(this->_root, k);
				if (z == TNULL) {
					return;
				}
				_delete_node(z);
			}

			const node_type* minimum() const {
				return _minimum(this->_root);
			}
			const node_type* maximum() const {
				return _maximum(this->_root);
			}
			const node_type* search(const key_type& k) {
				return _search(this->_root, k);
			}

			void debug() const {
				this->_debug(this->_root);
			}

		private:
			void _debug(node_type *x) const {
				//if (x != TNULL) {
				//	_debug(x->left);
				//	std::cout << *(x->key) << " ";
				//	_debug(x->right);
				//}
				node_type* current = _minimum(x);
				while (current != TNULL) {
					std::cout << *(current->key) << " ";
					current = _successor_iteration(current);
				}
				std::cout << std::endl;
				//current = _maximum(x);
				//while (current != TNULL) {
				//	std::cout << *(current->key) << " ";
				//	current = _predecessor_iteration(current);
				//}
			}
			void _inorder_tree_delete(node_type *x) {
				if (x != TNULL) {
					_inorder_tree_delete(x->left);
					node_type *next = x->right;
					//delte key
					_key_alloc.destroy(x->key);
					_key_alloc.deallocate(x->key, 1);
					//delte data
					if (x->data != NULL) {
						_value_alloc.destroy(x->data);
						_value_alloc.deallocate(x->data, 1);
					}
					delete x;
					_inorder_tree_delete(next);
				}
			}
			node_type* _create_node(const key_type& k, value_type* data) {
				node_type* ret = new TreeNode;
				ret->data = data;
				ret->key = _key_alloc.allocate(1);
				_key_alloc.construct(ret->key, k);
				ret->p = NULL;
				ret->left = TNULL;
				ret->right = TNULL;
				ret->c = Red;
				return ret;
			}
			node_type* _minimum(node_type* x) const {
				while (x->left != TNULL) {
					x = x->left;
				}
				return x;
			}
			node_type* _maximum(node_type* x) const {
				while (x->right != TNULL) {
					x = x->right;
				}
				return x;
			}
			node_type* _search(node_type* x, const key_type& k) const {
				if (x == TNULL || k == *(x->key))
					return x;
				if (_key_cmp(k, *(x->key))) {
					return _search(x->left, k);
				} else {
					return _search(x->right, k);
				}
			}
			// Successor/Predecessor interation allgorithm
			// _successor_iteration retrun node after prev
			node_type* _successor_iteration(node_type* prev) const {
				if (prev == NULL || prev == TNULL) {
					return NULL;
				}
				// The successor of a node is:
				//   Next-R rule
				//   If it has a right subtree, the leftmost node in the right subtree.
				if (prev->right != TNULL) {
					return _minimum(prev->right);
				}
				//   Next-U rule
				//   Otherwise, traverse up the tree.
				//    If prev node was a left child then that parent node is the successor.
				//    If prev node was a right child, continue going up.
				//    If you can't go up anymore, then there's no successor.
				while (prev->p != NULL && prev == prev->p->right) {
					prev = prev->p;
				}
				if (prev->p == NULL) {
					return TNULL;
				}
				return prev->p;
			}
			// _predecessor_iteration return node before next
			node_type* _predecessor_iteration(node_type* next) const {
				if (next == NULL || next == TNULL) {
					return NULL;
				}
				// The predecessor  of a node is:
				//   Next-L rule
				//   If it has a right subtree, the rightmost node in the left subtree.
				if (next->left != TNULL) {
					return _maximum(next->right);
				}
				//   Next-U rule
				//   Otherwise, traverse up the tree.
				//    If next node was a right child then that parent node is the predecessor.
				//    If next node was a left child, continue going up.
				//    If you can't go up anymore, then there's no predecessor.
				while (next->p != NULL && next == next->p->left) {
					next = next->p;
				}
				if (next->p == NULL) {
					return TNULL;
				}
				return next->p;
			}


			void _delete_node(node_type* z) {
				node_type *x, *y;
				Color y_original_color;

				y = z;
				y_original_color = y->c;
				if (z->left == TNULL) {
					x = z->right;
					_transplant(z, z->right);
				} else if (z->right == TNULL) {
					x = z->left;
					_transplant(z, z->left);
				} else {
					y = _minimum(z->right);
					y_original_color = y->c;
					x = y->right;
					if (y->p == z) {
						x->p = y;
					} else {
						_transplant(y, y->right);
						y->right = z->right;
						y->right->p = y;
					}
					_transplant(z, y);
					y->left = z->left;
					y->left->p = y;
					y->c = z->c;
				}

				// Delete node z
				//delte key
				_key_alloc.destroy(z->key);
				_key_alloc.deallocate(z->key, 1);
				//delte data
				if (z->data != NULL) {
					_value_alloc.destroy(z->data);
					_value_alloc.deallocate(z->data, 1);
				}
				delete z;

				if (y_original_color == Black) {
					_delete_fixup(x);
				}
			}

			void _insert_fixup(node_type *z) {
				node_type* y;

				while (z->p->c == Red) {
					// Z is son of left parent (has righ uncle)
					if (z->p == z->p->p->left) {
						y = z->p->p->right;
						// Red uncle (just recolor)
						if (y->c == Red) {
							y->c = Black;
							z->p->c = Black;
							z->p->p->c = Red;
							z = z->p->p;
						// Black uncle (with rotation)
						} else {
							if (z == z->p->right) {
								z = z->p;
								_left_rotate(z);
							}
							z->p->c = Black;
							z->p->p->c = Red;
							_right_rotate(z->p->p);
						}
					// Z is son of right parent (has left uncle)
					} else {
						y = z->p->p->left;
						// Red uncle (just recolor)
						if (y->c == Red) {
							y->c = Black;
							z->p->c = Black;
							z->p->p->c = Red;
							z = z->p->p;
						// Black uncle (with rotation)
						} else {
							if (z == z->p->left) {
								z = z->p;
								_right_rotate(z);
							}
							z->p->c = Black;
							z->p->p->c = Red;
							_left_rotate(z->p->p);
						}
					}
					if (z == this->_root) {
						break;
					}
				}
				_root->c = Black;
			}
			void _delete_fixup(node_type* x) {
				node_type* s;
				while (x != this->_root && x->c == Black) {
					if (x == x->p->left) {
						s = x->p->right;
						if (s->c == Red) {
							s->c = Black;
							x->p->c = Red;
							_left_rotate(x->p);
							s = x->p->right;
						}

						if (s->left->c == Black && s->right->c == Black) {
							s->c = Red;
							x = x->p;
						} else {
							if (s->right->c == Black) {
								s->left->c = Black;
								s->c = Red;
								_right_rotate(s);
								s = x->p->right;
							}

							s->c = x->p->c;
							x->p->c = Black;
							s->right->c = Black;
							_left_rotate(x->p);
							x = this->_root;
						}
					} else {
						s = x->p->left;
						if (s->c == Red) {
							s->c = Black;
							x->p->c = Red;
							_right_rotate(x->p);
							s = x->p->left;
						}

						if (s->right->c == Black && s->left->c == Black) {
							s->c = Red;
							x = x->p;
						} else {
							if (s->left->c == Black) {
								s->right->c = Black;
								s->c = Red;
								_left_rotate(s);
								s = x->p->left;
							}

							s->c = x->p->c;
							x->p->c = Black;
							s->left->c = Black;
							_right_rotate(x->p);
							x = this->_root;
						}
					}
				}
				x->c = Black;
			}

			void _left_rotate(node_type* x) {
				node_type* y = x->right;
				x->right = y->left;
				if (y->left != TNULL) {
					y->left->p = x;
				}
				y->p = x->p;
				if (x->p == NULL) {
					this->_root = y;
				} else if (x == x->p->left) {
					x->p->left = y;
				} else {
					x->p->right = y;
				}
				y->left = x;
				x->p = y;
			}
			void _right_rotate(node_type* x) {
				node_type* y = x->left;
				x->left = y->right;
				if (y->right != TNULL) {
					y->right->p = x;
				}
				y->p = x->p;
				if (x->p == NULL) {
					this->_root = y;
				} else if (x == x->p->right) {
					x->p->right = y;
				} else {
					x->p->left = y;
				}
				y->right = x;
				x->p = y;
			}
			void _transplant(node_type* u, node_type* v) {
				if (u->p == NULL) {
					this->_root = v;
				} else if (u == u->p->left) {
					u->p->left = v;
				} else {
					u->p->right = v;
				}
				v->p = u->p;
			}
	};
}

#endif /* RB_TREE_HPP */
