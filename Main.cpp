#include <iostream>
#include <string>
#include "Node.h"
using namespace std;
string tmp;
string trash;
int freeList;
bool first;
int tokenHashValue;
string garbageStr;
int garbagestack = 0;

#define NIL 0
#define HashSize 1031
#define memorylen 44



class Hash {
public:
	string symbol;
	int pointer;
};

class userStack {

public:
	userStack() : index(-2), pointer(-2) {}

	userStack(int i, int p) :index(i), pointer(p) {}



	int index, pointer;
};

bool stackIsEmpty(userStack stack) {
	if (stack.index == -2 && stack.pointer==-2)
		return true;
	else
		return false;


}
void InituserStackArr(userStack* arr, int num) {

	for (int i = 0; i < num; i++) {
		arr[i].index = -2;
		arr[i].pointer = -2;
	}


}

int getIndex(int x) {
	if (x < 0)
		return (-1) * x;
	else
		return x;
}


int checkSizestack(userStack* arr, int size) {
	int result = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i].index != -2 && arr[i].pointer != -2) {
			result++;
		}
		else
			break;

	}

	return result;
}

void userStackPush(userStack* arr, userStack newstack, int size) {

	arr[checkSizestack(arr, size)] = newstack;

}


userStack lastElement(userStack* arr, int size) {
	int count = 0;

	if(stackIsEmpty(arr[0]))
		return arr[0];

	for (int i = 0; i < size; i++) {
		if (stackIsEmpty(arr[i]) != true)
			count++;
		else
			break;

	}
	return arr[count-1];

}

int lastElementIndex(userStack* arr, int size) {
	int count = 0;

	if (stackIsEmpty(arr[0]))
		return 0;

	for (int i = 0; i < size; i++) {
		if (stackIsEmpty(arr[i]) != true)
			count++;
		else
			break;

	}
	return count - 1;


}
void userStackPop(userStack* arr, int size) {


	arr[lastElementIndex(arr,size)].index = -2;
	arr[lastElementIndex(arr, size)].pointer = -2;
}

Node* memoryarr = new Node[memorylen];
Hash* hashTable = new Hash[HashSize];
userStack* userFunc = new userStack[30];
userStack* variables = new userStack[50];





void InitHashTable(Hash* arr, int length) {
	for (int i = 0; i < length; i++) {
		arr[i].symbol = "";
		arr[i].pointer = 0;
	}

}

int getNext(int index, Node* arr) {
	if (index != 0 && index < 29)
		return index + 1;
	else
		return 0;

}








string eraseSpace(string str) {
	int count = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ' ')
			count++;
		else
			break;
	}

	return str.erase(0, count);
}

string tosmall(string str) {
	for (int i = 0; i < str.length(); i++)
		str[i] = tolower(str[i]);
	return str;
}
string eraseFirstSpace(string str) {

	
	string result = str;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] != ' ')
			break;
		else {
			result.erase(0, 1);
		}

	}

	return result;

}

string getNextToken(string str) {
	str = tosmall(str);
	str = eraseFirstSpace(str);
	
	string stmp = "";
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == ' ' && stmp != "")
			break;

		else if (str[i] == '(' || str[i] == ')' || str[i] == '`') {
			if (stmp == "") {
				stmp += str[i];
				break;
			}
			else
				break;
		}
		else if (str[i] == ' ' && stmp == "")
			continue;
		else
			stmp += str[i];
	}
	trash = str.substr(0, stmp.length());
	tmp = str.erase(0, stmp.length());
	tmp = eraseSpace(tmp);


	return stmp;
}




