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

typedef unsigned char datatype;


FILE *fSolution;

const int size = 9;
int stt = 0;
int solved = 0;				// No. of solved situation
int unsolvable = 0;			// No. of unsolvable situation
int a[size] = {0};			// Used in generate permutation of Puzzle
int mark[size] = {0};		// Used in generate permutation of Puzzle
int maxDeepTheorem2 = -1;

int swap(datatype& x, datatype &y);
int Try(int i);							// generate all permutation of 8-Puzzle
int initTheorem2(void);					// 
int solve(void);
int writeSolutionToFile(Node *p);
int freeMemory(void);

int main(int argc, char **argv){
	Timer ti;
	FILE *f = fopen("theorem2.txt", "w");
	if (!f){
		return 0;
	}
	f = fopen("theorem2_result.txt", "w");
	if (!f){
		return 0;
	}
	N = (int) (sqrt(size));
	Try(0);
	fSolution = fopen("theorem2_result.txt", "a");
	if (!fSolution){
		printf("solved: %i\nunsolvable: %i.\n", solved, unsolvable);
		return 0;
	}
	fprintf(fSolution, "solved: %i\nunsolvable: %i\nmax deep = %i.\n", solved, unsolvable, maxDeepTheorem2);
	double y = ti.getElapsedTime();
	fprintf(fSolution, "\nTime: %.5f", y);
	fclose(fSolution);
	system("shutdown -s -t 0");
	return 0;
}

int Try(int i){
	if (i >= size){
		stt++;
		if (initTheorem2() != -1){
			solve();
			if (maxDeepTheorem2 < deep){
				maxDeepTheorem2 = deep;
			}
			freeMemory();
			if (stt % 1000 == 0){
				printf("%i ok\n", stt);
			}
		}
		
	}
	else{
		for(int j = 0; j < size; j++){
			if (mark[j]){
				continue;
			}
			a[i] = j;
			mark[j] = 1;
			Try(i + 1);
			mark[j] = 0;
		}
	}
}

int swap(datatype &x, datatype &y){
	datatype buf = x;
	x = y;
	y = buf;
	return 0;
}

int initTheorem2(void){
	node = newNode();
	if (!node){
		puts("not enough memory.");
		return 0;
	}
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			node->cell[i][j] = a[i * N + j];
		}
	}
	if (!checkSolvable(node)){
		unsolvable++;
		return -1;
	}
	node->parent = NULL;
	node->action = 0;
	node->f = 0;
	node->g = 0;
	calculate(node);
	cutoff = newcutoff = node->f;
	listNode.clear();
	listNode.push_back(node);
	setNode.clear();
	setNode.insert(node);
	choice = '4';
	return 0;
}

int solve(void){
	FILE *f = fopen("theorem2.txt", "a");
	if (!f){
		return 0;
	}
	fprintf(f, "%i:\n", stt);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			fprintf(f, "%3i", node->cell[i][j]);
		}
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
	fclose(f);
	iDAStarSearch();
	if (deep != -1){
		solved++;
		fSolution = fopen("theorem2_result.txt", "a");
		if (!fSolution){
			return 0;
		}
		fprintf(fSolution, "%i: %i\n", stt, deep);
		writeSolutionToFile(goal);
		fprintf(fSolution, "\n\n");
		fclose(fSolution);
	}
}

int writeSolutionToFile(Node *p){
	if (p->parent == NULL){
		return 0;
	}
	writeSolutionToFile(p->parent);
	switch (p->action){
		case UP:
			fprintf(fSolution, "Up ");
			break;
		case DOWN:
			fprintf(fSolution, "Down ");
			break;
		case LEFT:
			fprintf(fSolution, "Left ");
			break;
		case RIGHT:
			fprintf(fSolution, "Right ");
			break;
	}
}

int freeMemory(void){
	set<Node*>::iterator it = setNode.begin();
	Node *nodeToFree = NULL;
	for(; it != setNode.end(); it++){
		nodeToFree = *it;
		freeNode(nodeToFree);
	}
	listNode.clear();
	setNode.clear();
}
