#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <list>
#include <set>
#include <malloc.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include "Timer.h"
#include "Timer.cpp"

//#define N 4
#define NUMSUFF 1000
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

using namespace std;

//typedef unsigned char datatype;

struct Node{
	char **cell;
	int f;
	int g;
	Node *parent;
	int action;
};

char fi[] = "npuzzle.txt";
//char fn[1000];
//char tmp[1000];
int N;							// Width of Puzzle

//char *existArr;
int numOfNode = 1;				// No. of node used to search
//int sizeAlloc = 0;
list<Node*> listNode;			// Fringe
set<Node*> setNode;				// Store all used node. Used to free memory
Node *node;						// Start State
Node *goal;						// Store Goal State
int deep = 0;					// Solution 'S Deep
char choice;					// Heuristic Flag
int cutoff = 0;					// Use in IDA*
int newcutoff = 0;				// Use in IDA*
int countShuffle = 0;			// No. of step to shuffle Puzzle
int timeAStar = 30;				// If A* run too long, terminate it
int timeIDAStar = 60;			// If IDA* run too long, terminate it

int (*function[4])(void);		// Use in analyzing only

int readData(void);
int init(void);
int init1(void);
int mapIndex(int row, int column);  // Convert 2D coordinates to 1D coordinates
int treeSearch(void);			// A* Search
int treeSearch1(void);			// IDA* Search
//int calculateCost(Node *p);
int calculate(Node *p);
int heuristic1(Node *p); 		// Manhattan
int heuristic2(Node *p); 		// Manhattan + Linear Conflict
int heuristic3(Node *p); 		// Tiles out of row and column
int heuristic4(Node *p); 		// Pythagorean - not admissable
int heuristic5(Node *p);		// N-MaxSwap - Gaschnig's Heuristic
int check(Node *p);		 		// Check if p is goal state or not
int checkSolvable(Node *p);		// Check state if it is solvable or not
int result(Node *p);			// Print solution
int blank_x(Node *p);			// return x coordinates of blank tile
int blank_y(Node *p);			// return y coordinates of blank tile
//int key(Node *p);
//int exist(Node *p);
//int mark(Node *p);
int up(void);					// blank up.    For analyzing only
int down(void);					// blank down.  For analyzing only
int left(void);					// blank left.  For analyzing only
int right(void);				// blank right. For analyzing only
int print(void);				// print current state
int freeNode(Node *p);			// Free memory allocated for node p
int copy(Node *a, Node *b);		// copy data from b to a

Node* newNode(void);
Node* pick(void);				// Choose one node to expand from fringe. Use in A*
Node* pick1(void);				// Choose one node to expand from fringe. Use in IDA*


int main(void){
//	function[0] = &up;
//	function[2] = &down;
//	function[1] = &left;
//	function[3] = &right;
	
	readData();
//	puts("readData ok");
	if (!checkSolvable(node)){
		puts("Unsolvable.");
		return 0;
	};
//	puts("Press any key to continue.");
//	getch();
	init();
//	char ch;
	do{
		print();
		puts("1. Manhattan");
		puts("2. Linear Conflict");
		puts("3. Tiles out of row and column");
		puts("4. Pythagorean (not admissable)");
		puts("5. N-MaxSwap");
		choice = getch();
		system("cls");
	} while ((choice < '1') || (choice > '5'));
//	choice = '3';
//	do{
//		print();
//		ch = getch();
//		if (ch != 13){
//			countShuffle++;
//		}
//		switch (ch){
//			case '8':
//				up();
//				break;
//			case '4':
//				left();
//				break;
//			case '6':
//				right();
//				break;
//			case '2':
//				down();
//				break;
//			default:
//				break;
//		}
//	} while (ch != 13);
	puts("Searching...");
	Timer ti;
	treeSearch();
	double y = ti.getElapsedTime();
	system("cls");
	result(goal);
	printf("Heuristic %c.\nThoi gian: %.3f s, Do dai loi giai: %i.", choice, y, deep);
	set<Node*>::iterator it = setNode.begin();
	Node *nodeToFree = NULL;
	for(; it != setNode.end(); it++){
		if (*it == node)
			continue;
		nodeToFree = *it;
		freeNode(nodeToFree);
	}
	listNode.clear();
	setNode.clear();
	return 0;
}

