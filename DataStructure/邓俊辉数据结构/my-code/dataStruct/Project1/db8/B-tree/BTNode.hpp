/* B-树的节点BTNode类 */

#include "../../db1-4/2-1Vector.hpp"
#define BTNodePosi(T) BTNode<T>* 

template <typename T> struct BTNode {
	BTNodePosi(T) parent;//父节点
	Vector<T> key;//关节码向量
	Vector<BTNodePosi(T)> child;//孩子向量

//构造函数,BTNode只能作为根节点创造,包含0个关键码和一个空的孩子指针
	BTNode() { parent = NULL; child.insert(NULL, 0); }

	BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL) {
		parent = NULL;
		key.insert(e,0);
		child.insert(lc,0);
		child.insert(rc,1);
		if (lc) lc->parent = this;
		if (rc) rc->parent = this;
	}
};