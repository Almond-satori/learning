#include "adjacencyMatrix.hpp"

//prim+���ȼ����pfs
template <typename Tv, typename Te> struct PrimPU {
	virtual void operator() (Graph<Tv, Te>* g, int s, int u) {
		if( g->status(u) == UNDISCOVERED )
			//���ձߵ�Ȩֵ���ڵ㸳���ȼ�,���ȼ�Ĭ��ΪINT_MAX
			if (g->priority(u) > g->weight(s, u)) {
				g->priority(u) = g->weight(s, u);
				g->parent(u) = s;//���¸��ڵ�
			}
	}
};

//dijkstra
template <typename Tv, typename Te> struct dijkstraPU {
	virtual void operator() (Graph<Tv, Te>* g, int s, int u) {
		if (g->status(u) == UNDISCOVERED)
			//���ձߵ�Ȩֵ���ڵ㸳���ȼ�,���ȼ�Ĭ��ΪINT_MAX
			if (g->priority(u) > g->weight(s, u) + g->priority(s) ) {
				g->priority(u) = g->weight(s, u) + g->priority(s);
				g->parent(u) = s;//���¸��ڵ�
			}
	}
};

/*
int main() {
	GraphMatrix<char, int> gm;
	//��p155(b) digraphΪ��
	//gm.insert('a');//0
	//gm.insert('b');//1
	//gm.insert('c');//2
	//gm.insert('d');//3
	//gm.insert(0, 1, 0, 2);
	//gm.insert(1, 2, 1, 0);
	//gm.insert(2, 2, 1, 2);
	//gm.insert(3, 2, 1, 3);
	
	//Ϊ�˲�����������,ע�͵���
	//gm.insert(4, 3, 2, 0); //c->a��
	//gm.insert(5, 4, 3, 1); //d->b��
	
	//gm.printVertexAndEdge();
	////gm.bfs(0);
	//gm.dfs(0);
	//gm.printVertexAndEdge();
	//myStack<char>* s = gm.tSort(0);
	//while (!s->empty())
	//	cout << s->pop() << endl;

	//����ͼ,������ͨ��ֽ�
	//p168 6.14˫��ͨ��
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