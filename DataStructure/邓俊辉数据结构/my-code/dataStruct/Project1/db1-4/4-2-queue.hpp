#include "List.hpp"

template <typename T>
class myQueue :public List<T> {
public:
	void enqueue(T const& e) { List<T>::insertAsLast(e); }
	T dequeue() { return List<T>::remove(List<T>::first()); }
	T& front() {//返回值为T&,要知道这个式子:int i=10;int&  r = i;是合法的,且输出r的值为10,并不是地址,但修改r会导致i被修改
		ListNodePosi(T) r =List<T>::first();//r的类型是ListNode<myBinNode<int> *>
		T res = r->data;//这里的r->data是ListNode中的节点数据data(类型为myBinNode<int> *)
		 return res;//res是myBinNode<int> *类型,res->data是myQueue<int>中的data,是int类型
	}//返回data的地址
};

