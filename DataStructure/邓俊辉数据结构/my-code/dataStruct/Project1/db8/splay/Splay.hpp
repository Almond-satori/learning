#include "../../db7/BST.hpp"

template <typename T> class Splay : public BST<T> {
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v);//���ڵ�v����������
public:
	BinNodePosi(T)& search(const T& e);//����(������ṹ�ı�)
	BinNodePosi(T) insert(const T& e);//����
	bool remove(const T& e);//ɾ��
};

template <typename NodePosi> inline
void attachAsLChild(NodePosi p, NodePosi lc) {//NodePosi��myBinNode<T>*
	p->lc = lc; if (lc) lc->parent = p;
}

template <typename NodePosi> inline
void attachAsRChild(NodePosi p, NodePosi rc) {//NodePosi��myBinNode<T>*
	p->rc = rc; if (rc) rc->parent = p;
}

template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
	if (!v) return NULL;
	BinNodePosi(T) p; BinNodePosi(T) g;
	while ( (p = v->parent) && (g = p->parent) ) {
		BinNodePosi(T) gg = g->parent;//�����ϲ�ڵ�
		if (IsLChild(*p)) {
			if (IsLChild(*v)) {	//zig-zig
				attachAsLChild(p, v->rc);
				attachAsLChild(g, p->rc);
				attachAsRChild(p, g);
				attachAsRChild(v, p);
			}
			else {	//zag-zig
				attachAsRChild(p, v->lc);
				attachAsLChild(g, v->rc);
				attachAsLChild(v, p);
				attachAsRChild(v, g);
			}
		}
		else {
			if (IsRChild(*v)) {	//zag-zag
				attachAsRChild(p, v->lc);
				attachAsRChild(g, p->lc);
				attachAsLChild(p, g);
				attachAsLChild(v, p);
			}
			else {	//zig-zag
				attachAsLChild(p, v->rc);
				attachAsRChild(g, v->lc);
				attachAsLChild(v, g);
				attachAsRChild(v, p);
			}
		}
		//�����ϲ�ڵ�gg
		if (!gg) v->parent = NULL;//v�Ǹ��ڵ�
		else
			(g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);//����ԭ���ķ�ʽ����gg
		myBinTree<T>::updateHeight(v); myBinTree<T>::updateHeight(p); myBinTree<T>::updateHeight(g);
	}//while
	//vһ����������,������Ϊż��(������1��),��ʱv->parent��root,��Ҫ���ֶ�����һ��;�������Ϊ����,v��root
	if (p = v->parent) {
		if (IsLChild(*v)) {
			attachAsLChild(p, v->rc); attachAsRChild(v, p);
		}
		else {
			attachAsRChild(p, v->lc); attachAsLChild(v, p);
		}
		myBinTree<T>::updateHeight(v); myBinTree<T>::updateHeight(p);
	}
	v->parent = NULL;
	return v;
}

template<typename T>
BinNodePosi(T) & Splay<T>::search(const T& e)
{
	BinNodePosi(T) p = searchIn(myBinTree<T>::_root, e, BST<T>::_hot = NULL);
	myBinTree<T>::_root = splay((p) ? p : BST<T>::_hot);
	return myBinTree<T>::_root;
}

template<typename T>
BinNodePosi(T) Splay<T>::insert(const T& e)
{	
	//�˻����,�������û��Ԫ��
	if (!myBinTree<T>::_root) { myBinTree<T>::_size++; return myBinTree<T>::_root = new myBinNode<T>(e); }
	//��������Ѿ���Ԫ��e
	if ((search(e)->data) == e) return myBinTree<T>::_root;
	//����,��ӽ�e��Ԫ��_hot������Ϊ���ڵ�
	BinNodePosi(T) t = myBinTree<T>::_root;
	myBinTree<T>::_size++;
	//���1:t��������>e>t(search����ѡ��t,���̺���e<t��������������)
	if (e > t->data) {
		t->parent = myBinTree<T>::_root = new myBinNode<T>(e, NULL, t, t->rc);
		if (HasRChild(*t)) { t->rc->parent = myBinTree<T>::_root; t->rc = NULL; }
	}
	else {
		t->parent = myBinTree<T>::_root = new myBinNode<T>(e, NULL, t->lc, t);
		if (HasLChild(*t)) { t->lc->parent = myBinTree<T>::_root; t->lc = NULL; }
	}
	myBinTree<T>::updateHeightAbove(t);
	return myBinTree<T>::_root; 
}

template<typename T>
bool Splay<T>::remove(const T& e)
{
	if (!myBinTree<T>::_root || e != search(e)->data) return false;
	BinNodePosi(T) v = myBinTree<T>::_root;//��ɾ���ڵ��Ѿ��ŵ�_rootλ��
	//������һ������,ֱ��ɾ��v
	if (!HasLChild(*v)) {
		myBinTree<T>::_root = myBinTree<T>::_root->rc; 
		if(myBinTree<T>::_root) myBinTree<T>::_root->parent = NULL;
	}
	else if (!HasRChild(*v)) {
		myBinTree<T>::_root = myBinTree<T>::_root->lc;
		if (myBinTree<T>::_root) myBinTree<T>::_root->parent = NULL;
	}
	else {//������������
		BinNodePosi(T) lTree = myBinTree<T>::_root->lc;
		myBinTree<T>::_root->lc = NULL;
		lTree->parent = NULL;//�Ͽ�������
		myBinTree<T>::_root = myBinTree<T>::_root->rc; myBinTree<T>::_root->parent = NULL;
		search(e);//����������Ѱ����ӽ�e�Ľڵ�(��β���һ��ʧ��,���Ὣ����e����Сֵ���ڸ��ڵ���)
		myBinTree<T>::_root->lc = lTree; myBinTree<T>::_root->parent = NULL; 
	}
	release(v->data); release(v); myBinTree<T>::_size--;//�ͷŽڵ�
	if (myBinTree<T>::_root) myBinTree<T>::updateHeight(myBinTree<T>::_root);
	return true;
}
