#include "2-1Vector.hpp"

template <typename T> class myStack : public Vector<T> {//Stack应该是跟库中的类重叠了报错
public:
	void push(T const& e) { Vector<T>::insert(e, Vector<T>::size()); }
	T pop() { return Vector<T>::remove(Vector<T>::size() - 1); }
	T& top() { return (*this)[Vector<T>::size() - 1]; }
};
