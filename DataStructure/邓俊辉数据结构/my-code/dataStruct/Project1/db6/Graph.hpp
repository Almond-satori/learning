#include "../db1-4/4-2-queue.hpp"
#include "../db1-4/4-1-stack.hpp"
#include <limits.h>

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;//����״̬
typedef enum {UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} EType;//��״̬

template <typename Tv,typename Te>//vertex��edge������
class Graph {
private:
	void reset() {	//�����ж���ͱߵĸ�����Ϣ��λ
		for (int i = 0; i < n; i++)	//�������ж���
		{
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1;
			parent(i) = -1;  priority(i) = INT_MAX;//���ȼ�����Ϊ���(��ֵ���)
			for (int j = 0; j < n; j++) {
				if (exists(i, j)) type(i, j) = UNDETERMINED;
			}
		}
	}
	void BFS(int, int&);//��ͨ������������
	void DFS(int, int&);//��ͨ�������������
	void BCC(int, int&, myStack<int>&);//��ͨ�����DFS��˫��ͨ�����ֽ��㷨
	bool Tsort(int, myStack<Tv>*);//��ͨ�����DFS�����������㷨
	template <typename PU> void PFS(int, PU);//��ͨ�������������
public:
//����
	int n;//�������
	virtual int insert(Tv const&) = 0;//����virtual void functionName() = 0;�ĺ�����һ�����麯��
	virtual Tv remove(int) = 0;//ɾ�������Լ��������,���ظö�����Ϣ
	virtual Tv& vertex(int) = 0;//��ȡ����v������(�ö���������)
	virtual int inDegree(int) = 0;//��ȡ����v�����(�ö���������)
	virtual int outDegree(int) = 0;//��ȡ����v�ĳ���(�ö���������)
	virtual int firstNbr(int) = 0;//��ȡ����v���׸��ڽӽڵ�
	virtual int nextNbr(int v,int j) = 0;//��ȡ����v������ڽڵ�j����һ�ڽӽڵ�
	virtual VStatus& status(int) = 0;//��ȡ����v��״̬
	virtual int& dTime(int) = 0;//����v��ʱ���ǩdTime
	virtual int& fTime(int) = 0;//����v��ʱ���ǩfTime
	virtual int& parent(int) = 0;//����v�ڱ������еĸ���
	virtual int& priority(int) = 0;//����v�ڱ������е����ȼ���
//��:Լ������ͼ���ڸ����ڵ�֮����˫��ͷ���ӵ�����ͼ
	int e;//�ܱ���
	virtual bool exists(int, int) = 0;//�жϱ�(v,u)�Ƿ����
	virtual void insert(Te const&, int, int, int) = 0;//�ڶ���v,u֮�����Ȩ��Ϊw�ı�e
	virtual Te remove(int, int) = 0;//ɾ������v��u֮��ı�e,���ظñߵ���Ϣ
	virtual EType& type(int, int) = 0;//��ȡ��(v,u)������
	virtual Te& edge(int, int) = 0;//��(v,u)������,�ñ߱������
	virtual int& weight(int, int) = 0;//��(v,u)��Ȩ��
//�㷨
	void bfs(int);//�����������
	void dfs(int);//�����������
	void bcc(int);//����DFS��˫��ͨ�����ֽ��㷨
	myStack<Tv>* tSort(int);//����DFS�����������㷨
	//void prim(int);//��С֧����prim�㷨
	//void dijkstra(int);//���·��Dijkstra�㷨
	template <typename PU> void pfs(int, PU);//���ȼ��������
};

template<typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s)
{
	Graph<Tv, Te>::reset();//��λ���нڵ���Ϣ
	int clock = 0; int v = s;//��ʼ��
	do {
		if (status(v) == UNDISCOVERED) {
			BFS(v, clock);
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//�ڽڵ��б�ѭ������,��ֹ��δ��ͨ�Ľڵ�©��
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
			if (status(u) == UNDISCOVERED) {//�ھ�u��δ������
				status(u) = DISCOVERED; queue.enqueue(u);
				type(v, u) = TREE; parent(u) = v;//���ñߵ�����Ϊ����
			}
			else {
				type(v, u) = CROSS;//��vu������Ϊ���(�ж���ڵ�ָ��u�����,���������Ӧ)
			}
		}
		status(v) = VISITED;
	}
}

