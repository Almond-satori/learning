#include "Entry.hpp"
#define QlistNodePosi(T) QuadlistNode<T>*

template <typename T> struct QuadlistNode
{
	T entry; //�����洢�Ĵ���
	QlistNodePosi(T) pred; QlistNodePosi(T) succ; //ǰ�����
	QlistNodePosi(T) above; QlistNodePosi(T) below; //��������
	QuadlistNode(T e = T(), QlistNodePosi(T) p = NULL, QlistNodePosi(T) s = NULL,
		QlistNodePosi(T) a = NULL, QlistNodePosi(T) b = NULL)
		: entry(e), pred(p), succ(s), above(a), below(b) {}
	//�����½ڵ�,�Ե�ǰ�ڵ�Ϊǰ��,�Խڵ�bΪ����
	QlistNodePosi(T) insertAsSuccAbove(T const& e, QlistNodePosi(T) b = NULL);

};

template<typename T>
QlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(T const& e, QlistNodePosi(T) b)
{
	//�Ե��ýڵ�Ϊǰ��,�Ե��ýڵ�ĺ��Ϊ���,û����ָ��,��ָ��Ϊb
	QlistNodePosi(T) x = new QlistNodePosi<T>(e, this, succ, NULL, b);
	succ->pred = x;
	succ = x;
	if (b) b->above = x;
	return x;//���½ڵ�λ�÷���
}
