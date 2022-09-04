#include "BST.hpp"

// x 是 myBinNode<T> 类型
#define Balanced(x) ( (stature((x).lc) ) == ( stature((x).rc)) )
#define BalFac(x) ( stature((x).lc) - stature((x).rc) )
#define AvlBalanced(x) ( (BalFac(x) > -2) && (BalFac(x) < 2) )

// p 是 myBinNode<T> * 类型,即BinNodePosi(T)
//在左右子树中取最高者,若等高,则取与父亲同侧者
#define tallerChild(p) (	/*左高*/ \
	stature((p)->lc) > stature((p)->rc) ? (p)->lc : ( /*右高*/ \
	stature((p)->rc) > stature((p)->lc) ? (p)->rc : ( /*等高,选择与父亲同侧(父节点是左节点,就选左节点)*/ \
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
	BinNodePosi(T)& x = BST<T>::search(e);	//在树中查找是否有数据为e的节点,并且将_hot指向最后节点的父节点,获取插入位置为x
	if (x) return x;	//data为e的节点已经在树中
	BinNodePosi(T) xx = x = new myBinNode<T>(e, BST<T>::_hot); BST<T>::_size++;	//在位置x处建立节点
	for (BinNodePosi(T) g = BST<T>::_hot; g; g = g->parent) {	//向根节点方向,寻找失衡节点
		if (!AvlBalanced(*g)) {		//失衡的情况,使之复衡
			FromParentTo(*g) = BST<int>::rotateAt(tallerChild(tallerChild(g)));//复衡
			break;//一次复衡,局部子树高度复原,再往上祖先的高度也因此复原
		}
		else
			myBinTree<T>::updateHeight(g);	//更新高度
	}
	return xx;//返回新节点位置
}

template<typename T>
bool AVL<T>::remove(const T& e)
{
	BinNodePosi(T)& x = BST<T>::search(e);
	if (!x) return false;
	removeAt(x, BST<T>::_hot); BST<T>::_size--;
	for (BinNodePosi(T) g = BST<T>::_hot; g; g = g->parent) {//从_hot一路向根节点
		if (!AvlBalanced(*g)) 
			g = FromParentTo(*g) = BST<T>::rotateAt(tallerChild(tallerChild(g)));//使得_hot以上每个节点平衡(失衡传播的节点也会平衡)
		myBinTree<T>::updateHeight(g);//更新高度
	}
	return true;
}
