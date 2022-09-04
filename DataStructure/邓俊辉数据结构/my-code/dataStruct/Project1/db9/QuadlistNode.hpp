#include "Entry.hpp"
#define QlistNodePosi(T) QuadlistNode<T>*

template <typename T> struct QuadlistNode
{
	T entry; //本结点存储的词条
	QlistNodePosi(T) pred; QlistNodePosi(T) succ; //前驱后继
	QlistNodePosi(T) above; QlistNodePosi(T) below; //上邻下邻
	QuadlistNode(T e = T(), QlistNodePosi(T) p = NULL, QlistNodePosi(T) s = NULL,
		QlistNodePosi(T) a = NULL, QlistNodePosi(T) b = NULL)
		: entry(e), pred(p), succ(s), above(a), below(b) {}
	//插入新节点,以当前节点为前驱,以节点b为下邻
	QlistNodePosi(T) insertAsSuccAbove(T const& e, QlistNodePosi(T) b = NULL);

};

template<typename T>
QlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(T const& e, QlistNodePosi(T) b)
{
	//以调用节点为前驱,以调用节点的后继为后继,没有上指针,下指针为b
	QlistNodePosi(T) x = new QlistNodePosi<T>(e, this, succ, NULL, b);
	succ->pred = x;
	succ = x;
	if (b) b->above = x;
	return x;//将新节点位置返回
}
