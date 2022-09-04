#include "priorityQueue.hpp"
#include "../db5/myBinTree.hpp"

template <typename T>
class PQ_LeftHeap : public PQ<T>, public myBinTree<T> {
public:
	PQ_LeftHeap(){} //默认构造
	PQ_LeftHeap(T* E, int n){ //通过数组批量构造,为简化这里简单地插入元素
		for (int i = 0; i < n; i++) insert(E[i]);
	}
	void insert(T e);
	T getMax();
	T delMax();
	void insert(T e);
};

//将两个左式堆合并,堆的规模由调用者更新
template <typename T> 
static BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b) {
	//递归基,
	if (!a) return b;
	if (!b) return a;
	//保证a的数据值更大,因为a作为局部子树的根节点
	if (a->data < b->data) swap(a, b);
	a->rc = merge(a->rc, b); //递归地将a的右子树与b合并,合并结果作为a的新右子树
	a->rc->parent = a; //子树连接根
	//最终,a的右子树或者b为空,将不为空的一边返回给`外层a`的右子树
	if (!a->lc || a->lc->npl < a->rc->npl) { //解决左孩子npl更小的问题(甚至左子树可能为空)
		swap(a->lc, a->rc);
	}
	a->npl = (a->rc) ? a->rc->npl + 1 : 1; //更新根节点的npl
	return a;
}

//基于merge的删除操作
template<typename T>
T PQ_LeftHeap<T>::delMax()
{
	BinNodePosi(T) left = _root->lc;
	BinNodePosi(T) right = _root->rc;
	T e = _root->data;
	delete _root; _size--;
	_root = merge(left, right);
	if (_root) _root->parent = NULL;
	return e;
}

template<typename T>
void PQ_LeftHeap<T>::insert(T e)
{
	BinNodePosi(T) v = new myBinNode(e);
	_root = merge(_root, v); _size++;
	_root->parent = NULL;
}


