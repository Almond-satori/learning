#include "ListNode.hpp"

template <typename T> class List {
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;//元素规模（不含哨兵），头哨兵，尾哨兵（哨兵没有任何数据，只是起判断边界的作用）

protected:
	void init();
	void copyNodes(ListNodePosi(T) p, int n);
	int clear();//清空所有结点
	
	
public:
//方便测试,将排序放在public中
	void selectionSort(ListNodePosi(T) p, int n);
	void insertionSort(ListNodePosi(T) p, int n);//对从p开始的n个元素进行排序
	void mergeSort(ListNodePosi(T) & p, int n);
	void merge(ListNodePosi(T)& p, int n, List<T>& list, ListNodePosi(T) q, int m);
//构造器
	List() { init(); }//默认构造器
	List(List<T> const& L);//整体复制链表L
	List(List<T> const& L, Rank r, int n);//复制链表L从r项开始的n项
	List(ListNodePosi(T) p, int n);//复制从位置p起的n项
//析构函数
	~List();
//只读接口
	T& operator[] (Rank r) const;
	ListNodePosi(T) first() { return header->succ; }
	ListNodePosi(T) last() { return trailer->pred; }
	ListNodePosi(T) find(T const& e) {
		find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p);
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p);//在有序链表中查找
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);//p以及p的n-1个后继中寻找max
	bool empty() { return _size > 0 ? false : true; }

//可写访问接口
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertAfter(ListNodePosi(T) p ,T const& e);
	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
	T remove(ListNodePosi(T) p);
	int deduplicate();//无序去重
	int uniquify();//有序去重
//遍历
	void traverse(void (*visit) (T&));

	template <typename VST>
	void traverse(VST&);
};
//列表初始化
template <typename T> void List<T>::init() {
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer;
	header->pred = NULL;
	trailer->pred = header;
	trailer->succ = NULL;
	_size = 0;
}

//将p链表中的n个元素拷贝到this的链表
template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{
	init();
	while (n-- > 0) { 
		insertAsLast(p->data); //由p的data创建一个新结点,插入到this的末尾
		p = p->succ;
	}
}

template<typename T>
int List<T>::clear()
{
	int oldSize = _size;
	while (_size > 0) {
		//remove会在内存中删除结点,并使得_size--
		remove(header->succ);
	}
	return oldSize;
}

//将从p开始的n个元素排序,待排序区间(head,tail)
template<typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) head = p->pred;
	ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++) tail = tail->succ;
	while (n > 1) {//n是待排序元素个数,如果n为1则不需要排序
		ListNodePosi(T) max = selectMax(head->succ, n);
		insertBefore(tail,remove(max));//在insertBefore中会根据data新建一个结点,这里需要删除结点
		tail = tail->pred;
		n--;
	}
}

template<typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
	for (int i = 0; i < n; i++) {
		//元素从小到大排序,从p开始向前寻找e,使得e是最后一个小于data的元素,将data插入到e后
		insertAfter(search(p->data, i, p), p->data);
		p = p->succ; 
		remove(p->pred);//将之前的内容删除
	}
}

template<typename T>
void List<T>::mergeSort(ListNodePosi(T) & p, int n)
{
	if (n < 2) return;//只有一个元素直接返回
	int m = n >> 1;
	ListNodePosi(T) q = p; for (int i = 0; i < m; i++)q = q->succ;//q初始化为一个指向中间元素的指针(长度为偶数时,指向偏右的中间元素)
	mergeSort(p, m);
	mergeSort(q, n - m);//递归调用,将整个链表分成一个个长度为2的子链表
	merge(p, m, *this, q, n-m);//合并子链表
}

template<typename T>
void List<T>::merge(ListNodePosi(T)&p, int n, List<T>& list, ListNodePosi(T) q, int m)
{
	ListNodePosi(T) pp = p->pred;//存储头结点的前一个结点(可能是header,也可能是不参与本轮排序的结点)
	while (m > 0) {//因为是向p中插入q中元素,当q元素用完时(m==0)才结束循环
		if ((n > 0)&&(p->data <= q->data))//在p指向的区间进行从小到大排序,如果p中元素用完,则直接去else
		{
			if (q == (p = p->succ)) break;//特殊情况,p的下一个元素就是q(q之前的元素被插入到p的链表了),这时链表已经完成,跳出循环
			//说明p区间的元素是合理的,不做修改
			n--;
		}
		else {
			//q指向的元素更小,需要插入到p的前面,注意要删除q所指向的节点
			q = q->succ;
			insertBefore(p, list.remove(q->pred));
			m--;
		}
	}
	p = pp->succ;
}

