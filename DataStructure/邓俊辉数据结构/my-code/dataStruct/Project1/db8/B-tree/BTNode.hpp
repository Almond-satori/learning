/* B-���Ľڵ�BTNode�� */

#include "../../db1-4/2-1Vector.hpp"
#define BTNodePosi(T) BTNode<T>* 

template <typename T> struct BTNode {
	BTNodePosi(T) parent;//���ڵ�
	Vector<T> key;//�ؽ�������
	Vector<BTNodePosi(T)> child;//��������

//���캯��,BTNodeֻ����Ϊ���ڵ㴴��,����0���ؼ����һ���յĺ���ָ��
	BTNode() { parent = NULL; child.insert(NULL, 0); }

	BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL) {
		parent = NULL;
		key.insert(e,0);
		child.insert(lc,0);
		child.insert(rc,1);
		if (lc) lc->parent = this;
		if (rc) rc->parent = this;
	}
};