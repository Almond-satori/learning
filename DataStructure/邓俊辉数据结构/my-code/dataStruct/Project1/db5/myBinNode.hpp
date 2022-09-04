#include <iostream>
#include "../db1-4/4-1-stack.hpp"
#include "../db1-4/4-2-queue.hpp"

#define BinNodePosi(T) myBinNode<T>*
#define stature(p) ( (p) ? (p)->height : -1 )//�ڵ�߶�,��������߶�Ϊ-1,����һ�����ڵ�����߶�Ϊ0

//myBinNode�ĳ���״̬�ж�,��ʵ��,x��myBinNode<T>
#define IsRoot(x) ( ! ( (x).parent ) )
#define IsLChild(x) ( !IsRoot(x) && ( (x).parent->lc == &(x) ))
#define IsRChild(x) ( !IsRoot(x) && ( (x).parent->rc == &(x) ))
#define HasParent(x) ( !IsRoot(x) )
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x) )
#define HasBothChild(x) (HasLChild(x) && HasRChild(x) )
#define IsLeaf(x) ( ! HasChild(x) )
//��myBinNode�������ϵ�Ľڵ�,ע��p������myBinNode<T> *
//�ֵܽڵ�
#define sibling(p) \
	(IsLChild( * (p) ) ? (p).parent->rc : (p).parent->lc)
//����ڵ�pΪBinNodePosi(T)
#define uncle(p) \
	(IsLChild(  *((p)->parent) ) ? (p)->parent->parent->rc : (p)->parent->parent->lc )
//���Ը��ڵ������,�������õ����ڵ��lc����rc
#define FromParentTo(x) \
	( IsRoot(x) ? myBinTree<T>::_root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )

typedef enum{RB_RED,RB_BLACK} RBColor;//�ڵ���ɫ(�����)

template <typename T> struct myBinNode {//�������ڵ�
public://Ϊ�˼�,ʡ�Է�װ
//��Ա
	T data;//����
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//���ڵ�,���Һ���
	int height;//�߶�
	int npl;//Null Path Length ��ʽ�� Ҳ������height����
	RBColor color;//���������ɫ
//���캯��
	//�Խ��и��ڵ�����Ĺ���
	myBinNode() :
		parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
	//һ��Ĵ��ι���
	myBinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, 
		int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}
//�����ӿ�
	int size();//ͳ�Ƶ�ǰ�ڵ�������
	BinNodePosi(T) insertAsLC(T const& e) { return lc = new myBinNode(e, this); }
	BinNodePosi(T) insertAsRC(T const& e) { return rc = new myBinNode(e, this); }
	BinNodePosi(T) succ();//ȡ����ǰ�ڵ��ֱ�Ӻ��
	template <typename VST> void travLevel(VST& visit);//��α���
	template <typename VST> void travPre(BinNodePosi(T) x,VST& visit);//�������
	template <typename VST> void travIn(BinNodePosi(T) x, VST& visit);//�������
	template <typename VST> void travPost(BinNodePosi(T) x,VST& visit);//��������
//�Ƚ���
	bool operator< (myBinNode const& bn) { return data < bn.data; }
	bool operator> (myBinNode const& bn) { return data > bn.data; }
	bool operator== (myBinNode const& bn) { return data == bn.data; }
	bool operator>= (myBinNode const& bn) { return data >= bn.data; }
	bool operator<= (myBinNode const& bn) { return data <= bn.data; }
};

////�������ѡ��
//template<typename T> template<typename VST>
//void myBinNode<T>::travIn(VST& visit)
//{
//	switch (1)
//	{
//	case 1: travIn_1(this, visit); break;
//	case 2: travIn_2(this, visit); break;
//	case 3: travIn_3(this, visit); break;
//	case 4: travIn_4(this, visit); break;
//	default: travIn_R(this, visit);break;//�ݹ��
//	}
//}

