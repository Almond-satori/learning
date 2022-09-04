#include <iostream>
using namespace std;

//ListNodeģ����
typedef int Rank;
#define ListNodePosi(T) ListNode<T>*

template <typename T> struct ListNode {
//��Ա
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ;
//���캯��
	ListNode(){}
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL) {
		data = e;
		pred = p;
		succ = s;
	}
		
//�����ӿ�
	ListNodePosi(T) insertAsPred(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);

//ֻ���ӿ�
	
};
//ǰ����
template<typename T>
inline ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
	ListNodePosi(T) x = new ListNode(e,pred,this);
	pred->succ = x;
	this->pred = x;
	return x;
}

//�����
template<typename T>
inline ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, this, succ);
	this->succ->pred = x;
	this->succ = x;
	return x;
}

