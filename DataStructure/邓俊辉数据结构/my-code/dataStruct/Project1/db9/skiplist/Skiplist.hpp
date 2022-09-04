#include "../../db1-4/List.hpp"
#include "../Entry.hpp"
#include "../QuadList.h"
//skiplist未测试

template <typename K,typename V>
class Skiplist : public Dictionary<K, V>, public List< Quadlist<Entry<K, V>>* > {
protected:
	bool skipSearch(ListNode< Quadlist<Entry<K, V>>* >*& qlist,
		QuadlistNode<Entry<K, V>>*& p,
		K& k);
public:
	int size() const { return empty() ? 0 : last()->data->size(); }
	int level() { return List::size(); }
	bool put(K k, V v);
	V* get(K k);
	bool remove(K k);
};

//qlist为顶层列表,p为qlist的首个节点,k为关键码
//出口:若成功,p为命中关键码的顶部节点,qlist为p所在的列表
		//若失败,p为不大于k的最大(最靠右)关键码的最底层,qlist为空
//多个命中取四联表中最靠后者
template<typename K, typename V>
bool Skiplist<K, V>::skipSearch(
	ListNode<Quadlist<Entry<K, V>>*>* &qlist,
	QuadlistNode<Entry<K, V>>* &p,
	K& k ) {
	//注意:链表有首尾哨兵,quadlist同样有首尾哨兵,注意边界处理
	while (true) {
		//向后查找到第一个大于k的位置
		while (p->succ && (p->entry.data <= k))
			p = p->succ;
		p = p->pred;//回退一步
		//防止p为边界节点,将其中的NULL(0)和数据比较,因此p必须有前驱(哨兵不是NULL)
		if (p->pred && (p->entry.key == k)) return true;
		qlist = qlist->succ;//进入下一层
		if (!qlist->succ) return false;//p为链表的trailer,没有下一个元素,查找失败
		p = (p->pred) ? p->below : qlist->data.first();//p是真实数据节点时,必有below节点;p为哨兵时则没有below,只能通过qlist寻找下一个节点
	}
}



template<typename K, typename V>
V* Skiplist<K, V>::get(K k)
{
	if (empty()) return NULL;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
	Quadlist<Entry<K, V>>* p = qlist->data->first();
	return skipSearch(qlist, p, k) ? &(p->entry.value) : NULL;
}

template<typename K, typename V>
bool Skiplist<K, V>::remove(K k)
{
	if (empty()) return false;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();
	if (!skipSearch(qlist, p, k)) return false;
	do {
		QuadlistNode<Entry<K, V>>* b=p->below;
		qlist->data->remove(p);//quadlist中删除quadlistNode节点p
		//转入下层
		p = b;
		qlist = qlist->succ;
	} while (qlist->succ);
	//删除完成后,高度可能会下降,自顶向下查看每层是否有元素
	while (!empty() && first()->data->empty())//查看本行Quadlist是否为空,删除总发生在first行
		List::remove(first());
	return true;
}

//插入entry(k,v)
template<typename K, typename V>
bool Skiplist<K, V>::put(K k, V v)
{
	Entry<K, V> e = Entry<K, V>(k, v);
	//如果链表为空,先新插入一行
	if (empty()) insertAsFirst(new Quadlist<Entry<K,V>>);
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();
	if (skipSearch(qlist, p, k))//查找是否有k的元素
		while (p->below) p = p->below;//已经有关键码k,转到k的塔底
	qlist = last();//转到最底
	QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove(e, p);//在p后插入元素e,此处为新建塔的基座
	while (rand() % 1) {//新建塔
		//为了确定新建楼层已存在,在该楼层中寻找节点
		//向前寻找,不低于此楼层的最近前驱(这个前驱有上层),能找到就直接创建本楼层节点,否则创建本层后再创建节点
		while (qlist->data.valid(p) && !p->above) p = p->pred;
		if (!qlist->data.valid(p)) {//若前驱是header哨兵
			if (qlist == first())
				insertAsFirst(new Quadlist<Entry<K,V>>);//创建新的一层
			p = qlist->pred->data->first()->pred;//p为上层的header
		}
		else {
			p = p->above;
		}
		qlist = qlist->pred;
		b = qlist->data->insertAfterAbove(e, p, b);
	}
}