int getHashValue(string str) {

	if (str == "(") {
		return -1;
	}
	else if (str == ")")
		return -2;
	else if (str == "+")
		return -5;
	else if (str == "-")
		return -6;
	else if (str == "*")
		return -7;
	else if (str == "car")
		return -14;
	else if (str == "cdr")
		return -15;
	else if (str == "`")
		return -17;
	else if (str == "cons")
		return -11;
	else if (str == "define")
		return -16;
	else if (str == "lambda")
		return -18;
	else if (str == "number?")
		return -8;
	else if (str == "symbol?")
		return -9;
	else if (str == "null?")
		return -10;
	else if (str == "cond")
		return -12;
	else if (str == "#t")
		return -3;
	else if (str == "#f")
		return -4;
	else if (str == "else")
		return -13;
	
	
	
	int sum = 0;
	string tmp1 = str;

	for (int i = 0; i < str.length(); i++) {
		sum += (int)tmp1[0]*(int)tmp1[0];
		tmp1.erase(0, 1);
	}

	int value = sum % HashSize;
	
	for (int i = 0; i < HashSize; i++) {
		if (hashTable[i].symbol == str)
			return (-1) * i;
	}


	if (hashTable[value].symbol != "" && hashTable[value].symbol == str)
		return (-1) * value;
	while (hashTable[value].symbol != "") {
		value++;
	}

	

	hashTable[value].symbol = str;

	return (-1) * value;
}

void setFlag(Node* arr, int start) {
	int i = start;
	int count = 1;
	while (count){
		if (memoryarr[i].left > 0) {
			count++;
		}
		if (memoryarr[i].right == 0)
			count--;


		memoryarr[i].flag = 1;
		i++;
				
	}


	

	
}

int findRightAfterGarbage(int start) {
	for (int i = start+1; i < memorylen; i++) {
		if (memoryarr[i].left == 0)
			return i;

	}

	for (int i = 1; i < start; i++) {
		if (memoryarr[i].left == 0)
			return i;

	}


}

void secretGC(Node* arr, int start) {
	for (int i = 1; i < HashSize - 1; i++) {
		if (hashTable[i].pointer > 0) {
			setFlag(memoryarr, hashTable[i].pointer);
		}
	}

	//for (int i = 0; i < checkSizestack(variables, 50); i++) {
	//	if (variables[i].pointer > 0) {
	//		setFlag(memoryarr, variables[i].pointer);
	//	}

	//}


	for (int i = 1; i < memorylen - 1; i++) {
		if (memoryarr[i].flag == 0) {
			memoryarr[i].left = 0;
		}

	}
	for (int i = 1; i < memorylen - 1; i++) {
		if (memoryarr[i].flag == 0) {
			memoryarr[i].right = findRightAfterGarbage(i);
		}

	}



	memoryarr[memorylen - 1].right = 0;


}



void garbageCollection(Node* arr,int start) {
	cout << "======Garbage Collection Called======" << endl;
	
	for (int i = 1; i < HashSize-1; i++) {
		if (hashTable[i].pointer > 0) {
			setFlag(memoryarr, hashTable[i].pointer);
		}
	}

	for (int i = 0; i < checkSizestack(variables, 50); i++) {
		if (variables[i].pointer > 0) {
			setFlag(memoryarr, variables[i].pointer);
		}

	}


	for (int i = 1; i < memorylen-1; i++) {
		if (memoryarr[i].flag == 0) {
			memoryarr[i].left = 0;
		}

	}
	for (int i = 1; i < memorylen-1; i++) {
		if (memoryarr[i].flag == 0) {
			memoryarr[i].right = findRightAfterGarbage(i);
		}

	}



	memoryarr[memorylen-1].right = 0;
	garbagestack++;
	
}


int alloc() {
	int count = 0;
	int tmp = 0;

	for (int i = 1; i < memorylen-1; i++) {
		if (NodeIsEmpty(memoryarr[i]) && memoryarr[i].right > 0) {
			if (count == 0) {
				memoryarr[i].right = -5;
				count++;
				tmp = i;
			}
			else {
				freeList = i;
				return tmp;
			}
		}

	}

	garbageCollection(memoryarr, 1);

	for (int i = 1; i < memorylen-1; i++) {
		if (NodeIsEmpty(memoryarr[i]) && memoryarr[i].right > 0) {
			if (count == 0) {
				memoryarr[i].right = -5;
				count++;
				tmp = i;
			}
			else {
				freeList = i;
				return tmp;
			}
		}

	}

	return 100;

}