Node* newNode(void){
	Node *p = new Node;
	p->cell = (char**) calloc(N, sizeof(char*));
	if (p->cell == NULL){
		return NULL;
	}
	for(int i = 0; i < N; i++){
		p->cell[i] = (char*) calloc(N, sizeof(char));
		if (p->cell[i] == NULL){
			for(int j = 0; j < i; j++){
				free(p->cell[j]);
			}
			free(p->cell);
			return NULL;
		}
	}
	return p;
}

int checkSolvable(Node *p){
	int *a = (int*) malloc(N * N * sizeof(int));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			a[mapIndex(i, j)] = p->cell[i][j];
		}
	}
	int inversions = 0;
	for(int i = 0; i < N * N - 1; i++){
		for(int j = i + 1; j < N * N; j++){
			if (!a[j]){
				continue;
			}
			if (a[i] > a[j]){
				inversions++;
			}
		}
	}
//	printf("inversions = %i\n", inversions);
	puts("");
//	printf("%i %i", N % 2, inversions % 2 == 0);
//	if (((N % 2) && (inversions % 2 == 0))  ||  ((N % 2 == 0) && (((N - blank_x(p)) % 2) == (inversions % 2 == 0)))){
	if (((N % 2) && (inversions % 2 == 0))  ||  ((N % 2 == 0) && ((blank_x(p) % 2) == (inversions % 2)))){
		return 1;
	}
	return 0;
}

int print(void){
	system("cls");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			printf("%3i", node->cell[i][j]);
		}
		puts("");
	}
//	printf("%i", countShuffle);
}

int readData(void){
	FILE *f = fopen(fi, "r");
	if (!f){
		exit(0);
	}
	fscanf(f, "%i", &N);
//	printf("N = %i\n", N);
	node = newNode();
//	puts("newNode ok");
	char **p = node->cell;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			fscanf(f, "%i", &p[i][j]);
		}
	}
	fclose(f);
	node->f = 0;
	node->g = 0;
//	print();
	return 0;
}

int up(void){
	int x = blank_x(node);
	int y = blank_y(node);
	if (x == 0){
		down();
		return 0;
	}
	swap(node->cell[x - 1][y], node->cell[x][y]);
	return 0;
}

int down(void){
	int x = blank_x(node);
	int y = blank_y(node);
	if (x == N - 1){
		up();
		return 0;
	}
	swap(node->cell[x][y], node->cell[x + 1][y]);
	return 0;
}

int left(void){
	int x = blank_x(node);
	int y = blank_y(node);
	if (y == 0){
		right();
		return 0;
	}
	swap(node->cell[x][y], node->cell[x][y - 1]);
	return 0;
}

int right(void){
	int x = blank_x(node);
	int y = blank_y(node);
	if (y == N - 1){
		left();
		return 0;
	}
	swap(node->cell[x][y], node->cell[x][y + 1]);
	return 0;
}

int swap(char &x, char &y){
	char buf = x;
	x = y;
	y = buf;
	return 0;
}

int init(void){
//	node = newNode();
//	char *m = &(node->cell[0][0]);
//	for(int i = 0; i < N * N; i++)
//		m[i] = i;
//	node->g = 0;
	node->parent = NULL;
	node->action = 0;
	calculate(node);
	newcutoff = cutoff = node->f;
	listNode.clear();
	listNode.push_back(node);
	setNode.clear();
	setNode.insert(node);
//	sizeAlloc = 0;
//	for(int i = N * N - 2; i >=0; i-- ){
//		sizeAlloc += (int) (i + 1) * pow(N * N, i + 1);
//	}
//	printf("size = %i\n", sizeAlloc);
	//puts("before alloc");
	//getch();
//	existArr = (char*) calloc(sizeAlloc + 1, sizeof(char));
	//puts("after alloc");
	//getch();
//	for(int i = 0; i < size + 2; i++)
//		if (existArr[i] != 0)
//			puts("det");
//	puts("cast inside init first:");
//	existArr[key(node)] = 1;
	//puts("cast inside init second:");
	//printf("key node = %i\n", key(node));
}

