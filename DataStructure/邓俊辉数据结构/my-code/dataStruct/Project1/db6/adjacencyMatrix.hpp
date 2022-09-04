#include "Graph.hpp"
#include <iostream>

//图的邻接矩阵实现

template <typename Tv> struct Vertex {//顶点对象
	Tv data; int inDegree, outDegree; VStatus status;
	int dTime, fTime;//时间标签
	int parent; int priority;//在遍历树中的父节点和优先级数
	Vertex(Tv const& d = (Tv)0) :	//构造新节点 
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};

template <typename Te> struct Edge {
	Te data; int weight; EType type;//数据,权重,类型
	Edge(Te const& d, int w) :data(d), weight(w), type(UNDETERMINED) {}//构造函数
};

template <typename Tv, typename Te>//顶点类型,边类型
class GraphMatrix : public Graph<Tv, Te> { //基于向量,以邻接矩阵形式实现的图
private:
	Vector<Vertex<Tv>> V;//顶点集,向量中存放所有的顶点
	Vector<Vector<Edge<Te>*>> E;//边集,邻接矩阵,存放所有边的引用
public:
	GraphMatrix() { Graph<Tv, Te>::n = Graph<Tv, Te>::e = 0; }
	~GraphMatrix() {
		for (int j = 0; j < Graph<Tv, Te>::n; j++)
			for (int k = 0; k < Graph<Tv, Te>::n; k++)
				delete E[j][k];
	}
	//顶点的基本操作:查询第i个顶点
	virtual Tv& vertex(int i) { return V[i].data; }
	virtual int inDegree(int i) { return V[i].inDegree; }//获取顶点v的入度(该顶点必须存在)
	virtual int outDegree(int i) { return V[i].outDegree; }//获取顶点v的出度(该顶点必须存在)
	virtual int firstNbr(int i) { return nextNbr(i, Graph<Tv, Te>::n); }//获取顶点v的首个邻接节点
	virtual int nextNbr(int i, int j) {
		while ((j > -1) && (!exists(i, --j)));
		return j;
	}
	virtual VStatus& status(int i) { return V[i].status; }//获取顶点v的状态
	virtual int& dTime(int i) { return V[i].dTime; }//顶点v的时间标签dTime
	virtual int& fTime(int i) { return V[i].fTime; }//顶点v的时间标签fTime
	virtual int& parent(int i) { return V[i].parent; }//顶点v在遍历树中的父亲
	virtual int& priority(int i) { return V[i].priority; }//顶点v在遍历树中的优先级数
//顶点的动态操作
	//插入节点
	virtual int insert(Tv const& vertex) {
		for (int j = 0; j < Graph<Tv, Te>::n; j++) E[j].insert(NULL);//二维数组中每一行预留一列
		Graph<Tv, Te>::n++;
		Vector < Edge<Te>* > edge = Vector < Edge<Te>*>(Graph<Tv, Te>::n, Graph<Tv, Te>::n, (Edge<Te>*)NULL);//创建新节点对应的边向量,data=n,weight=n
		E.insert(edge);//将新的一行插入
		return V.insert(Vertex<Tv>(vertex));
	}
	//删除节点
	virtual Tv remove(int i) {
		//边的删除,考虑删除节点的行和列(对应删除节点的出度和入度),释放删除节点的内存,修改其他节点的出入度
		//删除节点的所有出边
		for (int j = 0; j < Graph<Tv, Te>::n; j++)
			if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; }//逐个释放内存空间
		E.remove(i); Graph<Tv, Te>::n--;//删除一整行
		//删除节点的所有入边
		for (int j = 0; j < Graph<Tv, Te>::n; j++)
			if (Edge<Te>* e = E[j].remove(i)) {//删除j行i列,如果不存在继续循环,存在则释放内存
				delete e;
				V[j].outDegree--;
			}
		//顶点的删除
		Tv vBak = vertex(i); V.remove(i);//删除顶点i
		return vBak;
	}
	//边的确认操作
	virtual bool exists(int i, int j) { //判断边(i,j)是否存在
		return (i >= 0 && j >= 0 && i < Graph<Tv, Te>::n&& j < Graph<Tv, Te>::n && (E[i][j] != NULL));
	}
	//边的基本操作
	virtual EType& type(int i, int j) { return E[i][j]->type; }//获取边(v,u)的类型
	virtual Te& edge(int i, int j) { return E[i][j]->data; }//边(v,u)的数据,该边必须存在
	virtual int& weight(int i, int j) { return E[i][j]->weight; }//边(v,u)的权重
//边的动态操作
	//在顶点v,u之间插入权重为w的边e
	virtual void insert(Te const& edge, int w, int i, int j) {
		if (exists(i, j)) return;//只能插入不存在的边
		E[i][j] = new Edge<Te>(edge, w);
		Graph<Tv, Te>::e++;
		V[j].inDegree++;
		V[i].outDegree++;
	}
	//删除顶点v和u之间的边e,返回该边的信息
	virtual Te remove(int i, int j) {
		Te eBak = edge(i, j);
		delete E[i][j]; //释放E[i][j]的edge指针
		E[i][j] = NULL;//重新将E[i][j]赋值为空
		Graph<Tv, Te>::e--; V[i].outDegree--; V[j].inDegree--;
		return eBak;
	}
	void printVertexAndEdge() {
		for (int i = 0; i < Graph<Tv, Te>::n; i++)
		{
			cout << "节点" << V[i].data << ":" << endl;
			cout << "状态" << V[i].status  << endl;
			cout << "dTime" << V[i].dTime  << endl;
			cout<<"父节点" << V[i].parent  << endl;
			for (int j = 0; j < Graph<Tv, Te>::n ; j++) {
				if(E[i][j] != NULL)
					cout << "边(" <<i<<","<<j<<"):" << E[i][j]->data << "type:"<<E[i][j]->type << endl;
			}
		}
	}


};