void pushBack() {
	tmp = trash + tmp;
}


string concatenate(string str, string newstr) {
	string result = "";

	result += str;
	result += " ";
	result += newstr;

	return result;
}

string processing(string str) {

	string newcommand = "";
	string token = "";
	tmp = str;
	while ((token = getNextToken(tmp)) != "") {
		if (token == "define") {
			newcommand = concatenate(newcommand, token);
			token = getNextToken(tmp);
			if (token == "(") {
				token = getNextToken(tmp);
				newcommand = concatenate(newcommand, token);
				newcommand = concatenate(newcommand, "(lambda (");
				newcommand = concatenate(newcommand, processing(tmp));
				newcommand = concatenate(newcommand, ")");
				return newcommand;
			}
			else {
				newcommand = concatenate(newcommand, token);
			}
		}
		else if (token == "`") {
				newcommand += "(quote";
				int lpnum = 0;
				do {
					token = getNextToken(tmp);
					newcommand = concatenate(newcommand, token);
					if (token == "(") {
						lpnum += 1;

					}
					else if (token == ")") {
						lpnum -= 1;
					}

				} while (lpnum > 0);
				newcommand = concatenate(newcommand, ")");

			}
		else {
			newcommand = concatenate(newcommand, token);
		}	
	}

	return newcommand;
}









void printLine(int index, bool startList) {
	if (index == NIL) {
		cout << "()" << endl;
	}

	//else if (hashTable[index].pointer < -2) {
	//	cout<<eval()
	//}



	else if (index < 0) {
		//cout << "여기임" << hashTable[getIndex(index)].symbol<< endl;
		cout << hashTable[getIndex(index)].symbol << " ";
	}
	else if (index > 0) {
		if (startList == true) {
			cout << "(";
		}
		printLine(memoryarr[index].left, true);
		if (memoryarr[index].right != NIL) {
			printLine(memoryarr[index].right, false);
		}
		else {
			cout << ")";
		}
	}

}

void printRoot(int x) {
	cout << "Free List's Root : " << freeList << endl;
	cout << "List's Root :" << x << endl;

}


void printMemoryarr() {

	cout << "========Tree Array========" << endl;
	cout << "index		Left		Right		flag" << endl;

	for (int i = 1; i < memorylen; i++) {
		cout << i << "		" << memoryarr[i].left << "		" << memoryarr[i].right << "		"<<memoryarr[i].flag<<endl;

	}



}

void printHashTable() {
	cout << "========Hash Table========" << endl;
	cout << "HashValue		 Symbol		 Pointer" << endl;
	for (int i = 0; i < HashSize; i++) {
		if (hashTable[i].symbol == "")
			continue;
		else {
			cout << (-1)*i << "			" << hashTable[i].symbol << "			" << hashTable[i].pointer<<endl;
		}
	}

}


int quoteNext(int root) {
	int tmp = root;
	
	if (memoryarr[memoryarr[tmp].right].left < 0)
		return tmp + 2;
	else {
		while (memoryarr[memoryarr[memoryarr[tmp].right].left].right != 0) {
			tmp = memoryarr[memoryarr[memoryarr[tmp].right].left].right;

		}
		return tmp + 2;


	}




}


bool isNumStr(string str) {
	if (str == "")
		return false;

	if (str[0] == '-') {
		for (int i = 1; i < str.length(); i++) {

			if (str[i] >= '0' && str[i] <= '9') {
				continue;
			}
			else if (str[i] == '.' && i != 0 && i != str.length() - 1)
				continue;
			else
				return false;
		}
		return true;
	}
	else {
		for (int i = 0; i < str.length(); i++) {

			if (str[i] >= '0' && str[i] <= '9') {
				continue;
			}
			else if (str[i] == '.' && i != 0 && i != str.length() - 1)
				continue;
			else
				return false;
		}
		return true;
	}


}

