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

using namespace std;


char fi[] = "npuzzle.txt";

int readData(void);
int init(void);					// Preprocess
int result(Node *p);			// Print solution
int print(void);				// print current state



int main(void){
	readData();
//	puts("readData ok");
	if (!checkSolvable(node)){
		puts("Unsolvable.");
		return 0;
	};
	init();
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
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			printf("%3i", node->cell[i][j]);
		}
		puts("");
	}
}

int readData(void){
	FILE *f = fopen(fi, "r");
	if (!f){
		exit(0);
	}
	fscanf(f, "%i", &N);
	node = newNode();
	char **p = node->cell;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			fscanf(f, "%i", &p[i][j]);
		}
	}
	fclose(f);
	node->f = 0;
	node->g = 0;
	return 0;
}

int swap(char &x, char &y){
	char buf = x;
	x = y;
	y = buf;
	return 0;
}

int init(void){
	node->parent = NULL;
	node->action = 0;
	calculate(node);
	newcutoff = cutoff = node->f;
	listNode.clear();
	listNode.push_back(node);
	setNode.clear();
	setNode.insert(node);
}

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
}
