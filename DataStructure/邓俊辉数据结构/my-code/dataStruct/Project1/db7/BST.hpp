#include "../db5/myBinTree.hpp"
/*二叉搜索树模板*/

template <typename T> class BST : public myBinTree<T> {
protected:
	BinNodePosi(T) _hot;	//命中节点的父亲
	BinNodePosi(T) connect34(	//按照3+4结构,联结3个节点以及4棵子树
		BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, 
		BinNodePosi(T) T0 , BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3
	);
	BinNodePosi(T) rotateAt(BinNodePosi(T) v);	//对x以及其父亲,祖父做旋转调整
public:
	virtual BinNodePosi(T)& search(const T& e);	//查找
	virtual BinNodePosi(T) insert(const T& e);	//插入
	virtual bool remove(const T& e);	//删除
	 
};

//3个节点4棵子树,按中序遍历的顺序连接成一棵树,该树的根节点与上层的连接由调用者解决
template<typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, 
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
{
	//按照T0 a T1 b T2 c T3中序遍历的顺序连接成一棵树
	a->lc = T0; if (T0) T0->parent = a;
	a->rc = T1; if (T1) T1->parent = a;
	myBinTree<T>::updateHeight(a);
	c->lc = T2; if (T2) T2->parent = c;
	c->rc = T3; if (T3) T3->parent = c;
	myBinTree<T>::updateHeight(c);
	b->lc = a; a->parent = b;
	b->rc = c; c->parent = b;
	myBinTree<T>::updateHeight(b);
	return b;//将新树的根节点返回
}

//节点关系g->p->v
template<typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent;
	//根据三个节点的四种位置关系,利用connect34函数建立子树
	if (IsLChild(*p)) {
		if (IsLChild(*v)) {
			p->parent = g->parent; //新树的根节点向上连接
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

/*在以v为根节点的BST子树中查找关键码e*/
template<typename T>
static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) {
	if (!v || (v->data == e)) return v;	//递归出口: 本轮根节点为NULL 或者 找到对应的节点
	hot = v;	//需要记录当前节点,hot即类中的_hot,定义为指向查找元素的父节点(即使查找失败,或者仅有一个根节点的情况也满足语义)
	return searchIn(((e < v->data) ? v->lc : v->rc), e, hot);
}

template<typename T>
BinNodePosi(T) BST<T>::insert(const T& e)
{
	BinNodePosi(T)& x = search(e);	//在树中查找是否有数据为e的节点,并且将_hot指向最后节点的父节点
	if (x) return x;	//data为e的节点已经在树中
	x = new myBinNode<T>(e, _hot);		//search函数中将_hot->rc或者_hot->lc返回给x,所以对x赋值就直接接在树上了
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
	BinNodePosi(T) del = x;	//del是真正被删除的节点
	BinNodePosi(T) succ = NULL;
	if (!HasLChild(*x))		//x左子树为空,直接将x替换为右子树,通过del可删除原节点
		succ = x = x->rc;
	else if (!HasRChild(*x))		//x右子树为空,直接将x替换为左子树
		succ = x = x->lc;
	else {	//左右子树都有,则将该节点和其直接后继换位置
		del = del->succ();	//中序遍历的下一位置
		swap(del->data, x->data);	//交换数据后,我们要删除的节点就是del了
		BinNodePosi(T) delParent = del->parent;
		//根据中序遍历,有待删除节点a和其直接后继b,
		//要么b是a的右节点且b仅有右子树,
		//要么b在a的右子树中而且不是a的右节点,此时b没有左节点(否则b的左节点才是a的直接后继)
		//综上,b只可能有右子树,根据上述两种情况,保存del的右子树
		if (delParent == x)
			delParent->rc = succ = del->rc;	//b是其parent的rc
		else
			delParent->lc = succ = del->rc;	//b是其parent的lc
	}
	hot = del->parent;//hot为删除节点的父亲
	if (succ) succ->parent = hot;		//将原来的后继和删除节点的父节点相接
	release(del->data); release(del);
	return succ;	//返回接替者
}