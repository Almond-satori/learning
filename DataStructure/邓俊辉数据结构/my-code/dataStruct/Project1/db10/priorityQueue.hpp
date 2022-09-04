template <typename T> struct PQ { //���ȼ�����ģ����
	virtual void insert(T) = 0; //���ձȽ���ȷ�������ȼ����д����Ĳ���
	virtual T getMax() = 0; //ȡ�����ȼ���ߵĴ���
	virtual T delMax() = 0; //ɾ�����ȼ���ߵĴ���
};

#define InHeap(n,i) ( ( ( -1 ) < ( i ) ) && ( ( i ) < ( n ) ) ) //�ж�PQ[i]�Ƿ�Ϸ�
#define Parent(i) ( (i-1) >> 1) //�ڵ�i�ĸ��ڵ�Ϊ (i-1)/2ȡ����
#define LastInternal(n) Parent(n-1) //���һ���ڲ��ڵ�,��ĩ�ڵ�ĸ���
#define LChild(i) ( 1 + ( ( i ) << 1 ) ) //����
#define RChild(i) ( 2 + ( ( i ) << 1 ) ) //�Һ���
#define ParentValid(i) ( 0 < i ) //�ж�PQ[i]�Ƿ��и���
#define LChildValid(n,i) InHeap(n,LChild(i)) //�ж�PQ[i]�Ƿ���һ������
#define RChildValid(n,i) InHeap(n,RChild(i)) //�ж�PQ[i]�Ƿ���һ���Һ���
#define Bigger(PQ,i,j) (lt(PQ[i],PQ[j]) ? j : i) //ȡ����
#define ProperParent(PQ,n,i) /*���������еĴ���*/ \
	( RChildValid( n , i ) ? Bigger( PQ,Bigger( PQ,i,LChild(i) ),RChild(i) ) : \
	( LChildValid( n , i) ? Bigger( PQ, Bigger(PQ,i,RChild),LChild(i) ) : i \
) \
) 


