#include "BTree.hpp"

int main() {
	BTree<int> tree = BTree<int>();
	tree.insert(10);
	tree.insert(5);
	tree.insert(4);
	tree.insert(3);
	tree.insert(2);
	tree.insert(1);
	tree.insert(20);
	tree.insert(25);
	tree.insert(30);
	tree.insert(40);
	tree.remove(40);
	tree.remove(30);
	tree.remove(25);
	tree.remove(20);
	tree.remove(1);
	tree.remove(2);
}