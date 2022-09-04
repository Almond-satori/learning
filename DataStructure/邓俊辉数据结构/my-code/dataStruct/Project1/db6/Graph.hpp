#include "../db1-4/4-2-queue.hpp"
#include "../db1-4/4-1-stack.hpp"
#include <limits.h>

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;//顶点状态
typedef enum {UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} EType;//边状态

template <typename Tv,typename Te>//vertex和edge的类型
class Graph {
private:
	void reset() {	//将所有顶点和边的辅助信息复位
		for (int i = 0; i < n; i++)	//遍历所有顶点
		{
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1;
			parent(i) = -1;  priority(i) = INT_MAX;//优先级设置为最低(数值最大)
			for (int j = 0; j < n; j++) {
				if (exists(i, j)) type(i, j) = UNDETERMINED;
			}
		}
	}
	void BFS(int, int&);//连通域广度优先搜索
	void DFS(int, int&);//连通域深度优先搜索
	void BCC(int, int&, myStack<int>&);//连通域基于DFS的双连通分量分解算法
	bool Tsort(int, myStack<Tv>*);//连通域基于DFS的拓扑排序算法
	template <typename PU> void PFS(int, PU);//连通域优先搜索框架
public:
//顶点
	int n;//顶点个数
	virtual int insert(Tv const&) = 0;//形如virtual void functionName() = 0;的函数是一个纯虚函数
	virtual Tv remove(int) = 0;//删除顶点以及其关联边,返回该顶点信息
	virtual Tv& vertex(int) = 0;//获取顶点v的数据(该顶点必须存在)
	virtual int inDegree(int) = 0;//获取顶点v的入度(该顶点必须存在)
	virtual int outDegree(int) = 0;//获取顶点v的出度(该顶点必须存在)
	virtual int firstNbr(int) = 0;//获取顶点v的首个邻接节点
	virtual int nextNbr(int v,int j) = 0;//获取顶点v的相对于节点j的下一邻接节点
	virtual VStatus& status(int) = 0;//获取顶点v的状态
	virtual int& dTime(int) = 0;//顶点v的时间标签dTime
	virtual int& fTime(int) = 0;//顶点v的时间标签fTime
	virtual int& parent(int) = 0;//顶点v在遍历树中的父亲
	virtual int& priority(int) = 0;//顶点v在遍历树中的优先级数
//边:约定无向图等于各个节点之间以双箭头连接的有向图
	int e;//总边数
	virtual bool exists(int, int) = 0;//判断边(v,u)是否存在
	virtual void insert(Te const&, int, int, int) = 0;//在顶点v,u之间插入权重为w的边e
	virtual Te remove(int, int) = 0;//删除顶点v和u之间的边e,返回该边的信息
	virtual EType& type(int, int) = 0;//获取边(v,u)的类型
	virtual Te& edge(int, int) = 0;//边(v,u)的数据,该边必须存在
	virtual int& weight(int, int) = 0;//边(v,u)的权重
//算法
	void bfs(int);//广度优先搜索
	void dfs(int);//深度优先搜索
	void bcc(int);//基于DFS的双连通分量分解算法
	myStack<Tv>* tSort(int);//基于DFS的拓扑排序算法
	//void prim(int);//最小支撑树prim算法
	//void dijkstra(int);//最短路径Dijkstra算法
	template <typename PU> void pfs(int, PU);//优先级搜索框架
};

