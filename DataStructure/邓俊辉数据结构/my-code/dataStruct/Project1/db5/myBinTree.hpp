#include "myBinNode.hpp"
#include "../db1-4/release.hpp"
using namespace std;

//˵��:remove�е�release�޷�ʵ��,�����������,remove,removeAt�����ѱ�ע�͵�!


template <typename T> class myBinTree {
protected:
	int _size;//��������ģ
	BinNodePosi(T) _root;//���ڵ�����
	virtual int updateHeight(BinNodePosi(T) x);//����x�ڵ�ĸ߶�
	void updateHeightAbove(BinNodePosi(T) x);//����x�ڵ��Լ�x���ȵĽڵ�߶�
public:
//��������
	myBinTree() : _size(0), _root(NULL){}//���캯��
	~myBinTree() { if (_size > 0) remove(_root); }//��������
//ֻ������
	int size() const { return _size; }
	bool empty() const { return !_root; }
	BinNodePosi(T) root() const { return _root; }//��ȡ���ڵ�����
//����
	BinNodePosi(T) insertAsRoot(T const& e);//������ڵ�
	//����e��Ϊx�ڵ�����Һ��Ӳ���
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
	//T��Ϊx��������������
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x,  myBinTree<T>* & S);
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, myBinTree<T>* & S);//ֱ��ʹ������,��Ϊ��Ҫ�ں����ڲ��ͷ����ָ��
	int remove(BinNodePosi(T) x);//ɾ���ڵ�x�Լ���xΪ��������
	myBinTree<T>* secede(BinNodePosi(T) x);//������x�ӵ�ǰ����ɾ��,ת��Ϊһ�ö�������
	
//������,������ÿ��Ԫ��ִ��visit����
	//�������
	template <typename VST> void travLevel(VST& visit) { if (_root) _root->travLevel(visit); }
	//�������
	template <typename VST> void travPre(VST& visit) { if (_root) _root->travPre(_root,visit); }
	//�������
	template <typename VST> void travIn(VST& visit) { if (_root) _root->travIn(_root, visit); }
	//�������
	template <typename VST> void travPost(VST& visit) { if (_root) _root->travPost(_root, visit); }

//�Ƚ���
	bool operator== (myBinTree<T> const& t) { return _root && t._root && (_root == t._root); }
};

//�߶ȵĶ������������ͬ,���Ը���������ظú���
template<typename T>
int myBinTree<T>::updateHeight(BinNodePosi(T) x)
{
	//x=NULL null pointer
	if(x!=NULL)
		return x->height = 1 + max(stature(x->lc), stature(x->rc));
	return -1;//�����߶�Ϊ-1
	
}

template<typename T>
void myBinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x) {
		updateHeight(x);
		x = x->parent;
	}
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsRoot(T const& e)
{
	_size = 1;
	 _root = new myBinNode<T> (e);
	 BinNodePosi(T) r = _root;
	 return r;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsLC(e);//x�ڵ�ĺ���
	updateHeightAbove(x);//��x������ڵ�,x�Լ�x�����ȵĸ߶ȶ�Ҫ�ı�
	return x->lc;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsRC(e);//x�ڵ�ĺ���
	updateHeightAbove(x);//��x������ڵ�,x�Լ�x�����ȵĸ߶ȶ�Ҫ�ı�
	return x->rc;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::attachAsLC(BinNodePosi(T) x, myBinTree<T>*& S)
{
	if (x->lc = S->_root) x->lc->parent = x;
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL; 
	S->_size = 0; 
	release(S); S = NULL; //release���������ͷ���Դ
	return x;
}

template<typename T>
BinNodePosi(T) myBinTree<T>::insertAsRC(BinNodePosi(T) x, myBinTree<T>*& S)
{
	if (x->rc = S->_root) x->rc->parent = x;
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL;
	S->_size = 0;
	release(S); S = NULL;
	return x;
}

template<typename T>
int myBinTree<T>::remove( BinNodePosi(T) p )
{
	//p��ָ��
	myBinNode<T> x = *p;
	FromParentTo(x) =NULL;//�жϸ��ڵ㵽x������
	updateHeight(p->parent); //����Ǹ��ڵ�,������NULL
	int n = removeAt(p); _size -= n; return n;
}

template<typename T> 
static int removeAt(BinNodePosi(T) x) {
	if (!x) return 0;
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);
	release(x->data); release(x); return n;//�ͷű�ժ���ڵ�
}

template<typename T>
myBinTree<T>* myBinTree<T>::secede(BinNodePosi(T) x)
{
	//x��ָ��
	FromParentTo(*x) = NULL;//�жϸ��ڵ㵽x������
	updateHeightAbove(x->parent);
	myBinTree<T>* S = new myBinTree<T>;
	S->_root = x;
	x->parent = NULL;//ע����ڵ���Ҫ��parent����Ϊ��
	S->_size = x->size();
	_size -= S->_size;
	return S;
}

template <typename T>
void visit(T data) {
	cout << data << endl;
}