//int init1(void){
//	node = newNode();
//	srand(time(NULL));
//	char *m = &(node->cell[0][0]);
//	for(int i = 0; i < N * N; i++)
//		m[i] = i;
////	for(int i = 0; i < NUMSUFF; i++){
//////		system("cls");
//////		for(int j = 0; j < N; j++){
//////			for(int k = 0; k < N; k++)
//////				printf("%i ", node->cell[j][k]);
//////			puts("");
//////		}
////		(*function[rand() % 4])();
////		//Sleep(100);
////	}
//
//	FILE *f = fopen(fn, "r");
//	fscanf(f, "%i", &numShuffle);
//	int act;
//	int a[10000];
//	for(int i = 0; i < numShuffle; i++){
//		fscanf(f, "%i", a + i);
//	}
//	for(int i = 0; i < numShuffle; i++){
//		(*function[a[i]])();
//	}
//	fclose(f);
//	node->g = 0;
//	node->parent = NULL;
//	node->action = 0;
//	calculate(node);
//	newcutoff = cutoff = 0;
//	listNode.clear();
//	listNode.push_back(node);
//	setNode.clear();
//	setNode.insert(node);
//}

//int key(Node *p){
//	int value = 0;
//	char *x = (char*) &(p->cell[0][0]);
//	char m = *x;
//	int k = 0;
//	int l1 = N * N;
//	int l =  l1 - 1;
//	for(int i = 0; i < l; i++){
//		m = *(x + i);
//		k = m * (int) pow((l1), i);
//		//printf("m = %i k = %i\n", m, k);
//		value += k;
//	}
//	if (value < 0)
//		puts("hehe");
//	if (value > sizeAlloc)
//		puts("hihi");
//	return value;
//}

int calculate(Node *p){
	int sum;
	switch (choice){
		case '1':
			sum = heuristic1(p);
			break;
		case '2':
			sum = heuristic2(p);
			break;
		case '3':
			sum = heuristic3(p);
			break;
		case '4':
			sum = heuristic4(p);
			break;
		case '5':
			sum = heuristic5(p);
			break;
	}
	p->f = p->g + sum;
}

int heuristic1(Node *p){
	int cost;
	int sum = 0;
	int num;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			num = p->cell[i][j];
			if (num == 0){
				//printf("  ");
				continue;
			}
				
			cost = abs(num / N - i) + abs(num % N - j);
			//printf("%i ", cost);
			sum += cost;
		}
//		puts("");
	}
//	sum = 0;
	return sum;
}

int inRow(int position, int row){
	if (position / N == row){
		return 1;
	}
	return 0;
}

int inColumn(int position, int column){
	if (position % N == column){
		return 1;
	}
	return 0;
}

int heuristic2(Node *p){
	
	int sum = heuristic1(p);
	int num = 0;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N - 1; j++){
			int x1 = p->cell[i][j];
			if (!x1){
				continue;
			}
			if (!inRow(x1, i))
				continue;
			for(int k = j + 1; k < N; k++){
				int x2 = p->cell[i][k];
				if (!x2){
					continue;
				}
				if (!inRow(x2, i)){
					continue;
				}
				if (x1 > x2){
					num++;
				}
			}
		}
	}
	
	for(int j = 0; j < N; j++){
		for(int i = 0; i < N - 1; i++){
			int x1 = p->cell[i][j];
			if (!x1){
				continue;
			}
			if (!inColumn(x1, j)){
				continue;
			}
			for(int k = i + 1; k < N; k++){
				int x2 = p->cell[k][j];
				if (!x2){
					continue;
				}
				if (!inColumn(x2, j)){
					continue;
				}
				if (x1 > x2){
					num++;
				}
			}
		}
	}
	
	num *= 2;
	return sum += num;
}

int heuristic4(Node *p){
	int cost;
	int sum = 0;
	int num;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			num = p->cell[i][j];
			if (num == 0){
				//printf("  ");
				continue;
			}
				
			cost = (num / N - i) * (num / N - i) + (num % N - j) * (num % N - j);
			//printf("%i ", cost);
			sum += cost;
		}