template<typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s)
{
	Graph<Tv, Te>::reset();//复位所有节点信息
	int clock = 0; int v = s;//初始化
	do {
		if (status(v) == UNDISCOVERED) {
			BFS(v, clock);
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//在节点列表循环访问,防止有未连通的节点漏掉
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int v, int& clock)
{
	myQueue<int> queue;
	status(v) = DISCOVERED; queue.enqueue(v);
	while (!queue.empty()) {
		int v = queue.dequeue(); dTime(v) = ++clock;
		for (int u = firstNbr(v); u > -1; u = nextNbr(v, u))
		{
			if (status(u) == UNDISCOVERED) {//邻居u尚未被发现
				status(u) = DISCOVERED; queue.enqueue(u);
				type(v, u) = TREE; parent(u) = v;//设置边的类型为树边
			}
			else {
				type(v, u) = CROSS;//将vu边设置为跨边(有多个节点指向u的情况,与树边相对应)
			}
		}
		status(v) = VISITED;
	}
}

template<typename Tv, typename Te>
myStack<Tv>* Graph<Tv, Te>::tSort(int s)
{
	Graph<Tv, Te>::reset();//复位所有节点信息
	int v = s;//初始化
	myStack<Tv>* stack = new myStack<Tv>;
	do {
		if (status(v) == UNDISCOVERED) {
			if (!Tsort(v, stack)) {
				while (!stack->empty()) //当存在环路,弹出所有元素
					stack->pop(); 
				break;
			}
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//在节点列表循环访问,防止有未连通的节点漏掉
	return stack;
}

template<typename Tv, typename Te>
bool Graph<Tv, Te>::Tsort(int v, myStack<Tv>* stack) {
	status(v) = DISCOVERED;
	for (int u = firstNbr(v); u > -1; u = nextNbr(v,u)) {
		switch (status(u))
		{
		case UNDISCOVERED:
			if (!Tsort(u, stack)) return false;
			break; 
		case DISCOVERED://u已经被发现,但未进行深入访问
			//说明在进行深度优先搜索时指向前面已经探索的节点了
			return false;
		default://u已经被访问VISITED,根据dTime判断是前向边还是跨边(dTime记录访问次序)
			break;
		}
	}
	status(v) = VISITED; stack->push(vertex(v));
	return true;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s)
{
	Graph<Tv, Te>::reset();//复位所有节点信息
	int clock = 0; int v = s;//初始化
	do {
		if (status(v) == UNDISCOVERED) {
			DFS(v, clock);
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//在节点列表循环访问,防止有未连通的节点漏掉
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int& clock) {
	dTime(v) = ++clock; status(v) = DISCOVERED;
	for (int u = firstNbr(v); u > -1; u = nextNbr(v, u)) {
		switch (status(u))
		{
		case UNDISCOVERED:
			type(v, u) = TREE; parent(u) = v; DFS(u, clock);
			break;
		case DISCOVERED://u已经被发现,但未进行深入访问
			//说明在进行深度优先搜索时指向前面已经探索的节点了
			type(v, u) = BACKWARD; break;
		default://u已经被访问VISITED,根据dTime判断是前向边还是跨边(dTime记录访问次序)
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED; fTime(v) = ++clock;
}


template<typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s)
{
	Graph<Tv, Te>::reset();//复位所有节点信息
	int clock = 0; int v = s;//初始化
	myStack<int> stack;
	do {
		if (status(v) == UNDISCOVERED) {
			BCC(v, clock,stack);
			int x =stack.pop();
			cout << x << "出栈";
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//在节点列表循环访问,防止有未连通的节点漏掉
}

#define hca(x) (fTime(x)) //借用没有用的fTime充当hca,用于记录子树可以指向的最高祖先(dTime数值更小的祖先)
template<typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int v, int& clock,myStack<int>& stack) {
	hca(v)=dTime(v) = ++clock; status(v) = DISCOVERED; stack.push(v);
	for (int u = firstNbr(v); u > -1; u = nextNbr(v, u)) {
		switch (status(u))
		{
		case UNDISCOVERED:
			type(v, u) = TREE; parent(u) = v; 
			BCC(u, clock,stack);
			//深度递归完成后,返回时
			if (hca(u) < dTime(v))//如果v的邻居u形成的子树有能力指向v的真祖先
				hca(v) = min(hca(v), hca(u));//更新v的hca
			else {//如果没有能力指向v的真祖先,考虑v是关键点
				int x;
				while (v != (x = stack.pop())) cout << x << "出栈";
				cout <<"---" << endl;
				stack.push(v);//将v入栈
			}
			break;
		case DISCOVERED://u已经被发现,但未进行深入访问
			//说明在进行深度优先搜索时指向前面已经探索的节点了
			type(v, u) = BACKWARD; 
			//v的父亲不是u的情况
			if (u != parent(v)) {
				hca(v) = min(hca(v), dTime(u));
			}
			break;
		default://u已经被访问VISITED,根据dTime判断是前向边还是跨边(dTime记录访问次序)
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; 
			break;
		}
	}
	status(v) = VISITED; 
}



template<typename Tv, typename Te>
template<typename PU>
void Graph<Tv, Te>::pfs(int s, PU prioUpdater)
{
	reset();//复位所有节点信息
	int v = s;//初始化
	do {
		if (status(v) == UNDISCOVERED) {
			PFS(v, prioUpdater);
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//在节点列表循环访问,防止有未连通的节点漏掉
}

template<typename Tv, typename Te>
template<typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater)
{
	priority(s) = 0; status(s) = VISITED; parent(s) = -1;//给根节点初始化
	while (1) {
		for (int u = firstNbr(s); u > -1; u = nextNbr(s, u)) 
			prioUpdater(this,s,u);
		//遍历所有的节点,找到下一个优先级最高(值最小的)元素,一定会找出prioUpdater中被设置的s的邻居,因为其他未访问节点的优先级都是INT_MAX
		for (int shortest = INT_MAX , next = 0; next < n; next++) {
			if( status(next) == UNDISCOVERED )
				if (priority(next) < shortest) {
					shortest = priority(next);
					s = next;
				}	
		}
		if (status(s) == VISITED) break;
		cout << s << endl;
		status(s) = VISITED; type(parent(s), s) = TREE;
	}
}