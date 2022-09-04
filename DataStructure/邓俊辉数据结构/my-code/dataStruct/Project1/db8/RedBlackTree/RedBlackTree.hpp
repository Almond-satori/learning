#include "../../db7/BST.hpp"

//����δ���������

#define IsBlack(p) ( !(p) || ((p)->color == RB_BLACK)) //�ڽڵ���������,û�����úڽڵ��Ϊ����
#define IsRed(p) (!IsBlack(p)) //�Ǻڼ���
//heightָ�ڽڵ�ĸ߶�,��Ҫ���µ����Ϊ,1.���������߶Ȳ���;2.��ڵ�ĺڸ߶Ⱥ��亢�ӵĲ���;3.�ڽڵ�߶����亢�Ӹ߶�+1�����
#define BlackHeightUpdated(x)(\
	( (stature((x).lc)) == stature((x).rc) ) && \
	( (x).height == ( (IsRed(&x)) ? stature((x).lc) : stature((x).lc)+1 )) \
)

template <typename T> class RedBlackTree : public BST<T> {
protected:
	void solveDoubleRed(BinNodePosi(T) x);//˫������
	void solveDoubleBlack(BinNodePosi(T) r);//˫������
	int updateHeight(BinNodePosi(T) x);//����x�ڵ�߶�
public:
	BinNodePosi(T) insert(const T& e);//��д����
	bool remove(const T& e);//��дɾ��
};

//�ڵ�Ϊ��,�߶ȼ�1
template<typename T>
int RedBlackTree<T>::updateHeight(BinNodePosi(T) x)
{
	x->height = max(stature(x->lc), stature(x->rc));
	return IsBlack(x) ? x->height++ : x->height;
}

//��Ҷ�ӽڵ�λ���ϲ���һ���µĺ���
template<typename T>
BinNodePosi(T) RedBlackTree<T>::insert(const T& e)
{
	BinNodePosi(T)& x = search(e);//_hotָ��e�ĸ���,xΪ_hot������(lc����rc)
	if (x) return x;
	x = new myBinNode<T>(e,_hot,NULL,NULL,-1);//�������,�ڸ߶�Ϊ-1,���Һ���ΪNULL�����Һ���Ϊ�ڽڵ�
	_size++;
	solveDoubleRed(x);//�����ں�ڵ㴦�ֲ������,��˽���˫������
	return x ? x : _hot->parent;
}

template<typename T>
bool RedBlackTree<T>::remove(const T& e)
{
	BinNodePosi(T)& x = search(e); if (!x) return false;
	//rΪɾ���ڵ��ֱ�Ӻ��
	BinNodePosi(T) r = removeAt(x, _hot); if (!(--_size)) return true;
	if (!_hot) {//ɾ�����Ǹ��ڵ�,r�����˸��ڵ�
		_root->color = RB_BLACK; updateHeight(_root); return true;
	}
	//�߶Ȳ�ʧ��
	if (BlackHeightUpdated(*_hot)) return true;
	if (IsRed(r)) {//ֱ�Ӻ��Ϊ��ɫ
		r->color = RB_BLACK;
		r->height++;
		return true;
	}
	solveDoubleBlack(r);
	return true;
}

//˫������
template<typename T>
void RedBlackTree<T>::solveDoubleRed(BinNodePosi(T) x)
{
	//�ݹ��
	if (IsRoot(*x)){
		_root->color = RB_BLACK;
		_root->height++;
		return;
	}
	BinNodePosi(T) p = x->parent;
	BinNodePosi(T) g = p->parent;
	BinNodePosi(T) u = uncle(x);
	if ( IsBlack(u) ) {//uncleΪ�ڽڵ�
		//Ⱦɫ
		if( IsLChild(*p) == IsLChild(*x) ) //���ӳ���һ��
			p->color = RB_BLACK;
		else //���ӳ�������
			x->color = RB_BLACK;
		g->color = RB_RED;
		BinNodePosi(T) gg = g->parent;//�����ϲ�ڵ�
		BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);//���������˳�����xΪ���ӵ�������ƽ����ת
		r->parent = gg;
	}
	else {//��ڵ�����
		p->color = RB_BLACK; p->height++;//ע�����ø߶ȱ仯,��һ������ĸ߶ȱ仯����ת����������
		u->color = RB_BLACK; u->height++;
		if (!IsRoot(*g)) g->color = RB_RED;
		solveDoubleRed(g);
	}
}

template <typename T>
void RedBlackTree<T>::solveDoubleBlack(BinNodePosi(T) r) {
	BinNodePosi(T) p = r ? r->parent : _hot;
	if (!p) return;
	BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;//s��ԭ����x���ֵ�,����������r���ֵ�
	if (IsBlack(s) ) {//sΪ��
		BinNodePosi(T) t = NULL;//t��s�ĺ캢��,���Ҷ��Ǻ캢��,���ȿ�������
		if (IsRed(s->rc)) t = s->rc;
		if (IsRed(s->lc)) t = s->lc;
		if (t) { //s�к캢��,BB-1
			RBColor pOldColor = p->color;
			BinNodePosi(T) newRoot = FromParentTo(*p) = rotateAt(t);//rotateAt�����µĸ��ڵ�
			if (newRoot->lc) { newRoot->lc->color = RB_BLACK; updateHeight(newRoot->lc); }
			if (newRoot->rc) { newRoot->rc->color = RB_BLACK; updateHeight(newRoot->rc); }
			newRoot->color = pOldColor;
			updateHeight(newRoot);
		}
		else { //sû�к캢��,BB-2
			s->color = RB_Red; s->height--;
			if (IsRed(p)) { //BB-2-R
				p->color = RB_BLACK;//s�Ӻ�Ⱦ��,p�Ӻ�Ⱦ��,s��p�ĺ���,����p�߶Ȳ���
			}
			else { //BB-2-B
				p->height--;
				solveDoubleBlack(p);
			}
		}
	}
	else {//sΪ��,BB-3
		s->color = RB_BLACK; p->color = RB_RED;
		BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc;//ȡt��sͬ��
		_hot = p;//pΪr�ĸ���
		FromParentTo(*p) = rotateAt(t);
		solveDoubleBlack(r);//ת��bb-1,bb-2r 
	}

}
