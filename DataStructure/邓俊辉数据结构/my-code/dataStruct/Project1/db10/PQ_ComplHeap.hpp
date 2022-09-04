#include "../db1-4/2-1Vector.hpp"
#include "priorityQueue.hpp"

template <typename T> class PQ_ComplHeap : public PQ<T>, public Vector<T> {
protected:
	Rank percolateDown(Rank n, Rank i); //����
	Rank percolateUp(Rank n, Rank i); //����
	void heapify(Rank n); //Floyd���ѷ���
public:
	PQ_ComplHeap(){} //Ĭ�Ϲ���
	PQ_ComplHeap( T* A,Rank n ) { //�������� 
		copyFrom(A, 0, n);
		heapify(n);
	}
	void insert(T); //���ձȽ���ȷ�������ȼ�����,�������
	T getMax(); //ȡ�����ȼ���ߵĴ���
	T delMax(); //ɾ�����ȼ���ߵĴ���
	PQ_ComplHeap<T> heapSort(Rank lo, Rank hi);
};

template<typename T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i)
{
	Rank j; //���ڵ�,����,�Һ����е������
	while (i != (j = ProperParent(_elem, n, i))) {
		//���i���Ǹ��ڵ�,����,�Һ�������֮��������,��������߽���
		swap(_elem[i], _elem[j]);
		i = j;
	}
	return i;
}

template<typename T>
Rank PQ_ComplHeap<T>::percolateUp(Rank n, Rank i)
{
	while ( ParentValid(i) ) {
		Rank curr_parent = Parent(i);
		if (lt(_elem[i], _elem[curr_parent])) break; //��˳����ȷʱ,���ȼ���С�ڸ�,ֹͣ����
		swap(_elem[i], _elem[curr_parent]);
		i = curr_parent;
	}
	return i;
}

template<typename T>
void PQ_ComplHeap<T>::heapify(Rank n)
{
	//�Ե����ϵ����ӽ�С�Ķ�,�������Ϊ������
	for (int i = LastInternal(n); InHeap(i) ; i--){ percolateDown(i); }
}

template<typename T>
void PQ_ComplHeap<T>::insert(T)
{
	Vector<T>::insert(e); //�������в������ֵ
	percolateUp( _size - 1 ); //�Ըô����������˵��� 
}

template<typename T>
T PQ_ComplHeap<T>::getMax() { return _elem[0]; }

template<typename T>
T PQ_ComplHeap<T>::delMax()
{
	T max = _elem[0];
	_elem[0] = _elem[--_size]; //��ĩβ���������״���
	percolateDown(_size, 0);
	return max;
}

template<typename T>
PQ_ComplHeap<T> PQ_ComplHeap<T>::heapSort(Rank lo, Rank hi)
{
	//�����������佨����һ����ȫ�����[lo,hi)
	PQ_ComplHeap<T> heap = PQ_ComplHeap<T>(_elem + lo, hi - lo);
	while (!heap.empty()) {
		_elem[--hi] = heap.delMax();
	}
}
