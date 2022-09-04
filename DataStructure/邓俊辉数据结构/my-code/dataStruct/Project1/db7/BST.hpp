#include "../db5/myBinTree.hpp"
/*����������ģ��*/

template <typename T> class BST : public myBinTree<T> {
protected:
	BinNodePosi(T) _hot;	//���нڵ�ĸ���
	BinNodePosi(T) connect34(	//����3+4�ṹ,����3���ڵ��Լ�4������
		BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, 
		BinNodePosi(T) T0 , BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3
	);
	BinNodePosi(T) rotateAt(BinNodePosi(T) v);	//��x�Լ��丸��,�游����ת����
public:
	virtual BinNodePosi(T)& search(const T& e);	//����
	virtual BinNodePosi(T) insert(const T& e);	//����
	virtual bool remove(const T& e);	//ɾ��
	 
};

//3���ڵ�4������,�����������˳�����ӳ�һ����,�����ĸ��ڵ����ϲ�������ɵ����߽��
template<typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, 
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
{
	//����T0 a T1 b T2 c T3���������˳�����ӳ�һ����
	a->lc = T0; if (T0) T0->parent = a;
	a->rc = T1; if (T1) T1->parent = a;
	myBinTree<T>::updateHeight(a);
	c->lc = T2; if (T2) T2->parent = c;
	c->rc = T3; if (T3) T3->parent = c;
	myBinTree<T>::updateHeight(c);
	b->lc = a; a->parent = b;
	b->rc = c; c->parent = b;
	myBinTree<T>::updateHeight(b);
	return b;//�������ĸ��ڵ㷵��
}

//�ڵ��ϵg->p->v
template<typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent;
	//���������ڵ������λ�ù�ϵ,����connect34������������
	if (IsLChild(*p)) {
		if (IsLChild(*v)) {
			p->parent = g->parent; //�����ĸ��ڵ���������
			return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
		}
		else {
			v->parent = g->parent;
			return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
		}
	}
	else {
		if (IsRChild(*v)) {
			p->parent = g->parent;
			return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
		}
		else {
			v->parent = g->parent;
			return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
		}
	}
}

template<typename T>
BinNodePosi(T)& BST<T>::search(const T& e)
{
	return searchIn(myBinTree<T>::_root, e, _hot = NULL);
}

/*����vΪ���ڵ��BST�����в��ҹؼ���e*/
template<typename T>
static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) {
	if (!v || (v->data == e)) return v;	//�ݹ����: ���ָ��ڵ�ΪNULL ���� �ҵ���Ӧ�Ľڵ�
	hot = v;	//��Ҫ��¼��ǰ�ڵ�,hot�����е�_hot,����Ϊָ�����Ԫ�صĸ��ڵ�(��ʹ����ʧ��,���߽���һ�����ڵ�����Ҳ��������)
	return searchIn(((e < v->data) ? v->lc : v->rc), e, hot);
}

template<typename T>
BinNodePosi(T) BST<T>::insert(const T& e)
{
	BinNodePosi(T)& x = search(e);	//�����в����Ƿ�������Ϊe�Ľڵ�,���ҽ�_hotָ�����ڵ�ĸ��ڵ�
	if (x) return x;	//dataΪe�Ľڵ��Ѿ�������
	x = new myBinNode<T>(e, _hot);		//search�����н�_hot->rc����_hot->lc���ظ�x,���Զ�x��ֵ��ֱ�ӽ���������
	myBinTree<T>::_size++;
	myBinTree<T>::updateHeightAbove(x);
	return x;
}

template<typename T>
bool BST<T>::remove(const T& e)
{
	BinNodePosi(T)& x = search(e);
	if (!x) return false;
	removeAt(x,_hot);
	myBinTree<T>::_size--;
	myBinTree<T>::updateHeightAbove(_hot);
	return true;
}

template<typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
	BinNodePosi(T) del = x;	//del��������ɾ���Ľڵ�
	BinNodePosi(T) succ = NULL;
	if (!HasLChild(*x))		//x������Ϊ��,ֱ�ӽ�x�滻Ϊ������,ͨ��del��ɾ��ԭ�ڵ�
		succ = x = x->rc;
	else if (!HasRChild(*x))		//x������Ϊ��,ֱ�ӽ�x�滻Ϊ������
		succ = x = x->lc;
	else {	//������������,�򽫸ýڵ����ֱ�Ӻ�̻�λ��
		del = del->succ();	//�����������һλ��
		swap(del->data, x->data);	//�������ݺ�,����Ҫɾ���Ľڵ����del��
		BinNodePosi(T) delParent = del->parent;
		//�����������,�д�ɾ���ڵ�a����ֱ�Ӻ��b,
		//Ҫôb��a���ҽڵ���b����������,
		//Ҫôb��a���������ж��Ҳ���a���ҽڵ�,��ʱbû����ڵ�(����b����ڵ����a��ֱ�Ӻ��)
		//����,bֻ������������,���������������,����del��������
		if (delParent == x)
			delParent->rc = succ = del->rc;	//b����parent��rc
		else
			delParent->lc = succ = del->rc;	//b����parent��lc
	}
	hot = del->parent;//hotΪɾ���ڵ�ĸ���
	if (succ) succ->parent = hot;		//��ԭ���ĺ�̺�ɾ���ڵ�ĸ��ڵ����
	release(del->data); release(del);
	return succ;	//���ؽ�����
}