#include "../../db1-4/List.hpp"
#include "../Entry.hpp"
#include "../QuadList.h"
//skiplistδ����

template <typename K,typename V>
class Skiplist : public Dictionary<K, V>, public List< Quadlist<Entry<K, V>>* > {
protected:
	bool skipSearch(ListNode< Quadlist<Entry<K, V>>* >*& qlist,
		QuadlistNode<Entry<K, V>>*& p,
		K& k);
public:
	int size() const { return empty() ? 0 : last()->data->size(); }
	int level() { return List::size(); }
	bool put(K k, V v);
	V* get(K k);
	bool remove(K k);
};

//qlistΪ�����б�,pΪqlist���׸��ڵ�,kΪ�ؼ���
//����:���ɹ�,pΪ���йؼ���Ķ����ڵ�,qlistΪp���ڵ��б�
		//��ʧ��,pΪ������k�����(���)�ؼ������ײ�,qlistΪ��
//�������ȡ�������������
template<typename K, typename V>
bool Skiplist<K, V>::skipSearch(
	ListNode<Quadlist<Entry<K, V>>*>* &qlist,
	QuadlistNode<Entry<K, V>>* &p,
	K& k ) {
	//ע��:��������β�ڱ�,quadlistͬ������β�ڱ�,ע��߽紦��
	while (true) {
		//�����ҵ���һ������k��λ��
		while (p->succ && (p->entry.data <= k))
			p = p->succ;
		p = p->pred;//����һ��
		//��ֹpΪ�߽�ڵ�,�����е�NULL(0)�����ݱȽ�,���p������ǰ��(�ڱ�����NULL)
		if (p->pred && (p->entry.key == k)) return true;
		qlist = qlist->succ;//������һ��
		if (!qlist->succ) return false;//pΪ�����trailer,û����һ��Ԫ��,����ʧ��
		p = (p->pred) ? p->below : qlist->data.first();//p����ʵ���ݽڵ�ʱ,����below�ڵ�;pΪ�ڱ�ʱ��û��below,ֻ��ͨ��qlistѰ����һ���ڵ�
	}
}



template<typename K, typename V>
V* Skiplist<K, V>::get(K k)
{
	if (empty()) return NULL;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
	Quadlist<Entry<K, V>>* p = qlist->data->first();
	return skipSearch(qlist, p, k) ? &(p->entry.value) : NULL;
}

template<typename K, typename V>
bool Skiplist<K, V>::remove(K k)
{
	if (empty()) return false;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();
	if (!skipSearch(qlist, p, k)) return false;
	do {
		QuadlistNode<Entry<K, V>>* b=p->below;
		qlist->data->remove(p);//quadlist��ɾ��quadlistNode�ڵ�p
		//ת���²�
		p = b;
		qlist = qlist->succ;
	} while (qlist->succ);
	//ɾ����ɺ�,�߶ȿ��ܻ��½�,�Զ����²鿴ÿ���Ƿ���Ԫ��
	while (!empty() && first()->data->empty())//�鿴����Quadlist�Ƿ�Ϊ��,ɾ���ܷ�����first��
		List::remove(first());
	return true;
}

//����entry(k,v)
template<typename K, typename V>
bool Skiplist<K, V>::put(K k, V v)
{
	Entry<K, V> e = Entry<K, V>(k, v);
	//�������Ϊ��,���²���һ��
	if (empty()) insertAsFirst(new Quadlist<Entry<K,V>>);
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();
	if (skipSearch(qlist, p, k))//�����Ƿ���k��Ԫ��
		while (p->below) p = p->below;//�Ѿ��йؼ���k,ת��k������
	qlist = last();//ת�����
	QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove(e, p);//��p�����Ԫ��e,�˴�Ϊ�½����Ļ���
	while (rand() % 1) {//�½���
		//Ϊ��ȷ���½�¥���Ѵ���,�ڸ�¥����Ѱ�ҽڵ�
		//��ǰѰ��,�����ڴ�¥������ǰ��(���ǰ�����ϲ�),���ҵ���ֱ�Ӵ�����¥��ڵ�,���򴴽�������ٴ����ڵ�
		while (qlist->data.valid(p) && !p->above) p = p->pred;
		if (!qlist->data.valid(p)) {//��ǰ����header�ڱ�
			if (qlist == first())
				insertAsFirst(new Quadlist<Entry<K,V>>);//�����µ�һ��
			p = qlist->pred->data->first()->pred;//pΪ�ϲ��header
		}
		else {
			p = p->above;
		}
		qlist = qlist->pred;
		b = qlist->data->insertAfterAbove(e, p, b);
	}
}
