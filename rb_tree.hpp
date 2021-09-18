#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <functional>

namespace ft {
	template <  class Key,
			 	class T = Key,
				class Compare = std::less<Key> >
	class RBTree {
		public:
			typedef Key     key_type;
			typedef T*      value_type;
			typedef Compare key_compare;
		private:
			typedef struct Treenode {
				key_type    key;
				value_type  data;
				Treenode    *left;
				Treenode    *right;
				Treenode    *p;

				Treenode(key_type key = key_type(), value_type data = NULL)
					: key(key), data(data), left(NULL), right(NULL), p(NULL) {}
				~Treenode() {}

				void build(const key_type& k, value_type d) {
					this->key = k;
					this->data = d;
				}
			} node_t;

		public:
			RBTree() : _root(NULL) {}
			~RBTree() {
				this->_inorder_tree_delete(this->_root);
			}

			value_type& search(const key_type& k) {
				return (_search(this->_root, k)->data);
			}

			void insert(const key_type& k, value_type d) {
				node_t* new_node = new Treenode;
				new_node->build(k, d);
				_root = new_node;
			}
		private:
			void _inorder_tree_delete(node_t *x) {
				if (x != NULL) {
					_inorder_tree_delete(x->left);
					node_t *next = x->right;
					if (x->data != NULL) {
						delete x->data;
					}
					delete x;
					_inorder_tree_delete(next);
				}
			}
			node_t* _search(node_t* x, const key_type& k) {
				if (x == NULL || k == x->key)
					return x;
				if (key_compare(k, x->key)) {
					return _search(x->left, k);
				} else {
					return _search(x->right, k);
				}
			}
			void _left_rotate(node_t* x) {
				node_t* y = x->right;
				x->right = y->left;
				if (y->left != NULL) {
					y->left->parent = x;
				}
				y->parent = x->parent;
				if (x->parent == NULL) {
					this->_root = y;
				} else if (x == x->parent->left) {
					x->parent->left = y;
				} else {
					x->parent->right = y;
				}
				y->left = x;
				x->parent = y;
			}
			void _right_rotate(node_t* y) {
				node_t* x = y->left;
				y->left = x->right;
				if (x->right != NULL) {
					x->right->parent = y;
				}
				x->parent = y->parent;
				if (y->parent == NULL) {
					this->_root = x;
				} else if (y == y->parent->left) {
					y->parent->left = x;
				} else {
					y->parent->right = x;
				}
				x->right = y;
				y->parent = x;
			}

		private:
			node_t *_root;
	};
}

#endif /* RB_TREE_HPP */
