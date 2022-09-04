//B-tree ģ����
#include "BTNode.hpp"
#include "../../db1-4/release.hpp"

template <typename T> class BTree {
protected:
	int _size;	//��ŵĹؼ���key������
	int _order;	//B-tree�Ľ״�,��_order·ƽ��������,����Ϊ3��,����ʱָ��,�����޸�;
	BTNodePosi(T) _root;//���ڵ�
	BTNodePosi(T) _hot;//search�����ʵķǿսڵ�λ��
	void solveOverflow(BTNodePosi(T) v);//����������֮��ķ��Ѵ���
	void solveUnderflow(BTNodePosi(T) v);//��ɾ��������֮��ĺϲ�����
public:
	//���캯��,Ĭ��Ϊ����
	BTree(int order = 3) : _order(order), _size(0) {_root = new BTNode<T>();}
	~BTree() { if (_root) release(_root); }//����,�ͷ����нڵ�
	int const order() { return _order; }
	int const size() { return _size; }
	BTNodePosi(T)& root() { return _root; }
	bool empty() { return !_root; }
	BTNodePosi(T) search(const T& e);
	bool insert(const T& e);
	bool remove(const T& e);
};


template<typename T>
BTNodePosi(T) BTree<T>::search(const T& e)
{
	BTNodePosi(T) v = _root; _hot = NULL;
	while (v) {//������
		Rank r = v->key.search(e); 
		if ((r >= 0) && (v->key[r] == e)) return v;
		_hot = v; v = v->child[r + 1];//ת������,����e����key[r],ѡ��֧·r+1
	}
	return NULL;
}

template<typename T>
bool BTree<T>::insert(const T& e)
{
	BTNodePosi(T) v = search(e);//_hotָ�����һ���ڵ�
	if (v) return false;//���e�Ѿ����ڣ�ʧ��		
	Rank r = _hot->key.search(e);//��_hot�ڵ�Ĺؼ���������e(�ض�ʧ��,������e�ĺ���λ��)
	_hot->key.insert(e, r+1);//��������r��λ�ò���e
	_hot->child.insert(NULL, r + 2);
	_size++;
	solveOverflow(_hot);
	return true;
}

//��v�Ľڵ����һ��ֵ��,���ܻᳬ������
template<typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v)
{
	//δ��������
	if (v->child.size() <= _order) return;
	Rank mid = _order / 2;
	BTNodePosi(T) right = new BTNode<T>();//�½�һ��������,���ڴ洢���Ѻ���ұ�Ԫ����Ϣ
	//���������Ĺؼ��븳ֵ�±�Ϊi
	for (Rank i = 0; i < _order - mid -1; i++)
	{
		right->key.insert(v->key.remove(mid + 1),i);//removeɾ��λ����mid�ĺ�һλ,ɾ��һ�ζ�Ҫ�ƶ�ԭ����,�ȽϺ�ʱ
		right->child.insert(v->child.remove(mid + 1),i);//��Ԫ����������ʼ����,
	}
	right->child[_order - mid - 1] = v->child.remove(mid + 1);//���һ���������ֶ�����
	//�����ѳ��������������ӽڵ��parent��ֵ
	if (right->child[0])
		for (int i = 0; i < _order - mid; i++)
			right->child[i]->parent = right;
	BTNodePosi(T) p = v->parent;//Ϊ����ǰ�Ľڵ�v�ĸ���
	//����Ǹ��ڵ����,����Ҫ�½�һ�����ڵ�
	if (!p) {
		p=_root = new BTNode<T>();
		_root->child[0] = v;
		v->parent = p;
	}
	//�ڸ��׽ڵ���Ѱ�ҹؼ����λ��
	Rank r = 1 + p->key.search(v->key[0]);
	p->key.insert(v->key.remove(mid), r);
	p->child.insert(right, r+1);
	right->parent = p;
	solveOverflow(p);//���ڵ��ڲ�������Ҳ������,��Ҫ�ݹ���
}

