#include "2-1Vector.hpp"

template <typename T> class myStack : public Vector<T> {//StackӦ���Ǹ����е����ص��˱���
public:
	void push(T const& e) { Vector<T>::insert(e, Vector<T>::size()); }
	T pop() { return Vector<T>::remove(Vector<T>::size() - 1); }
	T& top() { return (*this)[Vector<T>::size() - 1]; }
};