bool isNum(int x) {
	string str = hashTable[getIndex(x)].symbol;

	if (str == "-")
		return false;
	
	if (str == "")
		return false;
	if (str[0] == '-') {
		for (int i = 1; i < str.length(); i++) {

			if (str[i] >= '0' && str[i] <= '9') {
				continue;
			}
			else if (str[i] == '.' && i != 0 && i != str.length() - 1)
				continue;
			else
				return false;
		}
		return true;
	}
	else {
		for (int i = 0; i < str.length(); i++) {

			if (str[i] >= '0' && str[i] <= '9') {
				continue;
			}
			else if (str[i] == '.' && i != 0 && i != str.length() - 1)
				continue;
			else
				return false;
		}
		return true;
	}


	/*for (int i = 0; i < str.length(); i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			continue;
		}
		else
			return false;
	}
	return true;*/
}

string checkNum(string str) {
	if (isNumStr(str) == true) {
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '.')
				return "double";

		}
		return "int";



	}

	return "not";

}


bool IsuserFunc(int index, userStack* arr) {

	int i = 0;
	while (!stackIsEmpty(arr[i])) {
		if (index == arr[i].index)
			return true;
		i++;
	}
	return false;
}

int thereisStack(userStack* arr,int index) {

	for (int i = 0; i < checkSizestack(arr, 50); i++) {
		if (getIndex(arr[i].index) == getIndex(index) && hashTable[getIndex(index)].pointer >0) {
			//cout << getIndex(arr[i].index) << "가  같아서 " << hashTable[getIndex(index)].symbol << endl;
			//cout << i << "번쨰 stack" << endl;
			return i;
		}


	}

	return -1;
}




int getFirst(int root);
int getSecond(int root);

