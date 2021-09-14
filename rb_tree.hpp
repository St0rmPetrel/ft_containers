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
			typedef struct TreeNode {
				const key_type key;
				value_type     data;
				TreeNode       *left;
				TreeNode       *right;
				TreeNode       *p;

				TreeNode(key_type key = key_type(), value_type data = NULL)
					: key(key), data(data), left(NULL), right(NULL), p(NULL) {}
				~TreeNode() {}
			} node;

		public:
			RBTree() : _root(NULL) {}
			~RBTree() {
				this->_inorder_tree_delete(this->_root);
			}

			value_type& search() {
				return (_search(this->_root, k).data);
			}
		private:
			void _inorder_tree_delete(node *x) {
				if x != NULL {
					_inorder_tree_delete(x.left);
					node *next = x.right;
					if x.data != NULL {
						delete x.data;
					}
					delete x;
					_inorder_tree_delete(next);
				}
			}
			node* _search(node* x, const key_type& k) {
				if x == NULL || k == x.key
					return x;
				if key_compare(k, x.key) {
					return _search(x.left, k);
				} else
					return _search(x.right, k);
				}
			}


			node *_root;
	};
}

#endif /* RB_TREE_HPP */
