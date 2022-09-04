#include <iostream>
using namespace std;
typedef int Rank;
#define DEFAULT_CAPACITY 3 //默认容量

template <typename T> 
class Vector {
protected:
	Rank _size; int _capacity; T* _elem;//当前大小，容量，元素(_elem是一个数组)
	//以数组区间[lo,hi)为蓝本赋值一个新向量,A原向量数组地址
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();//扩容
	void shrink();//缩容

public:
	//构造函数
	//c++中可以在参数列表中写默认值，如果函数调用没有指定参数值就使用默认值
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {//s为大小，v是每个元素的初始值
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }//数组整体复制
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
	Vector(Vector<T> const& V){ copyFrom(V._elem, 0,V._size); }
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

	//析构函数
	~Vector() { delete[] _elem; }//释放内存空间

	//重载赋值号=
	Vector<T>& operator=(Vector<T> const& V);
	T& operator[](Rank r);//重载下标运算符a[x]直接定位到数组_elem中的值

	//只读访问接口
	Rank size() const { return _size; }//函数后跟const表示该函数是只读访问，不会对成员变量做任何修改，编译器会对这一点进行检查
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi) const;
	Rank search(T const& e, Rank lo, Rank hi) const;//对有序的向量进行find
	Rank search(T const& e) const //有序向量整体查找
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	int disordered() const;
	bool empty() { return _size > 0 ?  false: true; }
	//可写访问接口
	Rank insert(T const& e, Rank r);
	Rank insert(T const& e) { return insert(e, _size); }
	void unsort(Rank lo, Rank hi);//排序算法对[lo,hi)进行重排序
	int remove(Rank lo, Rank hi);//删除区间[lo,hi)的元素
	T remove(Rank r);//删除Rank为r的元素
	int deduplicate();//无序去重
	int uniquify();//有序去重
	void mergeSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
	//遍历
	void traverse(void (*visit)(T&));
	Rank binSearch(T* A, T const& e, Rank lo, Rank hi) const;
};


template <typename T> //类型T，注意后续::左边要写明是泛型类
void Vector<T>:: copyFrom(T const* A, Rank lo, Rank hi) {//A是_elem,注意左闭右开，_elem[hi]并不会参与赋值
	_elem = new T[_capacity = (hi - lo) * 2];//以两倍创建新数组(用于扩容等场景)
	_size = 0;//从_size=0开始进行一次遍历
	while (lo < hi)
		_elem[_size++] = A[lo++];
}

//当我们进行复制时，希望用vector1=vector2这种操作完成整个数组的赋值，需要重载运算符=
template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
	if (_elem) delete [] _elem;//如果待赋值的向量数组有内容，将这个内容释放掉
	copyFrom(V._elem, 0, V._size);//
	return *this;//返回当前对象的引用，以便链式赋值(a=b=c)
}

template <typename T>
void Vector<T>::expand() {
	if (_size < _capacity) return;//如果当前大小小于容量，无需扩容
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem;
	_elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size;i++) 
		_elem[i] = oldElem[i];
	delete [] oldElem;
}

template <typename T>
void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY << 1)	return;//如果当前容量减半后小于默认容量，则不进行收缩
	if (_size > _capacity >> 2) return;//如果元素数小于容量的25%，则进行收缩
	T* _oldElem = _elem; _elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++){
		_elem[i] = _oldElem[i];
	}
	delete[] _oldElem;
}

//重载[]运算符
template <typename T>
T& Vector<T>::operator[](Rank r) {
	return _elem[r];
}

//置乱算法,将数组中的元素顺序打乱(并非成员函数)
template <typename T> void permute(Vector <T>& vector) {//这里参数一定要写引用，不然调用拷贝构造函数创建一个新的vector，最后被释放
	for (int i = vector.size(); i >0; i--)
		swap(vector[i - 1], vector[rand() % i]);//rand%i 生成一个[0,i)的随机数
}

//将置乱算法放入接口
template <typename T>
void Vector<T>::unsort(Rank lo,Rank hi) {
	T* V = _elem + lo;
	for (Rank i = hi-lo; i > 0; i--)
		swap(V[i-1], V[rand() % i]);
}

template <typename T>static bool lt(T* a, T* b) { return lt(*a, *b); }
template <typename T> static bool lt(T& a, T& b) { return a<b; }
template <typename T> static bool eq(T* a, T* b) { return eq(a,b); }
template <typename T> static bool eq(T& a, T& b) { return a == b; }



template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while ( (lo < hi--) && (e != _elem[hi]) );//从后向前查找,查找失败返回-1,如果有多个相同元素，默认返回Rank最大的那个
	return hi;
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{
	/*return (rand() % 2)?
		binSearch(_elem,e,lo,hi) :fibSearch(_elem,e,lo,hi);*/
	return binSearch(_elem, e, lo, hi);
}

//[lo,hi),如果查找失败,将返回小于元素e的最大值位置,这是由先判断e > this._elem[mi]决定的
template <typename T>
Rank Vector<T>::binSearch(T* A, T const& e, Rank lo, Rank hi) const {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		if (e > Vector<T>::_elem[mi]) lo = mi + 1;
		else if (e < Vector<T>::_elem[mi]) hi = mi;
		else return mi;
	}
	return lo - 1;//将失败位置返回
}


template <typename T>
Rank Vector<T>::insert(T const& e, Rank r) {
	expand();
	for (Rank j = _size; j > r; j--) 
		_elem[j] = _elem[j-1];
	_elem[r] = e;  _size++;
	return r;
}

template <typename T>
T Vector<T>::remove(Rank r) {
	T e = _elem[r];//备份被删除的元素
	remove(r, r+1);
	return e;
}
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
	if (hi == lo) return 0;//单独处理退化情况
	while (hi < _size)
		_elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;//返回删除元素的数量
}

//清除相同元素
template <typename T>
int Vector<T>::deduplicate() {
	int oldSize = _size;//记录原始长度
	Rank i = 1;
	while (i < _size) {
		if (find(_elem[i], 0, i) < 0)
			i++;
		else
			remove(i);
	}
	return oldSize = _size;

}
template<typename T>
int Vector<T>::uniquify()
{
	int i = 0, j = 0;
	while (++j < _size)
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];
	_size = ++i;
	shrink();
	return j-i;//返回删除的数据量
}
template<typename T>
void Vector<T>::traverse(void(* visit)(T&)){
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}

//判断是否有序
template<typename T>
int Vector<T>::disordered() const {
	int n = 0;
	for (int i = 1; i < _size; i++)
		if (_elem[i - 1] > _elem[i])
			n++;
	return n;//当n为0则向量有序
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo <= 1) return;//单个元素默认有序
	Rank mi = (lo + hi) / 2;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem+lo;
	int lb = mi - lo;
	T* B = new T[lb];//B中含有[lo,mi)
	for (int i = 0; i < lb; B[i++] = A[i]);
	int lc = hi - mi;
	T* C = _elem+mi;
	for (int i = 0, j = 0, k = 0; j < lb || k < lc;) {
		if ((j < lb) && ((k >= lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && ((j >= lb) || (B[j] > C[k]))) A[i++] = C[k++];
	}
	delete[] B;
}

//int main() {
//	Vector<int> v = Vector<int>(3,3,1);
//	v.insert(10, 1);
//	v.remove(1);
//}



