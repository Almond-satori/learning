#include "priorityQueue.hpp"
#include "../db5/myBinTree.hpp"

template <typename T>
class PQ_LeftHeap : public PQ<T>, public myBinTree<T> {
public:
	PQ_LeftHeap(){} //Ĭ�Ϲ���
	PQ_LeftHeap(T* E, int n){ //ͨ��������������,Ϊ������򵥵ز���Ԫ��
		for (int i = 0; i < n; i++) insert(E[i]);
	}
	void insert(T e);
	T getMax();
	T delMax();
	void insert(T e);
};

//��������ʽ�Ѻϲ�,�ѵĹ�ģ�ɵ����߸���
template <typename T> 
static BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b) {
	//�ݹ��,
	if (!a) return b;
	if (!b) return a;
	//��֤a������ֵ����,��Ϊa��Ϊ�ֲ������ĸ��ڵ�
	if (a->data < b->data) swap(a, b);
	a->rc = merge(a->rc, b); //�ݹ�ؽ�a����������b�ϲ�,�ϲ������Ϊa����������
	a->rc->parent = a; //�������Ӹ�
	//����,a������������bΪ��,����Ϊ�յ�һ�߷��ظ�`���a`��������
	if (!a->lc || a->lc->npl < a->rc->npl) { //�������npl��С������(��������������Ϊ��)
		swap(a->lc, a->rc);
	}
	a->npl = (a->rc) ? a->rc->npl + 1 : 1; //���¸��ڵ��npl
	return a;
}

//����merge��ɾ������
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