int eval(int root) {

	if (isNum(root) == true)
		return root;
	

	if (root < 0) {
		/*if (hashTable[getIndex(root)].pointer ==0) {
			return getIndex(root);
		}*/
		return hashTable[getIndex(root)].pointer;
	}

	int tokenIndex = getIndex(memoryarr[root].left);

	if (tokenIndex == 5) {
		int a1 = 0; int b1 = 0; double a2 = 0; double b2 = 0;


		if (checkNum(hashTable[getIndex(getFirst(root))].symbol) == "double" || checkNum(hashTable[getIndex(getSecond(root))].symbol) == "double") {
			a2 = stod(hashTable[getIndex(getFirst(root))].symbol);
			b2 = stod(hashTable[getIndex(getSecond(root))].symbol);
			return getHashValue(to_string(a2 + b2));
		}

		else {
			a1 = stoi(hashTable[getIndex(getFirst(root))].symbol);
			b1 = stoi(hashTable[getIndex(getSecond(root))].symbol);
			return getHashValue(to_string(a1 + b1));
		}

	}
	else if (tokenIndex == 6) {
		int a1 = 0; int b1 = 0; double a2 = 0; double b2 = 0;


		if (checkNum(hashTable[getIndex(getFirst(root))].symbol) == "double" || checkNum(hashTable[getIndex(getSecond(root))].symbol) == "double") {
			a2 = stod(hashTable[getIndex(getFirst(root))].symbol);
			b2 = stod(hashTable[getIndex(getSecond(root))].symbol);
			return getHashValue(to_string(a2 - b2));
		}

		else {
			a1 = stoi(hashTable[getIndex(getFirst(root))].symbol);
			b1 = stoi(hashTable[getIndex(getSecond(root))].symbol);
			return getHashValue(to_string(a1 - b1));
		}

	}

	else if (tokenIndex == 7) {
		int a1 = 0; int b1 = 0; double a2 = 0; double b2 = 0;


		if (checkNum(hashTable[getIndex(getFirst(root))].symbol) == "double" || checkNum(hashTable[getIndex(getSecond(root))].symbol) == "double") {
			a2 = stod(hashTable[getIndex(getFirst(root))].symbol);
			b2 = stod(hashTable[getIndex(getSecond(root))].symbol);
			return getHashValue(to_string(a2 * b2));
		}

		else {
			a1 = stoi(hashTable[getIndex(getFirst(root))].symbol);
			b1 = stoi(hashTable[getIndex(getSecond(root))].symbol);
			return getHashValue(to_string(a1 * b1));
		}

	}


	else if (tokenIndex == 14) {
		
		return memoryarr[getFirst(root)].left;
	}

	else if (tokenIndex == 15) {
		return memoryarr[getFirst(root)].right;
	}

	else if (tokenIndex == 17) {
		return memoryarr[memoryarr[root].right].left;
	}

	else if (tokenIndex == 11) {
		int newmemory = alloc();
		memoryarr[newmemory].left = eval(memoryarr[memoryarr[root].right].left);
		memoryarr[newmemory].right = eval(memoryarr[memoryarr[memoryarr[root].right].right].left);

		return newmemory;
	}
	else if (tokenIndex == 16) {

		if (getIndex(memoryarr[memoryarr[memoryarr[memoryarr[root].right].right].left].left) == 18) {
			hashTable[getIndex(memoryarr[memoryarr[root].right].left)].pointer = memoryarr[memoryarr[memoryarr[root].right].right].left;
			userStack a(getIndex(memoryarr[memoryarr[root].right].left), hashTable[getIndex(memoryarr[memoryarr[root].right].left)].pointer);
			userStackPush(userFunc, a,30);
			cout << hashTable[getIndex(memoryarr[memoryarr[root].right].left)].symbol << endl;
		
		}

		else {
			hashTable[getIndex(memoryarr[memoryarr[root].right].left)].pointer = eval(memoryarr[memoryarr[memoryarr[root].right].right].left);
		}


	}

	else if (tokenIndex == 8) {
		if (isNum(eval(memoryarr[memoryarr[root].right].left)) == true) {

				//cout << "TRUE" << endl;//
				return -3;

			}
			else {
				//cout << "FALSE" << endl;//
				return -4;

			}
	

	}
	else if (tokenIndex == 9) {
		int result = eval(memoryarr[memoryarr[root].right].left);
		//cout << "result값: " << result << endl;
		if (isNum(result)==false && hashTable[getIndex(memoryarr[memoryarr[root].right].left)].pointer != 0){
			//cout << "TRUE" << endl;//
			return -3;

		}
		else {
			//cout << "FALSE" << endl;//
			return -4;
			
		}
		}

	else if (tokenIndex == 10) {
		if (memoryarr[root].right == 0 || eval(memoryarr[memoryarr[root].right].left) == 0) {
			//cout << "T" << endl;
			return -3;
		}
		else {

			//cout << "F" << endl;
			return -4;

		}	

	}


	else if (tokenIndex == 12) {
		while (memoryarr[memoryarr[root].right].right != 0) {
			root = memoryarr[root].right;

			if (eval(memoryarr[memoryarr[root].left].left) == -3) {
				
			
				return eval(memoryarr[memoryarr[memoryarr[root].left].right].left);
				//return eval(memoryarr[memoryarr[root].left].right);

			}
			
		}

		if (eval(memoryarr[memoryarr[memoryarr[root].right].left].left)==-4) {

			if (memoryarr[memoryarr[memoryarr[root].right].left].left != -13) {
				cout << "ERROR!!!!" << endl;
					return 20000;
			}
		}
		//cout << "2번 경우" << endl;
		return eval(memoryarr[memoryarr[memoryarr[memoryarr[root].right].left].right].left);



}





	
	else if (IsuserFunc(tokenIndex, userFunc) == true) {  
		
		//cout << "called" << endl;

		int newroot = hashTable[tokenIndex].pointer;

		//x의 hashvalue(+)와 pointer를 stack 하나에 저장

		int varstart = memoryarr[memoryarr[newroot].right].left;
		int varend = memoryarr[memoryarr[newroot].right].right;


		for (int i = varstart; i < varend; i++) {
			//cout << hashTable[getIndex(memoryarr[i].left)].symbol<<"이 "<< hashTable[getIndex(hashTable[getIndex(memoryarr[i].left)].pointer)].symbol << " 라는 것을 stack에 저장" << endl;
			userStack a(getIndex(memoryarr[i].left), hashTable[getIndex(memoryarr[i].left)].pointer);
			userStackPush(variables, a, 50);


		}



		/*userStack a(getIndex(memoryarr[memoryarr[memoryarr[newroot].right].left].left), hashTable[getIndex(memoryarr[memoryarr[memoryarr[hashTable[tokenIndex].pointer].right].left].left)].pointer);

		userStackPush(variables, a, 50);*/

		int inputVarStart = getIndex(memoryarr[root].right);
		int inputtmp = inputVarStart;
		int assign = varstart;

		int x = varend - varstart;

		int* fixarr = new int[x];
		
		for (int i = 0; i < x; i++) {	
				
			if (isNumStr(hashTable[getIndex(memoryarr[inputtmp].left)].symbol) == true) {
				fixarr[i]= memoryarr[inputtmp].left;
				//cout << "1번 경우" << endl;
				//cout << "fixarr" << i << "번째 " << memoryarr[inputtmp].left << " 저장" << endl;
			}
			else {

				if (getIndex(hashTable[getIndex(memoryarr[inputtmp].left)].pointer) > 0) {
					fixarr[i] = hashTable[getIndex(memoryarr[inputtmp].left)].pointer;
					//cout << "2번 경우" << endl;
					//cout << "fixarr" << i << "번째 " << hashTable[getIndex(memoryarr[inputtmp].left)].pointer << " 저장" << endl;
				}
				else {
					//cout << "3번 경우" << endl;
					
					
					fixarr[i] = eval(getIndex(memoryarr[inputtmp].left));
					//cout << "fixarr" << i << "번째 " << eval(getIndex(memoryarr[inputtmp].left)) << " 저장" << endl;
					
					
				}
			}
			inputtmp = memoryarr[inputtmp].right;
			assign++;

		}


		inputVarStart = getIndex(memoryarr[root].right);
		inputtmp = inputVarStart;
		assign = varstart;



		for (int i = 0; i < varend - varstart; i++) {

			hashTable[getIndex(memoryarr[assign].left)].pointer = fixarr[i];


			assign++;
		

		}

		delete[] fixarr;



		/*	if (isNumStr(hashTable[getIndex(memoryarr[memoryarr[root].right].left)].symbol) == true)
				hashTable[getIndex(memoryarr[memoryarr[memoryarr[newroot].right].left].left)].pointer = ge	tIndex(memoryarr[memoryarr[root].right].left);
			else
				hashTable[getIndex(memoryarr[memoryarr[memoryarr[newroot].right].left].left)].pointer = hashTable[getIndex(memoryarr[memoryarr[root].right].left)].pointer;
	*/

	//int result = eval(memoryarr[memoryarr[memoryarr[hashTable[tokenIndex].pointer].right].left].left);
		int result = eval(memoryarr[memoryarr[memoryarr[newroot].right].right].left);


		int deassign = varstart;

		for (int i = varend - 1; i >= deassign; i--) {
			//cout << hashTable[getIndex(memoryarr[i].left)].symbol<<"이 "<< hashTable[getIndex(lastElement(variables, 50).pointer)].symbol << "라는 것을 stack으로부터 pop" << endl;
			hashTable[getIndex(memoryarr[i].left)].pointer = lastElement(variables, 50).pointer;
			userStackPop(variables, 50);
		}

		return result;


	}
	

	return 14000;


}
int innerstack = 0;

