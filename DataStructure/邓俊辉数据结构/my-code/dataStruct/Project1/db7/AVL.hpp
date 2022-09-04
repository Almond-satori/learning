#include "BST.hpp"

// x �� myBinNode<T> ����
#define Balanced(x) ( (stature((x).lc) ) == ( stature((x).rc)) )
#define BalFac(x) ( stature((x).lc) - stature((x).rc) )
#define AvlBalanced(x) ( (BalFac(x) > -2) && (BalFac(x) < 2) )

// p �� myBinNode<T> * ����,��BinNodePosi(T)
//������������ȡ�����,���ȸ�,��ȡ�븸��ͬ����
#define tallerChild(p) (	/*���*/ \
	stature((p)->lc) > stature((p)->rc) ? (p)->lc : ( /*�Ҹ�*/ \
	stature((p)->rc) > stature((p)->lc) ? (p)->rc : ( /*�ȸ�,ѡ���븸��ͬ��(���ڵ�����ڵ�,��ѡ��ڵ�)*/ \
	IsLChild( *(p) ) ? (p)->lc : (p)->rc	\
)	\
)	\
)

template <typename T> class AVL : public BST<T> {
public:
	BinNodePosi(T) insert(const T& e);//override
	bool remove(const T& e);//override
};

template<typename T>
BinNodePosi(T) AVL<T>::insert(const T& e)
{
	BinNodePosi(T)& x = BST<T>::search(e);	//�����в����Ƿ�������Ϊe�Ľڵ�,���ҽ�_hotָ�����ڵ�ĸ��ڵ�,��ȡ����λ��Ϊx
	if (x) return x;	//dataΪe�Ľڵ��Ѿ�������
	BinNodePosi(T) xx = x = new myBinNode<T>(e, BST<T>::_hot); BST<T>::_size++;	//��λ��x�������ڵ�
	for (BinNodePosi(T) g = BST<T>::_hot; g; g = g->parent) {	//����ڵ㷽��,Ѱ��ʧ��ڵ�
		if (!AvlBalanced(*g)) {		//ʧ������,ʹ֮����
			FromParentTo(*g) = BST<int>::rotateAt(tallerChild(tallerChild(g)));//����
			break;//һ�θ���,�ֲ������߶ȸ�ԭ,���������ȵĸ߶�Ҳ��˸�ԭ
		}
		else
			myBinTree<T>::updateHeight(g);	//���¸߶�
	}
	return xx;//�����½ڵ�λ��
}

template<typename T>
bool AVL<T>::remove(const T& e)
{
	BinNodePosi(T)& x = BST<T>::search(e);
	if (!x) return false;
	removeAt(x, BST<T>::_hot); BST<T>::_size--;
	for (BinNodePosi(T) g = BST<T>::_hot; g; g = g->parent) {//��_hotһ·����ڵ�
		if (!AvlBalanced(*g)) 
			g = FromParentTo(*g) = BST<T>::rotateAt(tallerChild(tallerChild(g)));//ʹ��_hot����ÿ���ڵ�ƽ��(ʧ�⴫���Ľڵ�Ҳ��ƽ��)
		myBinTree<T>::updateHeight(g);//���¸߶�
	}
	return true;
}
