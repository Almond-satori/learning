#include "List.hpp"

template <typename T>
class myQueue :public List<T> {
public:
	void enqueue(T const& e) { List<T>::insertAsLast(e); }
	T dequeue() { return List<T>::remove(List<T>::first()); }
	T& front() {//����ֵΪT&,Ҫ֪�����ʽ��:int i=10;int&  r = i;�ǺϷ���,�����r��ֵΪ10,�����ǵ�ַ,���޸�r�ᵼ��i���޸�
		ListNodePosi(T) r =List<T>::first();//r��������ListNode<myBinNode<int> *>
		T res = r->data;//�����r->data��ListNode�еĽڵ�����data(����ΪmyBinNode<int> *)
		 return res;//res��myBinNode<int> *����,res->data��myQueue<int>�е�data,��int����
	}//����data�ĵ�ַ
};

