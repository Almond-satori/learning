#include "QuadlistNode.hpp" //����Quadlist�ڵ���

template <typename T> class Quadlist { //Quadlistģ����
private:
   int _size; QlistNodePosi(T) header, trailer; //��ģ��ͷ�ڱ���β�ڱ�
protected:
   void init(); //Quadlist����ʱ�ĳ�ʼ��
   int clear(); //������нڵ�
public:
// ���캯��
   Quadlist() { init(); } //Ĭ��
// ��������
   ~Quadlist() { clear(); delete header; delete trailer; } //ɾ�����нڵ㣬�ͷ��ڱ�
// ֻ�����ʽӿ�
   Rank size() const { return _size; } //��ģ
   bool empty() const { return _size <= 0; } //�п�
   QlistNodePosi(T) first() const { return header->succ; } //�׽ڵ�λ��
   QlistNodePosi(T) last() const { return trailer->pred; } //ĩ�ڵ�λ��
   bool valid ( QlistNodePosi(T) p ) //�ж�λ��p�Ƿ����Ϸ�
   { return p && ( trailer != p ) && ( header != p ); }
// ��д���ʽӿ�
   T remove ( QlistNodePosi(T) p ); //ɾ�����Ϸ���λ��p���Ľڵ㣬���ر�ɾ���ڵ����ֵ
   QlistNodePosi(T) //��*e��Ϊp�ĺ�̡�b�����ڲ���
   insertAfterAbove ( T const& e, QlistNodePosi(T) p, QlistNodePosi(T) b = NULL );
// ����
   //void traverse ( void (* ) ( T& ) ); //�������ڵ㣬����ʵʩָ������������ָ�룬ֻ����ֲ��޸ģ�
   //template <typename VST> //������
   //void traverse ( VST& ); //�������ڵ㣬����ʵʩָ���������������󣬿�ȫ�����޸Ľڵ㣩
}; //Quadlist

template<typename T>
void Quadlist<T>::init()
{
	header = new QuadlistNode<T>;
	trailer = new QuadlistNode<T>;
	header->succ = trailer; header->pred = NULL;
	trailer.pred = header; trailer.succ = NULL;
	header->above = trailer->above = NULL;
	header->below = trailer->below = NULL;
	_size = 0;
}

template<typename T>
int Quadlist<T>::clear()
{
	int oldSize = _size;
	while (_size > 0)
		remove(header->succ);
	return oldSize;//����ɾ������
}

template<typename T>
T Quadlist<T>::remove(QlistNodePosi(T) p)
{	
	//����Ҫ����above��below,��Ϊ���������ᱻdelete,û��Ҫ�������е�ָ�뷽��
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	_size--;
	T e = p->entry; delete p;
	return e;//���ش���
}

//��p֮��,��b֮�ϲ���Ԫ��e
template<typename T>
QlistNodePosi(T) Quadlist<T>::insertAfterAbove(T const& e, QlistNodePosi(T) p, QlistNodePosi(T) b)
{
	_size++;
	return p->insertAsSuccAbove(e, b);
}
