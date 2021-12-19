#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <functional> // std::allocator
#include <memory>     // std::less

namespace ft {
	template <  class Key,
				class Compare = std::less<Key>,
				class Allocator = std::allocator<Key> >
	class RBTree {
		public:
			typedef Key          key_type;
			typedef Compare      key_compare;
			typedef Allocator    allocator_type;
		private:
			enum Color { Red, Black };
			typedef struct TreeNode {
				key_type    *key;

				Color       c;
				TreeNode    *left;
				TreeNode    *right;
				TreeNode    *p;

				TreeNode()
					: key(NULL),
					left(NULL),
					right(NULL),
					p(NULL),
					c(Black) {}
				~TreeNode() {}
			} node_type;

		private:
			node_type*           _root;
			node_type*           TNULL;

			node_type null_node;

			allocator_type _alloc;
			key_compare    _key_cmp;

		public:
			class const_iterator {
				private:
					node_type* _base;
					RBTree*    _enclosing;
				public:
					const_iterator() : _base(NULL), _enclosing(NULL) { }
					const_iterator(RBTree* e, node_type* base) : _enclosing(e), _base(base) { }
					const_iterator(const const_iterator& src) :
						_base(src._base), _enclosing(src._enclosing) { }
					~const_iterator() { }

					const_iterator& operator= (const const_iterator& src) {
						if (this != &src) {
							this->_base = src._base;
							this->_enclosing = src._enclosing;
						}
						return (*this);
					}
				public:
					bool operator==(const const_iterator &other) const {
						return (this->_base == other._base) &&
							(this->_enclosing == other._enclosing);
					}
					bool operator!=(const const_iterator &other) const {
						return (this->_base != other._base) ||
							(this->_enclosing != other._enclosing);
					}

					const key_type& operator*() const {
						return *(this->_base->key);
					}
					const key_type* operator->() const {
						return &(operator*());
					}

					// Pre-increment
					const_iterator& operator++() {
						this->_base = _enclosing->_successor_iteration(this->_base);
						return (*this);
					}
					// Post-increment
					const_iterator operator++(int) {
						const_iterator temp = *this;
						++(*this);
						return temp;
					}
					// Pre-decrement
					const_iterator& operator--() {
						this->_base = _enclosing->_predecessor_iteration(this->_base);
						return (*this);
					}
					// Post-decrement
					const_iterator operator--(int) {
						const_iterator temp = *this;
						--(*this);
						return temp;
					}
			};

		public:
			explicit RBTree(
					const allocator_type& alloc = allocator_type(),
					const key_compare& key_cmp = key_compare())
				: _alloc(alloc), _key_cmp(key_cmp) {
				this->TNULL = &null_node;
				this->_root = TNULL;
			}
			~RBTree() {
				this->_inorder_tree_delete(this->_root);
			}

			void insert_node(const key_type& k) {
				node_type* new_node = _create_node(k);

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

			const_iterator begin() {
				return (RBTree::const_iterator(this, _minimum(this->_root)));
			}
			const_iterator end() {
				return (RBTree::const_iterator(this, TNULL));
			}
			const_iterator find(const key_type& k) {
				return (RBTree::const_iterator(this, _search(this->_root, k)));
			}

		private:
			void _inorder_tree_delete(node_type *x) {
				if (x != TNULL) {
					_inorder_tree_delete(x->left);
					node_type *next = x->right;
					//delte key
					_alloc.destroy(x->key);
					_alloc.deallocate(x->key, 1);
					delete x;
					_inorder_tree_delete(next);
				}
			}
			node_type* _create_node(const key_type& k) {
				node_type* ret = new TreeNode;

				// Copy k in *(ret->key)
				ret->key = _alloc.allocate(1);
				_alloc.construct(ret->key, k);

				ret->p = NULL;
				ret->left = TNULL;
				ret->right = TNULL;
				ret->c = Red;
				return ret;
			}
			node_type* _minimum(node_type* x) const {
				while (x != TNULL && x->left != TNULL) {
					x = x->left;
				}
				return x;
			}
			node_type* _maximum(node_type* x) const {
				while (x != TNULL && x->right != TNULL) {
					x = x->right;
				}
				return x;
			}
			node_type* _search(node_type* x, const key_type& k) const {
				if (x == TNULL ||
						//k == *(x->key)
						(!_key_cmp(k, *(x->key)) && !_key_cmp(*(x->key), k)) ) {
					return x;
				}
				if (_key_cmp(k, *(x->key))) {
					return _search(x->left, k);
				} else {
					return _search(x->right, k);
				}
			}
			// Successor/Predecessor interation allgorithm
			// _successor_iteration retrun node after prev
			node_type* _successor_iteration(node_type* prev) const {
				if (prev == TNULL) {
					return _minimum(this->_root);
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
				if (next == TNULL) {
					return _maximum(this->_root);
				}
				if (next->left != TNULL) {
					return _maximum(next->left);
				}
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
				// delte key
				_alloc.destroy(z->key);
				_alloc.deallocate(z->key, 1);
				// delete node
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
