#include "../../db7/BST.hpp"

template <typename T> class Splay : public BST<T> {
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v);//将节点v延伸至树根
public:
	BinNodePosi(T)& search(const T& e);//查找(会引起结构改变)
	BinNodePosi(T) insert(const T& e);//插入
	bool remove(const T& e);//删除
};

template <typename NodePosi> inline
void attachAsLChild(NodePosi p, NodePosi lc) {//NodePosi是myBinNode<T>*
	p->lc = lc; if (lc) lc->parent = p;
}

template <typename NodePosi> inline
void attachAsRChild(NodePosi p, NodePosi rc) {//NodePosi是myBinNode<T>*
	p->rc = rc; if (rc) rc->parent = p;
}

template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
	if (!v) return NULL;
	BinNodePosi(T) p; BinNodePosi(T) g;
	while ( (p = v->parent) && (g = p->parent) ) {
		BinNodePosi(T) gg = g->parent;//保存上层节点
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
		//处理上层节点gg
		if (!gg) v->parent = NULL;//v是根节点
		else
			(g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);//按照原来的方式接入gg
		myBinTree<T>::updateHeight(v); myBinTree<T>::updateHeight(p); myBinTree<T>::updateHeight(g);
	}//while
	//v一次上升两层,当层数为偶数(层数从1计),此时v->parent是root,需要再手动上升一层;如果层数为奇数,v是root
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
	//退化情况,如果树中没有元素
	if (!myBinTree<T>::_root) { myBinTree<T>::_size++; return myBinTree<T>::_root = new myBinNode<T>(e); }
	//如果树中已经有元素e
	if ((search(e)->data) == e) return myBinTree<T>::_root;
	//否则,最接近e的元素_hot被设置为根节点
	BinNodePosi(T) t = myBinTree<T>::_root;
	myBinTree<T>::_size++;
	//情况1:t的右子树>e>t(search中能选出t,就蕴含了e<t的右子树的条件)
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
	BinNodePosi(T) v = myBinTree<T>::_root;//待删除节点已经放到_root位置
	//仅含有一边子树,直接删除v
	if (!HasLChild(*v)) {
		myBinTree<T>::_root = myBinTree<T>::_root->rc; 
		if(myBinTree<T>::_root) myBinTree<T>::_root->parent = NULL;
	}
	else if (!HasRChild(*v)) {
		myBinTree<T>::_root = myBinTree<T>::_root->lc;
		if (myBinTree<T>::_root) myBinTree<T>::_root->parent = NULL;
	}
	else {//左右子树都有
		BinNodePosi(T) lTree = myBinTree<T>::_root->lc;
		myBinTree<T>::_root->lc = NULL;
		lTree->parent = NULL;//断开左子树
		myBinTree<T>::_root = myBinTree<T>::_root->rc; myBinTree<T>::_root->parent = NULL;
		search(e);//在右子树中寻找最接近e的节点(这次查找一定失败,但会将大于e的最小值放在根节点上)
		myBinTree<T>::_root->lc = lTree; myBinTree<T>::_root->parent = NULL; 
	}
	release(v->data); release(v); myBinTree<T>::_size--;//释放节点
	if (myBinTree<T>::_root) myBinTree<T>::updateHeight(myBinTree<T>::_root);
	return true;
}
