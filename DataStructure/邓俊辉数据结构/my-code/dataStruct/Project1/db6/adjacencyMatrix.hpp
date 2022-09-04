#include "Graph.hpp"
#include <iostream>

//ͼ���ڽӾ���ʵ��

template <typename Tv> struct Vertex {//�������
	Tv data; int inDegree, outDegree; VStatus status;
	int dTime, fTime;//ʱ���ǩ
	int parent; int priority;//�ڱ������еĸ��ڵ�����ȼ���
	Vertex(Tv const& d = (Tv)0) :	//�����½ڵ� 
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};

template <typename Te> struct Edge {
	Te data; int weight; EType type;//����,Ȩ��,����
	Edge(Te const& d, int w) :data(d), weight(w), type(UNDETERMINED) {}//���캯��
};

template <typename Tv, typename Te>//��������,������
class GraphMatrix : public Graph<Tv, Te> { //��������,���ڽӾ�����ʽʵ�ֵ�ͼ
private:
	Vector<Vertex<Tv>> V;//���㼯,�����д�����еĶ���
	Vector<Vector<Edge<Te>*>> E;//�߼�,�ڽӾ���,������бߵ�����
public:
	GraphMatrix() { Graph<Tv, Te>::n = Graph<Tv, Te>::e = 0; }
	~GraphMatrix() {
		for (int j = 0; j < Graph<Tv, Te>::n; j++)
			for (int k = 0; k < Graph<Tv, Te>::n; k++)
				delete E[j][k];
	}
	//����Ļ�������:��ѯ��i������
	virtual Tv& vertex(int i) { return V[i].data; }
	virtual int inDegree(int i) { return V[i].inDegree; }//��ȡ����v�����(�ö���������)
	virtual int outDegree(int i) { return V[i].outDegree; }//��ȡ����v�ĳ���(�ö���������)
	virtual int firstNbr(int i) { return nextNbr(i, Graph<Tv, Te>::n); }//��ȡ����v���׸��ڽӽڵ�
	virtual int nextNbr(int i, int j) {
		while ((j > -1) && (!exists(i, --j)));
		return j;
	}
	virtual VStatus& status(int i) { return V[i].status; }//��ȡ����v��״̬
	virtual int& dTime(int i) { return V[i].dTime; }//����v��ʱ���ǩdTime
	virtual int& fTime(int i) { return V[i].fTime; }//����v��ʱ���ǩfTime
	virtual int& parent(int i) { return V[i].parent; }//����v�ڱ������еĸ���
	virtual int& priority(int i) { return V[i].priority; }//����v�ڱ������е����ȼ���
//����Ķ�̬����
	//����ڵ�
	virtual int insert(Tv const& vertex) {
		for (int j = 0; j < Graph<Tv, Te>::n; j++) E[j].insert(NULL);//��ά������ÿһ��Ԥ��һ��
		Graph<Tv, Te>::n++;
		Vector < Edge<Te>* > edge = Vector < Edge<Te>*>(Graph<Tv, Te>::n, Graph<Tv, Te>::n, (Edge<Te>*)NULL);//�����½ڵ��Ӧ�ı�����,data=n,weight=n
		E.insert(edge);//���µ�һ�в���
		return V.insert(Vertex<Tv>(vertex));
	}
	//ɾ���ڵ�
	virtual Tv remove(int i) {
		//�ߵ�ɾ��,����ɾ���ڵ���к���(��Ӧɾ���ڵ�ĳ��Ⱥ����),�ͷ�ɾ���ڵ���ڴ�,�޸������ڵ�ĳ����
		//ɾ���ڵ�����г���
		for (int j = 0; j < Graph<Tv, Te>::n; j++)
			if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; }//����ͷ��ڴ�ռ�
		E.remove(i); Graph<Tv, Te>::n--;//ɾ��һ����
		//ɾ���ڵ���������
		for (int j = 0; j < Graph<Tv, Te>::n; j++)
			if (Edge<Te>* e = E[j].remove(i)) {//ɾ��j��i��,��������ڼ���ѭ��,�������ͷ��ڴ�
				delete e;
				V[j].outDegree--;
			}
		//�����ɾ��
		Tv vBak = vertex(i); V.remove(i);//ɾ������i
		return vBak;
	}
	//�ߵ�ȷ�ϲ���
	virtual bool exists(int i, int j) { //�жϱ�(i,j)�Ƿ����
		return (i >= 0 && j >= 0 && i < Graph<Tv, Te>::n&& j < Graph<Tv, Te>::n && (E[i][j] != NULL));
	}
	//�ߵĻ�������
	virtual EType& type(int i, int j) { return E[i][j]->type; }//��ȡ��(v,u)������
	virtual Te& edge(int i, int j) { return E[i][j]->data; }//��(v,u)������,�ñ߱������
	virtual int& weight(int i, int j) { return E[i][j]->weight; }//��(v,u)��Ȩ��
//�ߵĶ�̬����
	//�ڶ���v,u֮�����Ȩ��Ϊw�ı�e
	virtual void insert(Te const& edge, int w, int i, int j) {
		if (exists(i, j)) return;//ֻ�ܲ��벻���ڵı�
		E[i][j] = new Edge<Te>(edge, w);
		Graph<Tv, Te>::e++;
		V[j].inDegree++;
		V[i].outDegree++;
	}
	//ɾ������v��u֮��ı�e,���ظñߵ���Ϣ
	virtual Te remove(int i, int j) {
		Te eBak = edge(i, j);
		delete E[i][j]; //�ͷ�E[i][j]��edgeָ��
		E[i][j] = NULL;//���½�E[i][j]��ֵΪ��
		Graph<Tv, Te>::e--; V[i].outDegree--; V[j].inDegree--;
		return eBak;
	}
	void printVertexAndEdge() {
		for (int i = 0; i < Graph<Tv, Te>::n; i++)
		{
			cout << "�ڵ�" << V[i].data << ":" << endl;
			cout << "״̬" << V[i].status  << endl;
			cout << "dTime" << V[i].dTime  << endl;
			cout<<"���ڵ�" << V[i].parent  << endl;
			for (int j = 0; j < Graph<Tv, Te>::n ; j++) {
				if(E[i][j] != NULL)
					cout << "��(" <<i<<","<<j<<"):" << E[i][j]->data << "type:"<<E[i][j]->type << endl;
			}
		}
	}


};






