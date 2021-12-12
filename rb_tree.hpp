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
		private:
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
		public:
			typedef struct FrontNode {
				bool            is_exist;
				const key_type& key;
				value_type*     data;

				explicit FrontNode(const key_type& key, value_type* data = NULL)
					: key(key), data(data), is_exist(false) {}
				~FrontNode() {}
				private:
					FrontNode();
			} front_node_type;

		public:
			explicit RBTree(
					const key_allocator_type& key_alloc = key_allocator_type(),
					const value_allocator_type& value_alloc = value_allocator_type(),
					const key_compare& key_cmp = key_compare())
				: _key_alloc(key_alloc),
				_value_alloc(value_alloc),
				_key_cmp(key_cmp) {
				this->_null_node = new TreeNode;
				this->_null_node->c = Black;
				this->_root = this->_null_node;
			}
			~RBTree() {
				this->_inorder_tree_delete(this->_root);
				delete this->_null_node;
			}

			front_node_type search(const key_type& k) {
				front_node_type ret_node(k);
				node_type* under_node = _search(this->_root, k);
				if (!_is_null_node(under_node)) {
					ret_node.data = under_node->data;
					ret_node.is_exist = true;
				}
				return ret_node;
			}

			void insert_node(const key_type& k, value_type* data = NULL) {
				node_type* new_node = _create_node(k, data);

				node_type* y = NULL;
				node_type* x = this->_root;

				while (!_is_null_node(x)) {
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
				if (_is_null_node(z)) {
					return;
				}
				_delete_node(z);
			}

			void debug() {
				this->_debug(this->_root);
			}

		private:
			void _debug(node_type *x) {
				if (!this->_is_null_node(x)) {
					_debug(x->left);
					std::cout << *(x->key) << " ";
					_debug(x->right);
				}
			}
			void _delete_node(node_type* z) {
				node_type *x, *y;
				Color y_original_color;

				if (_is_null_node(z) || z == NULL) {
					return;
				}

				y = z;
				y_original_color = y->c;
				if (_is_null_node(z->left)) {
					x = z->right;
					_transplant(z, z->right);
				} else if (_is_null_node(z->right)) {
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
			void _inorder_tree_delete(node_type *x) {
				if (!_is_null_node(x)) {
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
			node_type* _create_node(const key_type& k, value_type* data) {
				node_type* ret = new TreeNode;
				ret->data = data;
				ret->key = _key_alloc.allocate(1);
				_key_alloc.construct(ret->key, k);
				ret->p = NULL;
				ret->left = _null_node;
				ret->right = _null_node;
				ret->c = Red;
				return ret;
			}
			node_type* _minimum(node_type* x) {
				while (!_is_null_node(x->left)) {
					x = x->left;
				}
				return x;
			}
			node_type* _maximum(node_type* x) {
				while (!_is_null_node(x->right)) {
					x = x->right;
				}
				return x;
			}
			node_type* _search(node_type* x, const key_type& k) {
				if (this->_is_null_node(x) || k == *(x->key))
					return x;
				if (_key_cmp(k, *(x->key))) {
					return _search(x->left, k);
				} else {
					return _search(x->right, k);
				}
			}

			void _left_rotate(node_type* x) {
				node_type* y = x->right;
				x->right = y->left;
				if (!_is_null_node(y->left)) {
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
				if (!_is_null_node(y->right)) {
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
				if (_is_null_node(u->p)) {
					this->_root = v;
				} else if (u == u->p->left) {
					u->p->left = v;
				} else {
					u->p->right = v;
				}
				v->p = u->p;
			}

			bool _is_null_node(node_type* x) {
				if (x == NULL) {
					return true;
				}
				if (x->key == NULL) {
					return true;
				}
				return false;
			}

		private:
			node_type*           _root;
			node_type*           _null_node;

			key_allocator_type   _key_alloc;
			value_allocator_type _value_alloc;
			key_compare          _key_cmp;
	};
}

#endif /* RB_TREE_HPP */