int ReadString(string str) {
	innerstack++;
	string garbageStr = str;
	int garbage = garbagestack;
	int garbagex;
	int temp = 0;
	int root = 0;
	first = true;
	tokenHashValue = getHashValue(getNextToken(str));

	if (tokenHashValue == -1) {
		while ((tokenHashValue = getHashValue(getNextToken(tmp))) != -2) {
			if (first == true) {
				int freetemp = freeList;
				temp = alloc();
				if (garbagestack - garbage != 0) {
					freeList = freetemp;
					break;
				}

				root = temp;
				first = false;
			}
			else {
				int freetemp = freeList;
				int gbtmp = alloc();
				if (garbagestack - garbage != 0) {
					freeList = freetemp;
					break;

				}
				memoryarr[temp].right = gbtmp;
				temp = memoryarr[temp].right;
			}

			if (tokenHashValue == -1) {
				pushBack();
				int ttmp = ReadString(tmp);
				 
				if (garbagestack - garbage != 0) {
					//cout << "이 경우" << endl;

					//secretGC(memoryarr, 1);
					break;
				}
				memoryarr[temp].left = ttmp;
			}
			else {
				memoryarr[temp].left = tokenHashValue;
			}
		}
		if (garbagestack - garbage != 0) {
			if (innerstack > 1) {
				innerstack--;
				return 0;
			}
			else {
				//cout << "요오것" << endl;
				//cout << garbageStr << endl;
				garbagex = ReadString(garbageStr);
				innerstack--;
				return garbagex;
			}
		}
		else {
			memoryarr[temp].right = NIL;
			innerstack--;
			return root;
		}
	}
	else { 
		//	cout << "token: " << tokenHashValue << endl;
		
		if (isNum(getIndex(hashTable[getIndex(tokenHashValue)].pointer)) == true) {
			//cout << "1번 경우" << hashTable[getIndex(tokenHashValue)].symbol << endl;
			cout << hashTable[getIndex(hashTable[getIndex(tokenHashValue)].pointer)].symbol << endl;
		}
		else {
			//cout << "2번 경우" << hashTable[getIndex(tokenHashValue)].symbol << endl;
			//cout << eval(hashTable[getIndex(tokenHashValue)].pointer) << endl;
		}
		innerstack--;
		return tokenHashValue;

	}

}

