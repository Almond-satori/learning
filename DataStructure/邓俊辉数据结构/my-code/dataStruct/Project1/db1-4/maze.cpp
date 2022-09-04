#include "4-1-stack.hpp"

typedef enum {AVAILABLE,ROUTE,BACKTRACKED,WALL} Status;

typedef enum {UNKNOWN,EAST,SOUTH,WEST,NORTH,NO_WAY} Direction;

Direction nextDir(Direction dir) { return Direction(dir + 1); }//枚举类有编号,+1可以指向下一个属性

struct Cell { //每个位置的结点
	int x, y; Status status;//位置坐标,位置状态
	Direction in, out;//探索路径时,从方向in走入本结点,从本结点走向out方向
};

#define LABY_MAX 24 //迷宫尺寸
Cell laby[LABY_MAX][LABY_MAX]; //迷宫

//根据本结点的out,获取对应方向上的邻居结点并返回
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

//根据本结点的out,向对应方向的节点移动
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

//如果参数写成myStack<Cell*> path就会出现内存越界的问题?
//会创建一个copy原来向量的新向量,对这个新向量释放空间没有问题,但在释放原向量时出错 
//没有对Vector自定义拷贝函数,会新建一个newpath=path,这里面的每个元素都被赋值即newpath.x=path.x,包括数组T* _elem这样的引用类型
void printStack(myStack<Cell*>& path,Cell * s,Cell * e) {
	cout <<"start:" << "(" << s->x << "," << s->y << ")" << endl;
	cout <<"end:" << "(" << e->x << "," << e->y << ")" << endl;
	while (!path.empty()) {
		Cell* c = path.pop();
		cout << "(" << c->x << "," << c->y << ")" << endl;
	}
	return;//参数为myStack<Cell*> path的情况下,在函数返回前正常释放了克隆对象的内存
}


//在laby迷宫中从位置s开始到位置e结束
bool labyrinth(Cell laby[LABY_MAX][LABY_MAX], Cell* s, Cell* e) {
	if (s->status != AVAILABLE || e->status != AVAILABLE) {
		return false;
	}
	myStack<Cell*> path;
	s->in = UNKNOWN; s->status = ROUTE; path.push(s);//对起点初始化
	do {
		Cell* curr = path.top();//curr为当前操作结点
		if (curr == e) {
			printStack(path,s,e);
			return true;
		}

		while ((curr->out = nextDir(curr->out)) < NO_WAY) {
			if (neighbor(curr)->status == AVAILABLE) break;//如果状态是WALL(墙壁),BACKTRACKED(被回溯标记为死路)不能前进
		}
		if (curr->out < NO_WAY) {//找到可行进的方向,前进
			path.push(curr = advance(curr));
			curr->status = ROUTE; curr->out = UNKNOWN;
		}
		else {//未找到方向,说明是死路,左上标记后回退
			curr->status = BACKTRACKED;
			curr=path.pop();
		}
	} while (!path.empty());
}

static int labySize;  //此处借用dascpp中邓公的随机迷宫生成程序
static Cell* startCell;
static Cell* goalCell;
static void randLaby()
{
	labySize = 12;  //生成一个随机size的迷宫
	printf("Using a laby of size %d ...\n", labySize);
	for (int i = 0; i < labySize; i++)
		for (int j = 0; j < labySize; j++)
		{
			laby[i][j].x = i;
			laby[i][j].y = j;
			laby[i][j].in =
				laby[i][j].out = UNKNOWN;
			laby[i][j].status = WALL; //边界格点必须是墙
		}
	for (int i = 1; i < labySize - 1; i++)
		for (int j = 1; j < labySize - 1; j++)
			if (rand() % 4) laby[i][j].status = AVAILABLE; //75%的格点为空可用，增加迷宫难度在此酌情修改
	startCell = &laby[rand() % (labySize - 2) + 1][rand() % (labySize - 2) + 1];
	goalCell = &laby[rand() % (labySize - 2) + 1][rand() % (labySize - 2) + 1];
	startCell->status = goalCell->status = AVAILABLE; //起始格点必须可用
}

//int main() {
//	randLaby();
//	labyrinth(laby, startCell, goalCell);
//}

