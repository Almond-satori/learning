#include "ListNode.hpp"

template <typename T> class List {
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;//Ԫ�ع�ģ�������ڱ�����ͷ�ڱ���β�ڱ����ڱ�û���κ����ݣ�ֻ�����жϱ߽�����ã�

protected:
	void init();
	void copyNodes(ListNodePosi(T) p, int n);
	int clear();//������н��
	
	
public:
//�������,���������public��
	void selectionSort(ListNodePosi(T) p, int n);
	void insertionSort(ListNodePosi(T) p, int n);//�Դ�p��ʼ��n��Ԫ�ؽ�������
	void mergeSort(ListNodePosi(T) & p, int n);
	void merge(ListNodePosi(T)& p, int n, List<T>& list, ListNodePosi(T) q, int m);
//������
	List() { init(); }//Ĭ�Ϲ�����
	List(List<T> const& L);//���帴������L
	List(List<T> const& L, Rank r, int n);//��������L��r�ʼ��n��
	List(ListNodePosi(T) p, int n);//���ƴ�λ��p���n��
//��������
	~List();
//ֻ���ӿ�
	T& operator[] (Rank r) const;
	ListNodePosi(T) first() { return header->succ; }
	ListNodePosi(T) last() { return trailer->pred; }
	ListNodePosi(T) find(T const& e) {
		find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p);
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p);//�����������в���
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);//p�Լ�p��n-1�������Ѱ��max
	bool empty() { return _size > 0 ? false : true; }

//��д���ʽӿ�
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertAfter(ListNodePosi(T) p ,T const& e);
	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
	T remove(ListNodePosi(T) p);
	int deduplicate();//����ȥ��
	int uniquify();//����ȥ��
//����
	void traverse(void (*visit) (T&));

	template <typename VST>
	void traverse(VST&);
};
//�б��ʼ��
template <typename T> void List<T>::init() {
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer;
	header->pred = NULL;
	trailer->pred = header;
	trailer->succ = NULL;
	_size = 0;
}

//��p�����е�n��Ԫ�ؿ�����this������
template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{
	init();
	while (n-- > 0) { 
		insertAsLast(p->data); //��p��data����һ���½��,���뵽this��ĩβ
		p = p->succ;
	}
}

template<typename T>
int List<T>::clear()
{
	int oldSize = _size;
	while (_size > 0) {
		//remove�����ڴ���ɾ�����,��ʹ��_size--
		remove(header->succ);
	}
	return oldSize;
}

//����p��ʼ��n��Ԫ������,����������(head,tail)
template<typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) head = p->pred;
	ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++) tail = tail->succ;
	while (n > 1) {//n�Ǵ�����Ԫ�ظ���,���nΪ1����Ҫ����
		ListNodePosi(T) max = selectMax(head->succ, n);
		insertBefore(tail,remove(max));//��insertBefore�л����data�½�һ�����,������Ҫɾ�����
		tail = tail->pred;
		n--;
	}
}

template<typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
	for (int i = 0; i < n; i++) {
		//Ԫ�ش�С��������,��p��ʼ��ǰѰ��e,ʹ��e�����һ��С��data��Ԫ��,��data���뵽e��
		insertAfter(search(p->data, i, p), p->data);
		p = p->succ; 
		remove(p->pred);//��֮ǰ������ɾ��
	}
}

template<typename T>
void List<T>::mergeSort(ListNodePosi(T) & p, int n)
{
	if (n < 2) return;//ֻ��һ��Ԫ��ֱ�ӷ���
	int m = n >> 1;
	ListNodePosi(T) q = p; for (int i = 0; i < m; i++)q = q->succ;//q��ʼ��Ϊһ��ָ���м�Ԫ�ص�ָ��(����Ϊż��ʱ,ָ��ƫ�ҵ��м�Ԫ��)
	mergeSort(p, m);
	mergeSort(q, n - m);//�ݹ����,����������ֳ�һ��������Ϊ2��������
	merge(p, m, *this, q, n-m);//�ϲ�������
}

template<typename T>
void List<T>::merge(ListNodePosi(T)&p, int n, List<T>& list, ListNodePosi(T) q, int m)
{
	ListNodePosi(T) pp = p->pred;//�洢ͷ����ǰһ�����(������header,Ҳ�����ǲ����뱾������Ľ��)
	while (m > 0) {//��Ϊ����p�в���q��Ԫ��,��qԪ������ʱ(m==0)�Ž���ѭ��
		if ((n > 0)&&(p->data <= q->data))//��pָ���������д�С��������,���p��Ԫ������,��ֱ��ȥelse
		{
			if (q == (p = p->succ)) break;//�������,p����һ��Ԫ�ؾ���q(q֮ǰ��Ԫ�ر����뵽p��������),��ʱ�����Ѿ����,����ѭ��
			//˵��p�����Ԫ���Ǻ����,�����޸�
			n--;
		}
		else {
			//qָ���Ԫ�ظ�С,��Ҫ���뵽p��ǰ��,ע��Ҫɾ��q��ָ��Ľڵ�
			q = q->succ;
			insertBefore(p, list.remove(q->pred));
			m--;
		}
	}
	p = pp->succ;
}

template<typename T>
List<T>::List(List<T> const& L) { copyNodes(L.first(),L._size); }

//�����±궨λԪ��
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

//Ч�ʺܵ͵�ȡ��Ԫ�صķ�ʽ
template<typename T>
inline T& List<T>::operator[](Rank r) const
{
	ListNodePosi(T) p = first();
	while (r-- > 0) p = p->succ;
	return p->data;
}

//����
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

//��pǰ��n�������бȽ�,�ҵ�������e�����ֵ
template<typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p)
{
	while (n-- >= 0) {//�����=��Ϊ�˲�������ʹ��,��firstԪ�ص���searchʱ����header(�������NULL���򽫳���)
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
		//>=�е�=���������,����ȡ���һ�����ֵΪmax
		//(����,�Ѿ����ҵ��˺�������Ԫ��,���������max,�´β�����ȻҪѰ�ҵ�����)
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

//ɾ������,��ʹp�������е�Ψһ��ЧԪ��Ҳ������������,�������ǰ���ڱ�(�ڱ����ܱ�������)
template<typename T>
T List<T>::remove(ListNodePosi(T) p)
{
	p->pred->succ=p->succ;
	p->succ->pred = p->pred;
	T e = p->data;//����data����ֱ�Ӹ�ֵ,�������data����
	delete p;
	_size--;//��Ҫ�����޸Ĺ�ģ
	return e;
}

template<typename T>
int List<T>::deduplicate()
{
	if (_size < 2) return 0;//һ��Ԫ��
	int oldSize = _size;
	ListNodePosi(T) p=header;
	int r = 0;
	while ((p=p->succ)!=trailer) {
		//��һ�β���ظ�Ԫ�ؾͻὫ��ɾ��,���Բ����ܳ��ֶ���ظ�Ԫ�ص����
		ListNodePosi(T) q =	find(p->data, r, p);//find�ӱ��ڵ㿪ʼ��ǰ����
		q ? remove(q):r++;
	}
	return oldSize - _size;//ɾ����Ԫ����
}

template<typename T>
int List<T>::uniquify()
{
	if (_size < 2) return 0;//һ��Ԫ��
	int oldSize = _size;
	ListNodePosi(T) p = header; ListNodePosi(T) q;
	int r = 0;
	while (trailer != (q = p->succ)) {
		if (q->data == p->data)
			remove(q);
		else
			p = q;//�������pָ����һ��λ��
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
template<typename VST>//������
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