#include "myBinNode.hpp"
#include "../db1-4/release.hpp"
using namespace std;

//说明:remove中的release无法实现,因此析构函数,remove,removeAt函数已被注释掉!


template <typename T> class myBinTree {
protected:
	int _size;//二叉树规模
	BinNodePosi(T) _root;//根节点索引
	virtual int updateHeight(BinNodePosi(T) x);//更新x节点的高度
	void updateHeightAbove(BinNodePosi(T) x);//更新x节点以及x祖先的节点高度
public:
//构造析构
	myBinTree() : _size(0), _root(NULL){}//构造函数
	~myBinTree() { if (_size > 0) remove(_root); }//析构函数
//只读函数
	int size() const { return _size; }
	bool empty() const { return !_root; }
	BinNodePosi(T) root() const { return _root; }//获取根节点索引
//插入
	BinNodePosi(T) insertAsRoot(T const& e);//插入根节点
	//数据e作为x节点的左右孩子插入
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
	//T作为x的左右子树接入
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x,  myBinTree<T>* & S);
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, myBinTree<T>* & S);//直接使用引用,因为需要在函数内部释放这个指针
	int remove(BinNodePosi(T) x);//删除节点x以及以x为根的子树
	myBinTree<T>* secede(BinNodePosi(T) x);//将子树x从当前树中删除,转换为一棵独立子树
	
//操作器,遍历对每个元素执行visit函数
	//层序遍历
	template <typename VST> void travLevel(VST& visit) { if (_root) _root->travLevel(visit); }
	//先序遍历
	template <typename VST> void travPre(VST& visit) { if (_root) _root->travPre(_root,visit); }
	//中序遍历
	template <typename VST> void travIn(VST& visit) { if (_root) _root->travIn(_root, visit); }
	//后序遍历
	template <typename VST> void travPost(VST& visit) { if (_root) _root->travPost(_root, visit); }

//比较器
	bool operator== (myBinTree<T> const& t) { return _root && t._root && (_root == t._root); }
};

//高度的定义可能有所不同,可以根据情况重载该函数
template<typename T>
int myBinTree<T>::updateHeight(BinNodePosi(T) x)
{
	//x=NULL null pointer
	if(x!=NULL)
		return x->height = 1 + max(stature(x->lc), stature(x->rc));
	return -1;//空树高度为-1
	
}

template<typename T>
void myBinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x) {
		updateHeight(x);
		x = x->parent;
	}
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsRoot(T const& e)
{
	_size = 1;
	 _root = new myBinNode<T> (e);
	 BinNodePosi(T) r = _root;
	 return r;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsLC(e);//x节点的函数
	updateHeightAbove(x);//在x处插入节点,x以及x的祖先的高度都要改变
	return x->lc;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsRC(e);//x节点的函数
	updateHeightAbove(x);//在x处插入节点,x以及x的祖先的高度都要改变
	return x->rc;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::attachAsLC(BinNodePosi(T) x, myBinTree<T>*& S)
{
	if (x->lc = S->_root) x->lc->parent = x;
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL; 
	S->_size = 0; 
	release(S); S = NULL; //release函数可以释放资源
	return x;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsRC(BinNodePosi(T) x, myBinTree<T>*& S)
{
	if (x->rc = S->_root) x->rc->parent = x;
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL;
	S->_size = 0;
	release(S); S = NULL;
	return x;
}

template<typename T>
int myBinTree<T>::remove( BinNodePosi(T) p )
{
	//p是指针
	myBinNode<T> x = *p;
	FromParentTo(x) =NULL;//切断父节点到x的引用
	updateHeight(p->parent); //如果是根节点,则传入了NULL
	int n = removeAt(p); _size -= n; return n;
}

template<typename T> 
static int removeAt(BinNodePosi(T) x) {
	if (!x) return 0;
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);
	release(x->data); release(x); return n;//释放被摘除节点
}

template<typename T>
myBinTree<T>* myBinTree<T>::secede(BinNodePosi(T) x)
{
	//x是指针
	FromParentTo(*x) = NULL;//切断父节点到x的引用
	updateHeightAbove(x->parent);
	myBinTree<T>* S = new myBinTree<T>;
	S->_root = x;
	x->parent = NULL;//注意根节点需要将parent设置为空
	S->_size = x->size();
	_size -= S->_size;
	return S;
}

template <typename T>
void visit(T data) {
	cout << data << endl;
}
