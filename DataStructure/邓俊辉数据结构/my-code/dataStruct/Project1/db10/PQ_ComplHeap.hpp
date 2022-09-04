#include "../db1-4/2-1Vector.hpp"
#include "priorityQueue.hpp"

template <typename T> class PQ_ComplHeap : public PQ<T>, public Vector<T> {
protected:
	Rank percolateDown(Rank n, Rank i); //下滤
	Rank percolateUp(Rank n, Rank i); //上滤
	void heapify(Rank n); //Floyd建堆方法
public:
	PQ_ComplHeap(){} //默认构造
	PQ_ComplHeap( T* A,Rank n ) { //批量构造 
		copyFrom(A, 0, n);
		heapify(n);
	}
	void insert(T); //按照比较器确定的优先级次序,插入词条
	T getMax(); //取出优先级最高的词条
	T delMax(); //删除优先级最高的词条
	PQ_ComplHeap<T> heapSort(Rank lo, Rank hi);
};

template<typename T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i)
{
	Rank j; //父节点,左孩子,右孩子中的最大者
	while (i != (j = ProperParent(_elem, n, i))) {
		//如果i不是父节点,左孩子,右孩子三者之间的最大者,则与最大者交换
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
		if (lt(_elem[i], _elem[curr_parent])) break; //当顺序正确时,优先级子小于父,停止上滤
		swap(_elem[i], _elem[curr_parent]);
		i = curr_parent;
	}
	return i;
}

template<typename T>
void PQ_ComplHeap<T>::heapify(Rank n)
{
	//自底向上地连接较小的堆,最后连接为整个堆
	for (int i = LastInternal(n); InHeap(i) ; i--){ percolateDown(i); }
}

template<typename T>
void PQ_ComplHeap<T>::insert(T)
{
	Vector<T>::insert(e); //在向量中插入这个值
	percolateUp( _size - 1 ); //对该词条进行上滤调整 
}

template<typename T>
T PQ_ComplHeap<T>::getMax() { return _elem[0]; }

template<typename T>
T PQ_ComplHeap<T>::delMax()
{
	T max = _elem[0];
	_elem[0] = _elem[--_size]; //以末尾词条代替首词条
	percolateDown(_size, 0);
	return max;
}

template<typename T>
PQ_ComplHeap<T> PQ_ComplHeap<T>::heapSort(Rank lo, Rank hi)
{
	//将待排序区间建立成一个完全二叉堆[lo,hi)
	PQ_ComplHeap<T> heap = PQ_ComplHeap<T>(_elem + lo, hi - lo);
	while (!heap.empty()) {
		_elem[--hi] = heap.delMax();
	}
}
