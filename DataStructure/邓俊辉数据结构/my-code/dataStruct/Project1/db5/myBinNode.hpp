#include <iostream>
#include "../db1-4/4-1-stack.hpp"
#include "../db1-4/4-2-queue.hpp"

#define BinNodePosi(T) myBinNode<T>*
#define stature(p) ( (p) ? (p)->height : -1 )//节点高度,定义空树高度为-1,仅有一个根节点的数高度为0

//myBinNode的常用状态判断,宏实现,x是myBinNode<T>
#define IsRoot(x) ( ! ( (x).parent ) )
#define IsLChild(x) ( !IsRoot(x) && ( (x).parent->lc == &(x) ))
#define IsRChild(x) ( !IsRoot(x) && ( (x).parent->rc == &(x) ))
#define HasParent(x) ( !IsRoot(x) )
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x) )
#define HasBothChild(x) (HasLChild(x) && HasRChild(x) )
#define IsLeaf(x) ( ! HasChild(x) )
//与myBinNode有特殊关系的节点,注意p是引用myBinNode<T> *
//兄弟节点
#define sibling(p) \
	(IsLChild( * (p) ) ? (p).parent->rc : (p).parent->lc)
//叔叔节点p为BinNodePosi(T)
#define uncle(p) \
	(IsLChild(  *((p)->parent) ) ? (p)->parent->parent->rc : (p)->parent->parent->lc )
//来自父节点的引用,这里是拿到父节点的lc或者rc
#define FromParentTo(x) \
	( IsRoot(x) ? myBinTree<T>::_root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )

typedef enum{RB_RED,RB_BLACK} RBColor;//节点颜色(红黑树)

template <typename T> struct myBinNode {//二叉树节点
public://为了简化,省略封装
//成员
	T data;//数据
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//父节点,左右孩子
	int height;//高度
	int npl;//Null Path Length 左式堆 也可以用height代替
	RBColor color;//红黑树的颜色
//构造函数
	//对仅有根节点的树的构造
	myBinNode() :
		parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
	//一般的传参构造
	myBinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, 
		int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}
//操作接口
	int size();//统计当前节点后代总数
	BinNodePosi(T) insertAsLC(T const& e) { return lc = new myBinNode(e, this); }
	BinNodePosi(T) insertAsRC(T const& e) { return rc = new myBinNode(e, this); }
	BinNodePosi(T) succ();//取出当前节点的直接后继
	template <typename VST> void travLevel(VST& visit);//层次遍历
	template <typename VST> void travPre(BinNodePosi(T) x,VST& visit);//先序遍历
	template <typename VST> void travIn(BinNodePosi(T) x, VST& visit);//中序遍历
	template <typename VST> void travPost(BinNodePosi(T) x,VST& visit);//后续遍历
//比较器
	bool operator< (myBinNode const& bn) { return data < bn.data; }
	bool operator> (myBinNode const& bn) { return data > bn.data; }
	bool operator== (myBinNode const& bn) { return data == bn.data; }
	bool operator>= (myBinNode const& bn) { return data >= bn.data; }
	bool operator<= (myBinNode const& bn) { return data <= bn.data; }
};

////中序遍历选择
//template<typename T> template<typename VST>
//void myBinNode<T>::travIn(VST& visit)
//{
//	switch (1)
//	{
//	case 1: travIn_1(this, visit); break;
//	case 2: travIn_2(this, visit); break;
//	case 3: travIn_3(this, visit); break;
//	case 4: travIn_4(this, visit); break;
//	default: travIn_R(this, visit);break;//递归版
//	}
//}

template<typename T> template<typename VST>
void myBinNode<T>::travPre(BinNodePosi(T) x, VST& visit)
{	
	myStack<BinNodePosi(T)> stack;//存放节点
	while (true) {
		visitAlongLeftBranch(x,visit,stack);//退出函数时到达左分支的边界,x=NULL
		if (stack.empty()) break;//栈空结束
		x = stack.pop();//将最后一个右节点弹出,以这个右节点为根,继续操作
	}
}


template<typename T , typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) & x, VST& visit, myStack<BinNodePosi(T)>& stack){
	while (x) {//向左遍历到无左孩子的节点
		visit(x->data);//第一次循环,访问这棵树或者子树的根节点,后面的循环都是向左访问
		stack.push(x->rc);//将右节点入栈
		x = x->lc;
	}
}

template<typename T> template<typename VST>
void myBinNode<T>::travIn(BinNodePosi(T) x, VST& visit)
{
	myStack<BinNodePosi(T)> stack;
	while (true) {
		goAlongLeftBranch(x, visit, stack);//返回时已经x到了最左边的节点的下一个左节点(NULL)
		if (stack.empty()) break;//循环终止条件
		x = stack.pop();//弹出边界的左节点
		visit(x->data);
		x = x->rc;//访问右子树
	}
}

//向左访问,直到左节点为空
template<typename T, typename VST>
static void goAlongLeftBranch(BinNodePosi(T)& x, VST& visit, myStack<BinNodePosi(T)>& stack) {
	while (x) {//向左遍历到无左孩子的节点
		stack.push(x);//将沿路的节点入栈
		x = x->lc;
	}
}

template<typename T>
BinNodePosi(T) myBinNode<T>::succ()//中序遍历的下一位置
{
	BinNodePosi(T) s = this;
	if (s->rc) {//如果有右子树,则下一次访问的节点一定在右子树中(左->根->右,如果有右节点,说明现在访问的节点是根节点)
		s = rc;
		while (HasLChild(*s)) s = s->lc;
	}
	else {//如果不能继续向右访问,理解为一次子树的访问结束了,返回到下一次的左节点上
		while (IsRChild(*s)) s = s->parent;//向前返回到本次子树的根节点
		s = s->parent;//在本次子树的根节点向上返回(本次根节点是左孩子,一定是向右上返回)
	}
	return s;
}

template<typename T> template<typename VST>
void myBinNode<T>::travPost(BinNodePosi(T) x, VST& visit)
{
	myStack<BinNodePosi(T)> stack;
	stack.push(x);
	while (!stack.empty()) {
		if (stack.top() != x->parent) //栈顶为下一次的返回位置,当右子树的根节点被访问(判断不成立),才向上返回,否则以栈顶(右节点,即右子树的根)为新的根
			gotoHLVFL(stack);//找到子树的水平最左位置,并将沿路的左右子树入栈,先入右节点,后入左节点(左节点可以看为起点到水平最左位置的路径)
		x = stack.pop(); visit(x->data);
	}
}

template <typename T>
void gotoHLVFL(myStack<BinNodePosi(T)>& stack) {
	while (BinNodePosi(T) x = stack.top() ) {
		if (HasLChild(*x)) {//优先向左遍历
			if (HasRChild(*x)) stack.push(x->rc);
			stack.push(x->lc);
		}
		else {//没有左节点,才向右
			stack.push(x->rc);
		}
	}
	stack.pop();//弹出最后入栈的NULL
}

template<typename T> template<typename VST>
void myBinNode<T>::travLevel(VST& visit)
{
	myQueue<BinNodePosi(T)> queue;
	queue.enqueue(this);
	while (!queue.empty()) {
		cout<< queue.front()->data << endl;
		BinNodePosi(T) x = queue.dequeue(); visit(x->data);
		if ( HasLChild(*x) ) queue.enqueue(x->lc);
		if ( HasRChild(*x) ) queue.enqueue(x->rc);
	}
}