//		puts("");
	}
	return sum;
}

int heuristic5(Node *p){
	int size = N * N;
//	int *p = (int*) calloc(size, sizeof(int));
//	int *b = (int*) calloc(size, sizeof(int));
	int P[size];
	int B[size];
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			P[mapIndex(i, j)] = p->cell[i][j];
		}
	}
	for(int i = 0; i < size; i++){
		B[P[i]] = i;
	}
	int count = 0;
//	for(int i = 0; i < size; i++){
//		printf("%2i ", i);
//	}
//	printf(" | ");
//	for(int i = 0; i < size; i++){
//		printf("%2i ", i);
//	}
//	puts("");
	while (1){
		int check = 1;
		for(int i = 0; i < size; i++){
			if (P[i] != i){
				check = 0;
				break;
			}
		}
		if (check){
			break;
		}
		if (B[0]){
			swap(P[B[0]], P[B[B[0]]]);
			swap(B[0], B[B[0]]);
		}
		else{
			for(int i = 0; i < size; i++){
				if (!P[i]){
					continue;
				}
				if (P[i] == i){
					continue;
				}
				swap(P[i], P[B[0]]);
				swap(B[P[i]], B[0]);
				break;
			}
		}
//		for(int i = 0; i < size; i++){
//			B[P[i]] = i;
//		}
//		for(int i = 0; i < size; i++){
//			printf("%2i ", P[i]);
//		}
//		printf(" | ");
//		for(int i = 0; i < size; i++){
//			printf("%2i ", B[i]);
//		}
//		puts("");
//		getch();
		count++;
	}
//	free(p);
//	free(b);
	return count;
}

int heuristic3(Node *p){
	int sum = 0;
	int num;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			num = p->cell[i][j];
			//printf("%i ", num);
			if (num / N != i)
				sum++;
			if (num % N != j)
				sum++;
		}
	}
	//puts("hehe");
	return sum;
}

//int heuristic5(Node *p){ // Custom heuristic
//	int cost;
//	int sum = 0;
//	int num;
//	
//	for(int i = 0; i < N; i++){
//		for(int j = 0; j < N; j++){
//			num = p->cell[i][j];
//			if (num == 0){
//				continue;
//			}
//			int line = 0;
//			int x = abs(num / N - i);
//			int y = abs(num % N - j);
//			if (!x || !y)
//				line = 1;
//			cost = x + y;
//			if (!cost)
//				continue;
//			if (line)
//				sum += 1 + (cost - 1) * 5;
//			else{
//				cost = (int) sqrt(cost);
//				sum += 1 + (cost - 1) * 3;
//			}
////				sum += 1 + (cost - 1) * 3;
//		}
//	}
//	return sum - 1;
//}

int mapIndex(int row, int column){
	return row * N + column;
}

Node* pick(void){
	list<Node*>::iterator it = listNode.begin();
	Node *p = *it;
	for(; it != listNode.end(); it++){
		if (p->f > (*it)->f)
			p = *it;
	}
	listNode.remove(p);
	return p;
}

Node* pick1(void){
	list<Node*>::iterator it = listNode.begin();
	Node *p = *it;
	listNode.remove(p);
	return p;
}

int treeSearch(void){
	Timer ti;
	Node *p;
	Node *p1;
	int x;
	int y;
//	int keyp1;
	while (!listNode.empty()){
		y = ti.getElapsedTime();
		if (y > 30){
			deep = -1;
			return 0;
		}
		p = pick();
		if (check(p)){
//			result(p);
			goal = p;
			deep = p->g;
			return 0;
		}
		x = blank_x(p);
		y = blank_y(p);
		// blank up
		if ((x > 0) && (p->action != DOWN)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x - 1][y] = 0;
			p1->cell[x][y] = p->cell[x-1][y];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = UP;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		
		// blank down
		if ((x < N - 1) && (p->action != UP)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x + 1][y] = 0;
			p1->cell[x][y] = p->cell[x + 1][y];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = DOWN;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		
		// blank left
		if ((y > 0) && (p->action != RIGHT)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x][y - 1] = 0;
			p1->cell[x][y] = p->cell[x][y - 1];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = LEFT;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		
		// blank right
		if ((y < N - 1) && (p->action != LEFT)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x][y + 1] = 0;
			p1->cell[x][y] = p->cell[x][y + 1];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = RIGHT;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		//system("cls");
//		printf("%i:%i blank [%i] [%i]\n", numOfNode, listNode.size(), x, y);
	}
	return 0;
}

