#include "../../db7/BST.hpp"

//本节未做编译测试

#define IsBlack(p) ( !(p) || ((p)->color == RB_BLACK)) //黑节点特殊设置,没有设置黑节点的为红结点
#define IsRed(p) (!IsBlack(p)) //非黑即红
//height指黑节点的高度,需要更新的情况为,1.左右子树高度不等;2.红节点的黑高度和其孩子的不等;3.黑节点高度与其孩子高度+1不相等
#define BlackHeightUpdated(x)(\
	( (stature((x).lc)) == stature((x).rc) ) && \
	( (x).height == ( (IsRed(&x)) ? stature((x).lc) : stature((x).lc)+1 )) \
)

template <typename T> class RedBlackTree : public BST<T> {
protected:
	void solveDoubleRed(BinNodePosi(T) x);//双红修正
	void solveDoubleBlack(BinNodePosi(T) r);//双黑修正
	int updateHeight(BinNodePosi(T) x);//更新x节点高度
public:
	BinNodePosi(T) insert(const T& e);//重写插入
	bool remove(const T& e);//重写删除
};

//节点为黑,高度加1
template<typename T>
int RedBlackTree<T>::updateHeight(BinNodePosi(T) x)
{
	x->height = max(stature(x->lc), stature(x->rc));
	return IsBlack(x) ? x->height++ : x->height;
}

//在叶子节点位置上插入一个新的红结点
template<typename T>
BinNodePosi(T) RedBlackTree<T>::insert(const T& e)
{
	BinNodePosi(T)& x = search(e);//_hot指向e的父亲,x为_hot的引用(lc或者rc)
	if (x) return x;
	x = new myBinNode<T>(e,_hot,NULL,NULL,-1);//插入红结点,黑高度为-1,左右孩子为NULL故左右孩子为黑节点
	_size++;
	solveDoubleRed(x);//可能在红节点处又插入红结点,因此进行双红修正
	return x ? x : _hot->parent;
}

template<typename T>
bool RedBlackTree<T>::remove(const T& e)
{
	BinNodePosi(T)& x = search(e); if (!x) return false;
	//r为删除节点的直接后继
	BinNodePosi(T) r = removeAt(x, _hot); if (!(--_size)) return true;
	if (!_hot) {//删除的是根节点,r接替了根节点
		_root->color = RB_BLACK; updateHeight(_root); return true;
	}
	//高度不失衡
	if (BlackHeightUpdated(*_hot)) return true;
	if (IsRed(r)) {//直接后继为红色
		r->color = RB_BLACK;
		r->height++;
		return true;
	}
	solveDoubleBlack(r);
	return true;
}

//双红修正
template<typename T>
void RedBlackTree<T>::solveDoubleRed(BinNodePosi(T) x)
{
	//递归基
	if (IsRoot(*x)){
		_root->color = RB_BLACK;
		_root->height++;
		return;
	}
	BinNodePosi(T) p = x->parent;
	BinNodePosi(T) g = p->parent;
	BinNodePosi(T) u = uncle(x);
	if ( IsBlack(u) ) {//uncle为黑节点
		//染色
		if( IsLChild(*p) == IsLChild(*x) ) //父子朝向一致
			p->color = RB_BLACK;
		else //父子朝向相异
			x->color = RB_BLACK;
		g->color = RB_RED;
		BinNodePosi(T) gg = g->parent;//保存上层节点
		BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);//按中序遍历顺序对以x为孙子的子树做平衡旋转
		r->parent = gg;
	}
	else {//红节点的情况
		p->color = RB_BLACK; p->height++;//注意设置高度变化,上一种情况的高度变化在旋转函数中设置
		u->color = RB_BLACK; u->height++;
		if (!IsRoot(*g)) g->color = RB_RED;
		solveDoubleRed(g);
	}
}

template <typename T>
void RedBlackTree<T>::solveDoubleBlack(BinNodePosi(T) r) {
	BinNodePosi(T) p = r ? r->parent : _hot;
	if (!p) return;
	BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;//s是原树中x的兄弟,是现在树中r的兄弟
	if (IsBlack(s) ) {//s为黑
		BinNodePosi(T) t = NULL;//t是s的红孩子,左右都是红孩子,优先考虑左孩子
		if (IsRed(s->rc)) t = s->rc;
		if (IsRed(s->lc)) t = s->lc;
		if (t) { //s有红孩子,BB-1
			RBColor pOldColor = p->color;
			BinNodePosi(T) newRoot = FromParentTo(*p) = rotateAt(t);//rotateAt返回新的根节点
			if (newRoot->lc) { newRoot->lc->color = RB_BLACK; updateHeight(newRoot->lc); }
			if (newRoot->rc) { newRoot->rc->color = RB_BLACK; updateHeight(newRoot->rc); }
			newRoot->color = pOldColor;
			updateHeight(newRoot);
		}
		else { //s没有红孩子,BB-2
			s->color = RB_Red; s->height--;
			if (IsRed(p)) { //BB-2-R
				p->color = RB_BLACK;//s从黑染红,p从红染黑,s是p的孩子,所以p高度不变
			}
			else { //BB-2-B
				p->height--;
				solveDoubleBlack(p);
			}
		}
	}
	else {//s为红,BB-3
		s->color = RB_BLACK; p->color = RB_RED;
		BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc;//取t与s同侧
		_hot = p;//p为r的父亲
		FromParentTo(*p) = rotateAt(t);
		solveDoubleBlack(r);//转入bb-1,bb-2r 
	}

}