template<typename Tv, typename Te>
myStack<Tv>* Graph<Tv, Te>::tSort(int s)
{
	Graph<Tv, Te>::reset();//��λ���нڵ���Ϣ
	int v = s;//��ʼ��
	myStack<Tv>* stack = new myStack<Tv>;
	do {
		if (status(v) == UNDISCOVERED) {
			if (!Tsort(v, stack)) {
				while (!stack->empty()) //�����ڻ�·,��������Ԫ��
					stack->pop(); 
				break;
			}
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//�ڽڵ��б�ѭ������,��ֹ��δ��ͨ�Ľڵ�©��
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
		case DISCOVERED://u�Ѿ�������,��δ�����������
			//˵���ڽ��������������ʱָ��ǰ���Ѿ�̽���Ľڵ���
			return false;
		default://u�Ѿ�������VISITED,����dTime�ж���ǰ��߻��ǿ��(dTime��¼���ʴ���)
			break;
		}
	}
	status(v) = VISITED; stack->push(vertex(v));
	return true;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s)
{
	Graph<Tv, Te>::reset();//��λ���нڵ���Ϣ
	int clock = 0; int v = s;//��ʼ��
	do {
		if (status(v) == UNDISCOVERED) {
			DFS(v, clock);
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//�ڽڵ��б�ѭ������,��ֹ��δ��ͨ�Ľڵ�©��
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
		case DISCOVERED://u�Ѿ�������,��δ�����������
			//˵���ڽ��������������ʱָ��ǰ���Ѿ�̽���Ľڵ���
			type(v, u) = BACKWARD; break;
		default://u�Ѿ�������VISITED,����dTime�ж���ǰ��߻��ǿ��(dTime��¼���ʴ���)
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED; fTime(v) = ++clock;
}


template<typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s)
{
	Graph<Tv, Te>::reset();//��λ���нڵ���Ϣ
	int clock = 0; int v = s;//��ʼ��
	myStack<int> stack;
	do {
		if (status(v) == UNDISCOVERED) {
			BCC(v, clock,stack);
			int x =stack.pop();
			cout << x << "��ջ";
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//�ڽڵ��б�ѭ������,��ֹ��δ��ͨ�Ľڵ�©��
}

#define hca(x) (fTime(x)) //����û���õ�fTime�䵱hca,���ڼ�¼��������ָ����������(dTime��ֵ��С������)
template<typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int v, int& clock,myStack<int>& stack) {
	hca(v)=dTime(v) = ++clock; status(v) = DISCOVERED; stack.push(v);
	for (int u = firstNbr(v); u > -1; u = nextNbr(v, u)) {
		switch (status(u))
		{
		case UNDISCOVERED:
			type(v, u) = TREE; parent(u) = v; 
			BCC(u, clock,stack);
			//��ȵݹ���ɺ�,����ʱ
			if (hca(u) < dTime(v))//���v���ھ�u�γɵ�����������ָ��v��������
				hca(v) = min(hca(v), hca(u));//����v��hca
			else {//���û������ָ��v��������,����v�ǹؼ���
				int x;
				while (v != (x = stack.pop())) cout << x << "��ջ";
				cout <<"---" << endl;
				stack.push(v);//��v��ջ
			}
			break;
		case DISCOVERED://u�Ѿ�������,��δ�����������
			//˵���ڽ��������������ʱָ��ǰ���Ѿ�̽���Ľڵ���
			type(v, u) = BACKWARD; 
			//v�ĸ��ײ���u�����
			if (u != parent(v)) {
				hca(v) = min(hca(v), dTime(u));
			}
			break;
		default://u�Ѿ�������VISITED,����dTime�ж���ǰ��߻��ǿ��(dTime��¼���ʴ���)
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
	reset();//��λ���нڵ���Ϣ
	int v = s;//��ʼ��
	do {
		if (status(v) == UNDISCOVERED) {
			PFS(v, prioUpdater);
		}
	} while (s != (v = (++v % Graph<Tv, Te>::n)));//�ڽڵ��б�ѭ������,��ֹ��δ��ͨ�Ľڵ�©��
}

template<typename Tv, typename Te>
template<typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater)
{
	priority(s) = 0; status(s) = VISITED; parent(s) = -1;//�����ڵ��ʼ��
	while (1) {
		for (int u = firstNbr(s); u > -1; u = nextNbr(s, u)) 
			prioUpdater(this,s,u);
		//�������еĽڵ�,�ҵ���һ�����ȼ����(ֵ��С��)Ԫ��,һ�����ҳ�prioUpdater�б����õ�s���ھ�,��Ϊ����δ���ʽڵ�����ȼ�����INT_MAX
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