int treeSearch1(void){
	Timer ti;
	Node *p;
	Node *p1;
	int x;
	int y;
	int c = 0;
	cutoff = node->f;
//	int keyp1;
	while (1){
		if (listNode.empty()){
			set<Node*>::iterator it = setNode.begin();
			//puts("before clean");
			//getch();
			Node *nodeToFree = NULL;
			for(; it != setNode.end(); it++){
				if (*it == node)
					continue;
				nodeToFree = *it;
				freeNode(nodeToFree);
			}
			//puts("after clean");
			//getch();
			listNode.clear();
			listNode.push_back(node);
			setNode.clear();
			setNode.insert(node);
			cutoff = newcutoff;
			c = 0;
		}
		while (!listNode.empty()){
			y = ti.getElapsedTime();
			if (y > timeIDAStar){
				deep = -1;
				return 0;
			}
			p = pick1();
			if (check(p)){
//				result(p);
				goal = p;
				deep = p->g;
				return 0;
			}
			x = blank_x(p);
			y = blank_y(p);
			
			// blank up
			if ((x > 0) && (p->action != DOWN)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
//				puts("before");
				copy(p1, p);
//				puts("after");
				p1->cell[x - 1][y] = 0;
				p1->cell[x][y] = p->cell[x-1][y];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = UP;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			
			// blank down
			if ((x < N - 1) && (p->action != UP)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
				copy(p1, p);
				p1->cell[x + 1][y] = 0;
				p1->cell[x][y] = p->cell[x + 1][y];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = DOWN;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			
			// blank left
			if ((y > 0) && (p->action != RIGHT)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
				copy(p1, p);
				p1->cell[x][y - 1] = 0;
				p1->cell[x][y] = p->cell[x][y - 1];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = LEFT;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			
			// blank right
			if ((y < N - 1) && (p->action != LEFT)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
				copy(p1, p);
				p1->cell[x][y + 1] = 0;
				p1->cell[x][y] = p->cell[x][y + 1];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = RIGHT;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			//system("cls");
	//		printf("%i:%i blank [%i] [%i]\n", numOfNode, listNode.size(), x, y);
		}
	}
	return 0;
}

int blank_x(Node *p){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if (p->cell[i][j] == 0)
				return i;
		}
	}
}

int blank_y(Node *p){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if (p->cell[i][j] == 0)
				return j;
		}
	}
}

int check(Node *p){
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			if (p->cell[i][j] != (i * N + j))
				return 0;
	return 1;
}

int result(Node *p){
	if (p->parent == NULL){
		return 0;
	}
	result(p->parent);
	switch (p->action){
		case UP:
			puts("up");
			break;
		case DOWN:
			puts("down");
			break;
		case LEFT:
			puts("left");
			break;
		case RIGHT:
			puts("right");
			break;
	}
//	system("cls");
//	for(int i = 0; i < N; i++){
//		for(int j = 0; j < N; j++)
//			printf("%2i ", p->cell[i][j]);
//		puts("");
//	}
//	Sleep(100);
}

int exist(Node *p){
	//return existArr[key(p)];
	return 0;
}

int mark(Node *p){
	//existArr[key(p)] = 1;
	return 1;
}

int freeNode(Node *p){
	for(int i = 0; i < N; i++){
		free(p->cell[i]);
	}
	free(p->cell);
	free(p);
}

int copy(Node *a, Node *b){
	a->action = b->action;
	a->f = b->f;
	a->g = b->g;
	a->parent = b->parent;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			a->cell[i][j] = b->cell[i][j];
		}
	}
	return 0;
}
