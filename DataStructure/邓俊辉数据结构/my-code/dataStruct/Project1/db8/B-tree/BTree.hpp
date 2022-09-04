//B-tree 模板类
#include "BTNode.hpp"
#include "../../db1-4/release.hpp"

template <typename T> class BTree {
protected:
	int _size;	//存放的关键码key的总数
	int _order;	//B-tree的阶次,即_order路平衡搜索树,至少为3阶,创建时指定,不能修改;
	BTNodePosi(T) _root;//根节点
	BTNodePosi(T) _hot;//search最后访问的非空节点位置
	void solveOverflow(BTNodePosi(T) v);//因插入而上溢之后的分裂处理
	void solveUnderflow(BTNodePosi(T) v);//因删除而下溢之后的合并处理
public:
	//构造函数,默认为三阶
	BTree(int order = 3) : _order(order), _size(0) {_root = new BTNode<T>();}
	~BTree() { if (_root) release(_root); }//析构,释放所有节点
	int const order() { return _order; }
	int const size() { return _size; }
	BTNodePosi(T)& root() { return _root; }
	bool empty() { return !_root; }
	BTNodePosi(T) search(const T& e);
	bool insert(const T& e);
	bool remove(const T& e);
};


template<typename T>
BTNodePosi(T) BTree<T>::search(const T& e)
{
	BTNodePosi(T) v = _root; _hot = NULL;
	while (v) {//逐层查找
		Rank r = v->key.search(e); 
		if ((r >= 0) && (v->key[r] == e)) return v;
		_hot = v; v = v->child[r + 1];//转入子树,由于e大于key[r],选择支路r+1
	}
	return NULL;
}

template<typename T>
bool BTree<T>::insert(const T& e)
{
	BTNodePosi(T) v = search(e);//_hot指向最后一个节点
	if (v) return false;//如果e已经存在，失败		
	Rank r = _hot->key.search(e);//在_hot节点的关键码中搜索e(必定失败,将返回e的合适位置)
	_hot->key.insert(e, r+1);//在数组中r的位置插入e
	_hot->child.insert(NULL, r + 2);
	_size++;
	solveOverflow(_hot);
	return true;
}

//当v的节点插入一个值后,可能会超出限制
template<typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v)
{
	//未超出限制
	if (v->child.size() <= _order) return;
	Rank mid = _order / 2;
	BTNodePosi(T) right = new BTNode<T>();//新建一个右子树,用于存储分裂后的右边元素信息
	//对右子树的关键码赋值下标为i
	for (Rank i = 0; i < _order - mid -1; i++)
	{
		right->key.insert(v->key.remove(mid + 1),i);//remove删除位置在mid的后一位,删除一次都要移动原数组,比较耗时
		right->child.insert(v->child.remove(mid + 1),i);//从元素左子树开始插入,
	}
	right->child[_order - mid - 1] = v->child.remove(mid + 1);//最后一个右子树手动插入
	//将分裂出来的右子树的子节点的parent赋值
	if (right->child[0])
		for (int i = 0; i < _order - mid; i++)
			right->child[i]->parent = right;
	BTNodePosi(T) p = v->parent;//为分裂前的节点v的父亲
	//如果是根节点分裂,则需要新建一个根节点
	if (!p) {
		p=_root = new BTNode<T>();
		_root->child[0] = v;
		v->parent = p;
	}
	//在父亲节点中寻找关键码的位置
	Rank r = 1 + p->key.search(v->key[0]);
	p->key.insert(v->key.remove(mid), r);
	p->child.insert(right, r+1);
	right->parent = p;
	solveOverflow(p);//父节点在插入后可能也上溢了,需要递归解决
}