int getFirst(int root) {
	return eval(memoryarr[memoryarr[root].right].left);
}

int getSecond(int root) {
	return eval(memoryarr[memoryarr[memoryarr[root].right].right].left);
}

void printResult(int result) {


	cout << "Evaluation Result : " <<hashTable[getIndex(result)].symbol << endl;

}



int main() {

	InitNodeArray(memoryarr, memorylen);
	freeList = 1;
	InitHashTable(hashTable, HashSize);
	InituserStackArr(userFunc, 30);
	InituserStackArr(variables, 50);
	
	hashTable[1].symbol = "(";
	hashTable[2].symbol = ")";
	hashTable[5].symbol = "+";
	hashTable[6].symbol = "-";
	hashTable[7].symbol = "*";
	hashTable[14].symbol = "car";
	hashTable[15].symbol = "cdr";
	hashTable[17].symbol = "quote";
	hashTable[11].symbol = "cons";
	hashTable[16].symbol = "define";
	hashTable[18].symbol = "lambda";
	hashTable[8].symbol = "number?";
	hashTable[9].symbol = "symbol?";
	hashTable[10].symbol = "null?";
	hashTable[12].symbol = "cond";
	hashTable[3].symbol = "#t";
	hashTable[4].symbol = "#f";
	hashTable[13].symbol = "else";




	int count;
	count = 0;

	

	while (true) {

		int no = 0;
		string str;
		cout << ">";
		getline(cin, str);

		string newstr;
		

		newstr = processing(str);
		

		newstr = eraseFirstSpace(newstr);

		//cout << newstr << endl;
		int root = ReadString(newstr);
		
		cout << "]";

		int result = eval(root);

		if (newstr[0] != '(' && isNum(result)==true) {
			no = 1;
		}
		
		//cout << "result 값 : "<<result << endl;

		if (newstr[0] != '(' && result > 50) {/////
			result = (-1) * result;

		}
		
		if (result != 14000 && no == 0) {
			//cout << "연산 최종 결과는??? ::: " << endl;
			printLine(result, true);
			cout << endl;
		}
		printRoot(root);
		//printLine(root, true);

		


		printMemoryarr();
		printHashTable();

		if (result == 20000) {
			cout << ":::::ERROR ::::: PROBLEM WITH COND" << endl;
			break;

		}



		
		cout << endl << endl;

	}
	

	cout << "Error로 인해 프로그램이 종료되었습니다." << endl;
	
}