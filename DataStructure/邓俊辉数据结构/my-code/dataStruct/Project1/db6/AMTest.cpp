#include "adjacencyMatrix.hpp"

//prim+优先级框架pfs
template <typename Tv, typename Te> struct PrimPU {
	virtual void operator() (Graph<Tv, Te>* g, int s, int u) {
		if( g->status(u) == UNDISCOVERED )
			//按照边的权值给节点赋优先级,优先级默认为INT_MAX
			if (g->priority(u) > g->weight(s, u)) {
				g->priority(u) = g->weight(s, u);
				g->parent(u) = s;//更新父节点
			}
	}
};

//dijkstra
template <typename Tv, typename Te> struct dijkstraPU {
	virtual void operator() (Graph<Tv, Te>* g, int s, int u) {
		if (g->status(u) == UNDISCOVERED)
			//按照边的权值给节点赋优先级,优先级默认为INT_MAX
			if (g->priority(u) > g->weight(s, u) + g->priority(s) ) {
				g->priority(u) = g->weight(s, u) + g->priority(s);
				g->parent(u) = s;//更新父节点
			}
	}
};

/*
int main() {
	GraphMatrix<char, int> gm;
	//以p155(b) digraph为例
	//gm.insert('a');//0
	//gm.insert('b');//1
	//gm.insert('c');//2
	//gm.insert('d');//3
	//gm.insert(0, 1, 0, 2);
	//gm.insert(1, 2, 1, 0);
	//gm.insert(2, 2, 1, 2);
	//gm.insert(3, 2, 1, 3);
	
	//为了测试拓扑排序,注释掉环
	//gm.insert(4, 3, 2, 0); //c->a环
	//gm.insert(5, 4, 3, 1); //d->b环
	
	//gm.printVertexAndEdge();
	////gm.bfs(0);
	//gm.dfs(0);
	//gm.printVertexAndEdge();
	//myStack<char>* s = gm.tSort(0);
	//while (!s->empty())
	//	cout << s->pop() << endl;

	//无向图,测试连通域分解
	//p168 6.14双连通域
	GraphMatrix<char, int> gm2;
	gm2.insert('a');//0
	gm2.insert('b');//1
	gm2.insert('c');//2
	gm2.insert('d');//3
	gm2.insert('e');//4
	gm2.insert('f');//5
	gm2.insert('g');//6
	
	gm2.insert(0, 4, 0, 1);
	gm2.insert(0, 4, 1, 0);
	gm2.insert(0, 2, 1, 3);
	gm2.insert(0, 2, 3, 1);
	gm2.insert(0, 1, 2, 3);
	gm2.insert(0, 1, 3, 2);
	gm2.insert(0, 2, 0, 2);
	gm2.insert(0, 2, 2, 0);

	gm2.insert(0, 1, 3, 4);
	gm2.insert(0, 1, 4, 3);

	gm2.insert(0, 1, 4, 5);
	gm2.insert(0, 1, 5, 4);
	gm2.insert(0, 1, 5, 6);
	gm2.insert(0, 1, 6, 5);
	gm2.insert(0, 1, 4, 6);
	gm2.insert(0, 1, 6, 4);

	//gm2.bcc(0);
	PrimPU<char, int> prim;
	dijkstraPU<char, int> dijkstra;
	gm2.pfs(1, dijkstra);
	return 0;
}
*/