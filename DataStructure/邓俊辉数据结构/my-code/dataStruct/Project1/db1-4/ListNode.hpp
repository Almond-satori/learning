#include <iostream>
using namespace std;

//ListNode模板类
typedef int Rank;
#define ListNodePosi(T) ListNode<T>*

template <typename T> struct ListNode {
//成员
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ;
//构造函数
	ListNode(){}
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL) {
		data = e;
		pred = p;
		succ = s;
	}
		
//操作接口
	ListNodePosi(T) insertAsPred(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);

//只读接口
	
};
//前插入
template<typename T>
inline ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
	ListNodePosi(T) x = new ListNode(e,pred,this);
	pred->succ = x;
	this->pred = x;
	return x;
}

//后插入
template<typename T>
inline ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, this, succ);
	this->succ->pred = x;
	this->succ = x;
	return x;
}

