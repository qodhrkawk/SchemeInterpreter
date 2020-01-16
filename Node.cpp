#include <string>
#include "Node.h"
using namespace std;

void InitNodeArray(Node* arr, int arrayLen) {
	for (int i = 1; i < arrayLen; i++) {
		arr[i].left = 0;
		arr[i].right = i + 1;
		arr[i].flag = 0;
	}
	arr[0].left = 0;
	arr[0].right = 0;
	arr[0].flag = 1;
	arr[arrayLen - 1].right = 0;
}

bool NodeIsEmpty(Node node) {
	if (node.left == 0)
		return true;
	return false;
}