template<typename T>
List<T>::List(List<T> const& L) { copyNodes(L.first(),L._size); }

//根据下标定位元素
template<typename T>
List<T>::List(List<T> const& L, Rank r, int n){copyNodes(L[r],n);}

template<typename T>
List<T>::List(ListNodePosi(T) p, int n){copyNodes(p, n);}

template<typename T>
List<T>::~List()
{
	clear();
	delete header;
	delete trailer;
}

//效率很低的取出元素的方式
template<typename T>
inline T& List<T>::operator[](Rank r) const
{
	ListNodePosi(T) p = first();
	while (r-- > 0) p = p->succ;
	return p->data;
}

//查找
template<typename T>
inline ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p)
{
	while (n-- > 0) {
		p = p->pred;
		if (e == p->data)
			return p;
	}
	return NULL;
}

//对p前的n个结点进行比较,找到不大于e的最后值
template<typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p)
{
	while (n-- >= 0) {//这里的=是为了插入排序使用,当first元素调用search时返回header(如果返回NULL程序将出错)
		if (((p = p->pred)->data) <= e)
			return p;
	}
	return NULL;
}

template<typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) max = p;
	while (n>1) {
		//>=中的=是有意义的,我们取最后一个最大值为max
		//(试想,已经查找到了后面的最大元素,如果不更新max,下次查找仍然要寻找到后面)
		//if (!lt(p->data,max->data))
		if ((p->succ->data)>=(max->data))//test
			max = p->succ;
		p = p->succ;
		n--;
	}
	return max;
}

template<typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{
	_size++;
	return header->insertAsSucc(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e)
{
	_size++;
	return trailer->insertAsPred(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const& e)
{
	_size++;
	return p->insertAsSucc(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const& e)
{
	_size++;
	return p->insertAsPred(e);
}

//删除操作,即使p是链表中的唯一有效元素也可以正常运行,这得益于前后哨兵(哨兵不能被外界访问)
template<typename T>
T List<T>::remove(ListNodePosi(T) p)
{
	p->pred->succ=p->succ;
	p->succ->pred = p->pred;
	T e = p->data;//假设data可以直接赋值,保存这个data返回
	delete p;
	_size--;//不要忘记修改规模
	return e;
}

template<typename T>
int List<T>::deduplicate()
{
	if (_size < 2) return 0;//一个元素
	int oldSize = _size;
	ListNodePosi(T) p=header;
	int r = 0;
	while ((p=p->succ)!=trailer) {
		//第一次查出重复元素就会将其删除,所以不可能出现多个重复元素的情况
		ListNodePosi(T) q =	find(p->data, r, p);//find从本节点开始向前查找
		q ? remove(q):r++;
	}
	return oldSize - _size;//删除的元素数
}

template<typename T>
int List<T>::uniquify()
{
	if (_size < 2) return 0;//一个元素
	int oldSize = _size;
	ListNodePosi(T) p = header; ListNodePosi(T) q;
	int r = 0;
	while (trailer != (q = p->succ)) {
		if (q->data == p->data)
			remove(q);
		else
			p = q;//不相等则p指向下一个位置
	}
	return oldSize - _size;
}

template<typename T>
void List<T>::traverse(void(* visit)(T&))
{
	for (ListNodePosi(T) p = header.succ; p != trailer; p = p->succ)
		visit(p->data);
}

template<typename T>
template<typename VST>//操作器
void List<T>::traverse(VST&)
{
	for (ListNodePosi(T) p = header.succ; p != trailer; p = p->succ)
		visit(p->data);
}

//int main() {
//	List<int> list =  List<int>();
//	list.insertAsFirst(10);
//	list.insertAsFirst(1);
//	list.insertAsFirst(2);
//	list.insertAsFirst(6);
//	list.insertAsFirst(20);
//	ListNodePosi(int) x =list.insertAsFirst(25);
//	list.mergeSort(x, 6);
//}