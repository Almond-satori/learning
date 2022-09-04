#include <iostream>
using namespace std;
typedef int Rank;
#define DEFAULT_CAPACITY 3 //Ĭ������

template <typename T> 
class Vector {
protected:
	Rank _size; int _capacity; T* _elem;//��ǰ��С��������Ԫ��(_elem��һ������)
	//����������[lo,hi)Ϊ������ֵһ��������,Aԭ���������ַ
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();//����
	void shrink();//����

public:
	//���캯��
	//c++�п����ڲ����б���дĬ��ֵ�������������û��ָ������ֵ��ʹ��Ĭ��ֵ
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {//sΪ��С��v��ÿ��Ԫ�صĳ�ʼֵ
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }//�������帴��
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
	Vector(Vector<T> const& V){ copyFrom(V._elem, 0,V._size); }
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

	//��������
	~Vector() { delete[] _elem; }//�ͷ��ڴ�ռ�

	//���ظ�ֵ��=
	Vector<T>& operator=(Vector<T> const& V);
	T& operator[](Rank r);//�����±������a[x]ֱ�Ӷ�λ������_elem�е�ֵ

	//ֻ�����ʽӿ�
	Rank size() const { return _size; }//�������const��ʾ�ú�����ֻ�����ʣ�����Գ�Ա�������κ��޸ģ������������һ����м��
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi) const;
	Rank search(T const& e, Rank lo, Rank hi) const;//���������������find
	Rank search(T const& e) const //���������������
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	int disordered() const;
	bool empty() { return _size > 0 ?  false: true; }
	//��д���ʽӿ�
	Rank insert(T const& e, Rank r);
	Rank insert(T const& e) { return insert(e, _size); }
	void unsort(Rank lo, Rank hi);//�����㷨��[lo,hi)����������
	int remove(Rank lo, Rank hi);//ɾ������[lo,hi)��Ԫ��
	T remove(Rank r);//ɾ��RankΪr��Ԫ��
	int deduplicate();//����ȥ��
	int uniquify();//����ȥ��
	void mergeSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
	//����
	void traverse(void (*visit)(T&));
	Rank binSearch(T* A, T const& e, Rank lo, Rank hi) const;
};


template <typename T> //����T��ע�����::���Ҫд���Ƿ�����
void Vector<T>:: copyFrom(T const* A, Rank lo, Rank hi) {//A��_elem,ע������ҿ���_elem[hi]��������븳ֵ
	_elem = new T[_capacity = (hi - lo) * 2];//����������������(�������ݵȳ���)
	_size = 0;//��_size=0��ʼ����һ�α���
	while (lo < hi)
		_elem[_size++] = A[lo++];
}

//�����ǽ��и���ʱ��ϣ����vector1=vector2���ֲ��������������ĸ�ֵ����Ҫ���������=
template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
	if (_elem) delete [] _elem;//�������ֵ���������������ݣ�����������ͷŵ�
	copyFrom(V._elem, 0, V._size);//
	return *this;//���ص�ǰ��������ã��Ա���ʽ��ֵ(a=b=c)
}

template <typename T>
void Vector<T>::expand() {
	if (_size < _capacity) return;//�����ǰ��СС����������������
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem;
	_elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size;i++) 
		_elem[i] = oldElem[i];
	delete [] oldElem;
}

template <typename T>
void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY << 1)	return;//�����ǰ���������С��Ĭ���������򲻽�������
	if (_size > _capacity >> 2) return;//���Ԫ����С��������25%�����������
	T* _oldElem = _elem; _elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++){
		_elem[i] = _oldElem[i];
	}
	delete[] _oldElem;
}

//����[]�����
template <typename T>
T& Vector<T>::operator[](Rank r) {
	return _elem[r];
}

//�����㷨,�������е�Ԫ��˳�����(���ǳ�Ա����)
template <typename T> void permute(Vector <T>& vector) {//�������һ��Ҫд���ã���Ȼ���ÿ������캯������һ���µ�vector������ͷ�
	for (int i = vector.size(); i >0; i--)
		swap(vector[i - 1], vector[rand() % i]);//rand%i ����һ��[0,i)�������
}

//�������㷨����ӿ�
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
	while ( (lo < hi--) && (e != _elem[hi]) );//�Ӻ���ǰ����,����ʧ�ܷ���-1,����ж����ͬԪ�أ�Ĭ�Ϸ���Rank�����Ǹ�
	return hi;
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{
	/*return (rand() % 2)?
		binSearch(_elem,e,lo,hi) :fibSearch(_elem,e,lo,hi);*/
	return binSearch(_elem, e, lo, hi);
}

//[lo,hi),�������ʧ��,������С��Ԫ��e�����ֵλ��,���������ж�e > this._elem[mi]������
template <typename T>
Rank Vector<T>::binSearch(T* A, T const& e, Rank lo, Rank hi) const {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		if (e > Vector<T>::_elem[mi]) lo = mi + 1;
		else if (e < Vector<T>::_elem[mi]) hi = mi;
		else return mi;
	}
	return lo - 1;//��ʧ��λ�÷���
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
	T e = _elem[r];//���ݱ�ɾ����Ԫ��
	remove(r, r+1);
	return e;
}
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
	if (hi == lo) return 0;//���������˻����
	while (hi < _size)
		_elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;//����ɾ��Ԫ�ص�����
}

//�����ͬԪ��
template <typename T>
int Vector<T>::deduplicate() {
	int oldSize = _size;//��¼ԭʼ����
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
	return j-i;//����ɾ����������
}
template<typename T>
void Vector<T>::traverse(void(* visit)(T&)){
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}

//�ж��Ƿ�����
template<typename T>
int Vector<T>::disordered() const {
	int n = 0;
	for (int i = 1; i < _size; i++)
		if (_elem[i - 1] > _elem[i])
			n++;
	return n;//��nΪ0����������
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo <= 1) return;//����Ԫ��Ĭ������
	Rank mi = (lo + hi) / 2;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem+lo;
	int lb = mi - lo;
	T* B = new T[lb];//B�к���[lo,mi)
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



