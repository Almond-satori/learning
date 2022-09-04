#include "4-1-stack.hpp"

struct	Queen {
	int x, y;
	Queen(int ax = 0, int by = 0) : x(ax), y(by) {};
	bool operator== (Queen const& q) const {
		return q.x == x || q.y == y || q.x + q.y == x + y || q.x - q.y == x - y;//判断皇后是否有冲突
	}
	bool operator!= (Queen const& q) const {
		return !(*this == q);
	}
};

//尝试放置queen
int placeQueen(int N) {
	int nCheck = 0;
	int nSolu = 0;//解法个数
	myStack<Queen>solu;//保存每一行的放置的queen
	Queen q(0, 0);
	//按照行数进行遍历
	do {
		if (q.x >= N || q.y >= N) {
			q = solu.pop(); q.y++;//后退一行,在这一行中的下一列放置queen
		}
		else {//本轮不回退就试探本行的queen
			while (q.y < N && solu.find(q) >= 0 ) q.y++;//在本行对应合法列上放置queen,如果冲突(find >=0)就进入循环
			if (q.y < N) {//如果是因为不冲突而退出循环的情况
				solu.push(q); 
				if (solu.size() >= N) nSolu++;//全局解找到后,下一次循环会逐步后退
				q.x++; q.y = 0;//寻找下一行的queen
			}
		}
	} while (q.x > 0 || q.y < N);//当所有情况都遍历后结束循环(从0,0开始到找到一个解,向后回退并寻找,最后回退到第0行第N列时结束)
	return nSolu;
}

//int main() {
//	int x = placeQueen(11);
//	cout << x << endl;
//	return 0;
//}

