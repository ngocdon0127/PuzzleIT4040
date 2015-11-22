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
#include "Heuristic.cpp"
#include "Algorithm.cpp"

//#define N 4
#define NUMSUFF 1000

using namespace std;

//typedef unsigned char datatype;

char fi[] = "npuzzle.txt";
//char fn[1000];
//char tmp[1000];

//char *existArr;
//int sizeAlloc = 0;


int countShuffle = 0;			// No. of step to shuffle Puzzle

int (*function[4])(void);		// Use in analyzing only

int readData(void);
int init(void);
int init1(void);
int result(Node *p);			// Print solution

int up(void);					// blank up.    For analyzing only
int down(void);					// blank down.  For analyzing only
int left(void);					// blank left.  For analyzing only
int right(void);				// blank right. For analyzing only
int print(void);				// print current state



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
		puts("Problem:");
		print();
		puts("\nChoose Heuristic Function:");
		puts("1. Manhattan");
		puts("2. Linear Conflict");
		puts("3. Tiles out of row and column");
		puts("4. Pythagorean (not admissible)");
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
	iDAStarSearch();
	double y = ti.getElapsedTime();
	system("cls");
	result(goal);
	puts("");
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

int print(void){
//	system("cls");
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

int result(Node *p){
	if (p->parent == NULL){
		return 0;
	}
	result(p->parent);
	switch (p->action){
		case UP:
			printf("Up ");
			break;
		case DOWN:
			printf("Down ");
			break;
		case LEFT:
			printf("Left ");
			break;
		case RIGHT:
			printf("Right ");
			break;
	}
//	puts("");
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
