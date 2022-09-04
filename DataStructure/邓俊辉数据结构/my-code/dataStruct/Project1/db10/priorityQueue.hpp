template <typename T> struct PQ { //优先级队列模板类
	virtual void insert(T) = 0; //按照比较器确定的优先级进行词条的插入
	virtual T getMax() = 0; //取出优先级最高的词条
	virtual T delMax() = 0; //删除优先级最高的词条
};

#define InHeap(n,i) ( ( ( -1 ) < ( i ) ) && ( ( i ) < ( n ) ) ) //判断PQ[i]是否合法
#define Parent(i) ( (i-1) >> 1) //节点i的父节点为 (i-1)/2取下整
#define LastInternal(n) Parent(n-1) //最后一个内部节点,即末节点的父亲
#define LChild(i) ( 1 + ( ( i ) << 1 ) ) //左孩子
#define RChild(i) ( 2 + ( ( i ) << 1 ) ) //右孩子
#define ParentValid(i) ( 0 < i ) //判断PQ[i]是否有父亲
#define LChildValid(n,i) InHeap(n,LChild(i)) //判断PQ[i]是否有一个左孩子
#define RChildValid(n,i) InHeap(n,RChild(i)) //判断PQ[i]是否有一个右孩子
#define Bigger(PQ,i,j) (lt(PQ[i],PQ[j]) ? j : i) //取大者
#define ProperParent(PQ,n,i) /*父子三者中的大者*/ \
	( RChildValid( n , i ) ? Bigger( PQ,Bigger( PQ,i,LChild(i) ),RChild(i) ) : \
	( LChildValid( n , i) ? Bigger( PQ, Bigger(PQ,i,RChild),LChild(i) ) : i \
) \
) 