template<typename T> template<typename VST>
void myBinNode<T>::travPre(BinNodePosi(T) x, VST& visit)
{	
	myStack<BinNodePosi(T)> stack;//��Žڵ�
	while (true) {
		visitAlongLeftBranch(x,visit,stack);//�˳�����ʱ�������֧�ı߽�,x=NULL
		if (stack.empty()) break;//ջ�ս���
		x = stack.pop();//�����һ���ҽڵ㵯��,������ҽڵ�Ϊ��,��������
	}
}


template<typename T , typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) & x, VST& visit, myStack<BinNodePosi(T)>& stack){
	while (x) {//��������������ӵĽڵ�
		visit(x->data);//��һ��ѭ��,������������������ĸ��ڵ�,�����ѭ�������������
		stack.push(x->rc);//���ҽڵ���ջ
		x = x->lc;
	}
}

template<typename T> template<typename VST>
void myBinNode<T>::travIn(BinNodePosi(T) x, VST& visit)
{
	myStack<BinNodePosi(T)> stack;
	while (true) {
		goAlongLeftBranch(x, visit, stack);//����ʱ�Ѿ�x��������ߵĽڵ����һ����ڵ�(NULL)
		if (stack.empty()) break;//ѭ����ֹ����
		x = stack.pop();//�����߽����ڵ�
		visit(x->data);
		x = x->rc;//����������
	}
}

//�������,ֱ����ڵ�Ϊ��
template<typename T, typename VST>
static void goAlongLeftBranch(BinNodePosi(T)& x, VST& visit, myStack<BinNodePosi(T)>& stack) {
	while (x) {//��������������ӵĽڵ�
		stack.push(x);//����·�Ľڵ���ջ
		x = x->lc;
	}
}

template<typename T>
BinNodePosi(T) myBinNode<T>::succ()//�����������һλ��
{
	BinNodePosi(T) s = this;
	if (s->rc) {//�����������,����һ�η��ʵĽڵ�һ������������(��->��->��,������ҽڵ�,˵�����ڷ��ʵĽڵ��Ǹ��ڵ�)
		s = rc;
		while (HasLChild(*s)) s = s->lc;
	}
	else {//������ܼ������ҷ���,���Ϊһ�������ķ��ʽ�����,���ص���һ�ε���ڵ���
		while (IsRChild(*s)) s = s->parent;//��ǰ���ص����������ĸ��ڵ�
		s = s->parent;//�ڱ��������ĸ��ڵ����Ϸ���(���θ��ڵ�������,һ���������Ϸ���)
	}
	return s;
}

template<typename T> template<typename VST>
void myBinNode<T>::travPost(BinNodePosi(T) x, VST& visit)
{
	myStack<BinNodePosi(T)> stack;
	stack.push(x);
	while (!stack.empty()) {
		if (stack.top() != x->parent) //ջ��Ϊ��һ�εķ���λ��,���������ĸ��ڵ㱻����(�жϲ�����),�����Ϸ���,������ջ��(�ҽڵ�,���������ĸ�)Ϊ�µĸ�
			gotoHLVFL(stack);//�ҵ�������ˮƽ����λ��,������·������������ջ,�����ҽڵ�,������ڵ�(��ڵ���Կ�Ϊ��㵽ˮƽ����λ�õ�·��)
		x = stack.pop(); visit(x->data);
	}
}

template <typename T>
void gotoHLVFL(myStack<BinNodePosi(T)>& stack) {
	while (BinNodePosi(T) x = stack.top() ) {
		if (HasLChild(*x)) {//�����������
			if (HasRChild(*x)) stack.push(x->rc);
			stack.push(x->lc);
		}
		else {//û����ڵ�,������
			stack.push(x->rc);
		}
	}
	stack.pop();//���������ջ��NULL
}

template<typename T> template<typename VST>
void myBinNode<T>::travLevel(VST& visit)
{
	myQueue<BinNodePosi(T)> queue;
	queue.enqueue(this);
	while (!queue.empty()) {
		cout<< queue.front()->data << endl;
		BinNodePosi(T) x = queue.dequeue(); visit(x->data);
		if ( HasLChild(*x) ) queue.enqueue(x->lc);
		if ( HasRChild(*x) ) queue.enqueue(x->rc);
	}
}
