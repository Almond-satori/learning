#include "4-1-stack.hpp"

struct	Queen {
	int x, y;
	Queen(int ax = 0, int by = 0) : x(ax), y(by) {};
	bool operator== (Queen const& q) const {
		return q.x == x || q.y == y || q.x + q.y == x + y || q.x - q.y == x - y;//�жϻʺ��Ƿ��г�ͻ
	}
	bool operator!= (Queen const& q) const {
		return !(*this == q);
	}
};

//���Է���queen
int placeQueen(int N) {
	int nCheck = 0;
	int nSolu = 0;//�ⷨ����
	myStack<Queen>solu;//����ÿһ�еķ��õ�queen
	Queen q(0, 0);
	//�����������б���
	do {
		if (q.x >= N || q.y >= N) {
			q = solu.pop(); q.y++;//����һ��,����һ���е���һ�з���queen
		}
		else {//���ֲ����˾���̽���е�queen
			while (q.y < N && solu.find(q) >= 0 ) q.y++;//�ڱ��ж�Ӧ�Ϸ����Ϸ���queen,�����ͻ(find >=0)�ͽ���ѭ��
			if (q.y < N) {//�������Ϊ����ͻ���˳�ѭ�������
				solu.push(q); 
				if (solu.size() >= N) nSolu++;//ȫ�ֽ��ҵ���,��һ��ѭ�����𲽺���
				q.x++; q.y = 0;//Ѱ����һ�е�queen
			}
		}
	} while (q.x > 0 || q.y < N);//��������������������ѭ��(��0,0��ʼ���ҵ�һ����,�����˲�Ѱ��,�����˵���0�е�N��ʱ����)
	return nSolu;
}

//int main() {
//	int x = placeQueen(11);
//	cout << x << endl;
//	return 0;
//}

