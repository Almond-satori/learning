#include "4-1-stack.hpp"

typedef enum {AVAILABLE,ROUTE,BACKTRACKED,WALL} Status;

typedef enum {UNKNOWN,EAST,SOUTH,WEST,NORTH,NO_WAY} Direction;

Direction nextDir(Direction dir) { return Direction(dir + 1); }//ö�����б��,+1����ָ����һ������

struct Cell { //ÿ��λ�õĽ��
	int x, y; Status status;//λ������,λ��״̬
	Direction in, out;//̽��·��ʱ,�ӷ���in���뱾���,�ӱ��������out����
};

#define LABY_MAX 24 //�Թ��ߴ�
Cell laby[LABY_MAX][LABY_MAX]; //�Թ�

//���ݱ�����out,��ȡ��Ӧ�����ϵ��ھӽ�㲢����
Cell* neighbor(Cell* cell) {
	switch (cell->out)
	{
	case NORTH: return cell - LABY_MAX; break;
	case SOUTH: return cell + LABY_MAX; break;
	case WEST: return cell - 1; break;
	case EAST: return cell + 1; break;
	default: exit(-1);
	}
}

//���ݱ�����out,���Ӧ����Ľڵ��ƶ�
Cell* advance(Cell* cell) {
	Cell* next;
	switch (cell->out)
	{
	case NORTH: next = cell - LABY_MAX; next->in = SOUTH; break;
	case SOUTH: next = cell + LABY_MAX; next->in = NORTH; break;
	case WEST: next = cell - 1; next->in = EAST; break;
	case EAST: next = cell + 1; next->in = WEST; break;
	default: exit(-1);
	}
	return next;
}

//�������д��myStack<Cell*> path�ͻ�����ڴ�Խ�������?
//�ᴴ��һ��copyԭ��������������,������������ͷſռ�û������,�����ͷ�ԭ����ʱ���� 
//û�ж�Vector�Զ��忽������,���½�һ��newpath=path,�������ÿ��Ԫ�ض�����ֵ��newpath.x=path.x,��������T* _elem��������������
void printStack(myStack<Cell*>& path,Cell * s,Cell * e) {
	cout <<"start:" << "(" << s->x << "," << s->y << ")" << endl;
	cout <<"end:" << "(" << e->x << "," << e->y << ")" << endl;
	while (!path.empty()) {
		Cell* c = path.pop();
		cout << "(" << c->x << "," << c->y << ")" << endl;
	}
	return;//����ΪmyStack<Cell*> path�������,�ں�������ǰ�����ͷ��˿�¡������ڴ�
}


//��laby�Թ��д�λ��s��ʼ��λ��e����
bool labyrinth(Cell laby[LABY_MAX][LABY_MAX], Cell* s, Cell* e) {
	if (s->status != AVAILABLE || e->status != AVAILABLE) {
		return false;
	}
	myStack<Cell*> path;
	s->in = UNKNOWN; s->status = ROUTE; path.push(s);//������ʼ��
	do {
		Cell* curr = path.top();//currΪ��ǰ�������
		if (curr == e) {
			printStack(path,s,e);
			return true;
		}

		while ((curr->out = nextDir(curr->out)) < NO_WAY) {
			if (neighbor(curr)->status == AVAILABLE) break;//���״̬��WALL(ǽ��),BACKTRACKED(�����ݱ��Ϊ��·)����ǰ��
		}
		if (curr->out < NO_WAY) {//�ҵ����н��ķ���,ǰ��
			path.push(curr = advance(curr));
			curr->status = ROUTE; curr->out = UNKNOWN;
		}
		else {//δ�ҵ�����,˵������·,���ϱ�Ǻ����
			curr->status = BACKTRACKED;
			curr=path.pop();
		}
	} while (!path.empty());
}

static int labySize;  //�˴�����dascpp�е˹�������Թ����ɳ���
static Cell* startCell;
static Cell* goalCell;
static void randLaby()
{
	labySize = 12;  //����һ�����size���Թ�
	printf("Using a laby of size %d ...\n", labySize);
	for (int i = 0; i < labySize; i++)
		for (int j = 0; j < labySize; j++)
		{
			laby[i][j].x = i;
			laby[i][j].y = j;
			laby[i][j].in =
				laby[i][j].out = UNKNOWN;
			laby[i][j].status = WALL; //�߽��������ǽ
		}
	for (int i = 1; i < labySize - 1; i++)
		for (int j = 1; j < labySize - 1; j++)
			if (rand() % 4) laby[i][j].status = AVAILABLE; //75%�ĸ��Ϊ�տ��ã������Թ��Ѷ��ڴ������޸�
	startCell = &laby[rand() % (labySize - 2) + 1][rand() % (labySize - 2) + 1];
	goalCell = &laby[rand() % (labySize - 2) + 1][rand() % (labySize - 2) + 1];
	startCell->status = goalCell->status = AVAILABLE; //��ʼ���������
}

//int main() {
//	randLaby();
//	labyrinth(laby, startCell, goalCell);
//}

