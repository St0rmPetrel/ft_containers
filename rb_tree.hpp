#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <functional>
#include <memory>

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

				explicit FrontNode(const key_type& key,
						value_type* data = NULL)
					: key(key), data(data), is_exist(false) {}
				~FrontNode() {}
				private:
					FrontNode();
			} front_node_type;

		public:
			explicit RBTree(
					const key_allocator_type& key_alloc = key_allocator_type(),
					const value_allocator_type& value_alloc =
						value_allocator_type())
				: _key_alloc(key_alloc), _value_alloc(value_alloc) {
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

		private:
			void _inorder_tree_delete(node_type *x) {
				if (!this->_is_null_node(x)) {
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
			node_type* _search(node_type* x, const key_type& k) {
				if (this->_is_null_node(x) || k == x->key)
					return x;
				if (key_compare(k, x->key)) {
					return _search(x->left, k);
				} else {
					return _search(x->right, k);
				}
			}
			void _left_rotate(node_type* x) {
				node_type* y = x->right;
				x->right = y->left;
				if (!this->_is_null_node(y->left)) {
					y->left->parent = x;
				}
				y->parent = x->parent;
				if (!this->_is_null_node(x->parent)) {
					this->_root = y;
				} else if (x == x->parent->left) {
					x->parent->left = y;
				} else {
					x->parent->right = y;
				}
				y->left = x;
				x->parent = y;
			}
			void _right_rotate(node_type* y) {
				node_type* x = y->left;
				y->left = x->right;
				if (!this->_is_null_node(x->left)) {
					x->right->parent = y;
				}
				x->parent = y->parent;
				if (!this->_is_null_node(y->parent)) {
					this->_root = x;
				} else if (y == y->parent->left) {
					y->parent->left = x;
				} else {
					y->parent->right = x;
				}
				x->right = y;
				y->parent = x;
			}
			bool _is_null_node(node_type* x) {
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
	};
}

#endif /* RB_TREE_HPP */