template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v)
{
	//满足b-tree边数量的下限,直接返回
	if (v->child.size() >= (_order + 1) / 2) return;
	BTNodePosi(T) p = v->parent;
	if (!p) {//v是根节点的情况
		//根节点的关键码数量大于等于1即可
		if( !v->key.size() && v->child[0] ){//当根节点已经没有关键码,但有child时
			_root = v->child[0];
			_root->parent = NULL;
			v->child = NULL; release(v);
		}
		return;
	}
	//v非根节点
	Rank r = 0; 
	while (p->child[r] != v) r++;//找到对应v节点的p的child的下标
	//case1,向左兄弟借关键码
	if (r > 0) {
		BTNodePosi(T) ls = p->child[r - 1];
		if (ls->child.size() > (_order + 1) / 2) {//查看左兄弟的关键码数量是否够
			v->key.insert(p->key[r - 1],0);//v从p借一个元素
			p->key[r - 1] = ls->key.remove(ls->key.size()-1);//p从ls借一个元素
			v->child.insert(ls->child.remove(ls->child.size()-1),0);//ls少了一个元素,将其对应的右子树(可能为NULL)给v
			if (v->child[0])
				v->child[0]->parent = v;
		return;
		}
	}
	//case2,向右兄弟借关键码
	if (r < p->child.size() - 1) {
		BTNodePosi(T) rs = p->child[r+1];
		if (rs->child.size() > (_order + 1) / 2) {
			v->key.insert( p->key[r] , v->key.size() );//注意这里不要删除p中的元素,而是先拿过来
			p->key[r] = rs->key.remove(0);//这里直接对p中元素赋值,不需要插入
			v->child.insert(rs->child.remove(0), v->child.size());
			if (v->child[v->child.size() - 1]) {
				v->child[v->child.size() - 1]->parent = v;
			}
			return;
		}
	}
	//case3 左右兄弟要么为空,要么空间不足以借给v关键码,将左右节点以及p的一个元素合并为一个节点
	//case 3.1
	if (r > 0) {
		BTNodePosi(T) ls = p->child[r - 1];
		//合并p中元素
		ls->key.insert(p->key.remove(r - 1), ls->key.size());//直接在p中删除了这个关键码
		p->child.remove(r);//将关键码右子树删除
		//合并v中元素
		ls->child.insert(v->child.remove(0), ls->child.size());
		if (ls->child[ls->child.size()-1])
			ls->child[ls->child.size()-1]->parent = ls;
		while (!v->key.empty()) {
			ls->key.insert(v->key.remove(0), ls->key.size());
			ls->child.insert(v->child.remove(0), ls->child.size());
			if (ls->child[ls->child.size() - 1])
				ls->child[ls->child.size() - 1]->parent = ls;
		}
		release(v);
	}
	else {//case 3.2 
		BTNodePosi(T) rs = p->child[r + 1];
		//合并p中元素
		rs->key.insert(p->key.remove(r),0);//直接在p中删除了这个关键码
		p->child.remove(r);//将关键码左子树删除
		//合并v中元素
		rs->child.insert(v->child.remove(v->child.size()-1),0);
		if (rs->child[0])
			rs->child[0]->parent = rs;
		while (!v->key.empty()) {
			rs->key.insert(v->key.remove(v->key.size()-1), 0);
			rs->child.insert(v->child.remove(v->child.size() - 1),0);
			if (rs->child[0])
				rs->child[0]->parent = rs;
		}
		release(v);
	}
	solveUnderflow(p);
	return;
}

template <typename T>
bool BTree<T>::remove(const T& e) {
	BTNodePosi(T) v = search(e);//定位到节点
	if (!v) return false;
	Rank r = v->key.search(e);//定位到关键码
	if (v->child[0]) {//如果v不是叶子节点(B-tree的child是从左往右依次填充的,child[1]必须在child[0]存在的情况下才能填充)
		//我们需要找到删除节点中序遍历的下一位置
		BTNodePosi(T) u = v->child[r + 1];//获取右子树
		while (u->child[0]) u = u->child[0];//在右子树中尽可能向左(这与二叉树有所不同,因为右子树依赖于左子树,没有左子树就没有右子树)
		//u为右子树的最左节点
		v->key[r] = u->key[0];
		//将v作为待删除节点,r为待删除关键码的秩
		v = u;
		r = 0;
	}
	//v是叶子,直接删除;v不是叶子,在交换数据后删除v直接后继的节点
	v->key.remove(r);
	v->child.remove(r + 1);//虽然数组元素是空,但是也要删除,使得child数组大小正确
	_size--;
	solveUnderflow(v);//解决删除节点的下溢问题
	return true;
}