template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v)
{
	//����b-tree������������,ֱ�ӷ���
	if (v->child.size() >= (_order + 1) / 2) return;
	BTNodePosi(T) p = v->parent;
	if (!p) {//v�Ǹ��ڵ�����
		//���ڵ�Ĺؼ����������ڵ���1����
		if( !v->key.size() && v->child[0] ){//�����ڵ��Ѿ�û�йؼ���,����childʱ
			_root = v->child[0];
			_root->parent = NULL;
			v->child = NULL; release(v);
		}
		return;
	}
	//v�Ǹ��ڵ�
	Rank r = 0; 
	while (p->child[r] != v) r++;//�ҵ���Ӧv�ڵ��p��child���±�
	//case1,�����ֵܽ�ؼ���
	if (r > 0) {
		BTNodePosi(T) ls = p->child[r - 1];
		if (ls->child.size() > (_order + 1) / 2) {//�鿴���ֵܵĹؼ��������Ƿ�
			v->key.insert(p->key[r - 1],0);//v��p��һ��Ԫ��
			p->key[r - 1] = ls->key.remove(ls->key.size()-1);//p��ls��һ��Ԫ��
			v->child.insert(ls->child.remove(ls->child.size()-1),0);//ls����һ��Ԫ��,�����Ӧ��������(����ΪNULL)��v
			if (v->child[0])
				v->child[0]->parent = v;
		return;
		}
	}
	//case2,�����ֵܽ�ؼ���
	if (r < p->child.size() - 1) {
		BTNodePosi(T) rs = p->child[r+1];
		if (rs->child.size() > (_order + 1) / 2) {
			v->key.insert( p->key[r] , v->key.size() );//ע�����ﲻҪɾ��p�е�Ԫ��,�������ù���
			p->key[r] = rs->key.remove(0);//����ֱ�Ӷ�p��Ԫ�ظ�ֵ,����Ҫ����
			v->child.insert(rs->child.remove(0), v->child.size());
			if (v->child[v->child.size() - 1]) {
				v->child[v->child.size() - 1]->parent = v;
			}
			return;
		}
	}
	//case3 �����ֵ�ҪôΪ��,Ҫô�ռ䲻���Խ��v�ؼ���,�����ҽڵ��Լ�p��һ��Ԫ�غϲ�Ϊһ���ڵ�
	//case 3.1
	if (r > 0) {
		BTNodePosi(T) ls = p->child[r - 1];
		//�ϲ�p��Ԫ��
		ls->key.insert(p->key.remove(r - 1), ls->key.size());//ֱ����p��ɾ��������ؼ���
		p->child.remove(r);//���ؼ���������ɾ��
		//�ϲ�v��Ԫ��
		ls->child.insert(v->child.remove(0), ls->child.size());
		if (ls->child[ls->child.size()-1])
			ls->child[ls->child.size()-1]->parent = ls;
		while (!v->key.empty()) {
			ls->key.insert(v->key.remove(0), ls->key.size());
			ls->child.insert(v->child.remove(0), ls->child.size());
			if (ls->child[ls->child.size() - 1])
				ls->child[ls->child.size() - 1]->parent = ls;
		}
		release(v);
	}
	else {//case 3.2 
		BTNodePosi(T) rs = p->child[r + 1];
		//�ϲ�p��Ԫ��
		rs->key.insert(p->key.remove(r),0);//ֱ����p��ɾ��������ؼ���
		p->child.remove(r);//���ؼ���������ɾ��
		//�ϲ�v��Ԫ��
		rs->child.insert(v->child.remove(v->child.size()-1),0);
		if (rs->child[0])
			rs->child[0]->parent = rs;
		while (!v->key.empty()) {
			rs->key.insert(v->key.remove(v->key.size()-1), 0);
			rs->child.insert(v->child.remove(v->child.size() - 1),0);
			if (rs->child[0])
				rs->child[0]->parent = rs;
		}
		release(v);
	}
	solveUnderflow(p);
	return;
}

template <typename T>
bool BTree<T>::remove(const T& e) {
	BTNodePosi(T) v = search(e);//��λ���ڵ�
	if (!v) return false;
	Rank r = v->key.search(e);//��λ���ؼ���
	if (v->child[0]) {//���v����Ҷ�ӽڵ�(B-tree��child�Ǵ���������������,child[1]������child[0]���ڵ�����²������)
		//������Ҫ�ҵ�ɾ���ڵ������������һλ��
		BTNodePosi(T) u = v->child[r + 1];//��ȡ������
		while (u->child[0]) u = u->child[0];//���������о���������(���������������ͬ,��Ϊ������������������,û����������û��������)
		//uΪ������������ڵ�
		v->key[r] = u->key[0];
		//��v��Ϊ��ɾ���ڵ�,rΪ��ɾ���ؼ������
		v = u;
		r = 0;
	}
	//v��Ҷ��,ֱ��ɾ��;v����Ҷ��,�ڽ������ݺ�ɾ��vֱ�Ӻ�̵Ľڵ�
	v->key.remove(r);
	v->child.remove(r + 1);//��Ȼ����Ԫ���ǿ�,����ҲҪɾ��,ʹ��child�����С��ȷ
	_size--;
	solveUnderflow(v);//���ɾ���ڵ����������
	return true;
}


