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
int solved = 0;
int unsolvable = 0;
int a[size] = {0};
int mark[size] = {0};
int maxDeepTheorem2 = -1;

//double aMinTime[NUM_HEURISTIC] = {0.0};
//double aMaxTime[NUM_HEURISTIC] = {0.0};
//double aSumTime[NUM_HEURISTIC] = {0.0};
//float aMinNode[NUM_HEURISTIC] = {0.0};
//float aMaxNode[NUM_HEURISTIC] = {0.0};
//float aSumNode[NUM_HEURISTIC] = {0.0};
//int aSumDeep[NUM_HEURISTIC] = {0};
//int aSolved[NUM_HEURISTIC] = {0};
//
//double iDAMinTime[NUM_HEURISTIC] = {0.0};
//double iDAMaxTime[NUM_HEURISTIC] = {0.0};
//double iDASumTime[NUM_HEURISTIC] = {0.0};
//float iDAMinNode[NUM_HEURISTIC] = {0.0};
//float iDAMaxNode[NUM_HEURISTIC] = {0.0};
//float iDASumNode[NUM_HEURISTIC] = {0.0};
//int iDASumDeep[NUM_HEURISTIC] = {0};
//int iDASolved[NUM_HEURISTIC] = {0};


int generatePuzzle(void);
int readData(void);
int print(void);
int result(Node *p);
int swap(datatype& x, datatype &y);
int Try(int i);
int initTheorem2(void);
int solve(void);
int writeSolutionToFile(Node *p);
int freeMemory(void);
//int up(void);
//int down(void);
//int left(void);
//int right(void);

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
//	for(startState = START_STATE; startState < FINISH_STATE; startState++){
//		do{
//			generatePuzzle();
//			puts("generated");
//		} while (!checkSolvable(node));
//		FILE *f = fopen("startState.txt", "a");
//		if (!f){
//			return 0;
//		}
//		fprintf(f, "%i\n", startState);
//		for(int i = 0; i < N; i++){
//			for(int j = 0; j < N; j++){
//				fprintf(f, "%3i", node->cell[i][j]);
//			}
//			fprintf(f, "\n");
//		}
//		fprintf(f, "\n");
//		fclose(f);
//		for(choice = '4'; choice <= '4'; choice++){
//			if (choice == '4')
//				continue;
//			
//			 A*
//			numOfNode = 0;
//			FILE *fresult;
//			fresult = fopen("result.txt", "a");
//			if (!fresult){
//				continue;
//			}
//			Timer ti;
//			init1();
//			puts("\ninit done");
//			aStarSearch();
//			double y = ti.getElapsedTime();
//			fprintf(fresult, "%-3i: heuristic %c :      %-9.0f %-3i %6.4f s | ", startState, choice, numOfNode, deep, y);
//			fclose(fresult);
//			printf("done %c of test %i.\n", choice, startState);
//			int sttHeuristic = choice - 49;
//			if (deep >= 0){
//				aSolved[sttHeuristic]++;
//				if (startState == 0){
//					aMaxTime[sttHeuristic] = aMinTime[sttHeuristic] = y;
//					aMinNode[sttHeuristic] = aMaxNode[sttHeuristic] = numOfNode;
//				}
//				else{
//					aMaxTime[sttHeuristic] = aMaxTime[sttHeuristic] < y ? y : aMaxTime[sttHeuristic];
//					aMinTime[sttHeuristic] = aMinTime[sttHeuristic] > y ? y : aMinTime[sttHeuristic];
//					aMaxNode[sttHeuristic] = aMaxNode[sttHeuristic] < numOfNode ? numOfNode : aMaxNode[sttHeuristic];
//					aMinNode[sttHeuristic] = aMinNode[sttHeuristic] > numOfNode ? numOfNode : aMinNode[sttHeuristic];
//				}
//				aSumTime[sttHeuristic] += y;
//				aSumNode[sttHeuristic] += numOfNode;
//				aSumDeep[sttHeuristic] += deep;
//			}
//			
//			 IDA*
//			numOfNode = 0;
//			fresult = fopen("result.txt", "a");
//			if (!fresult){
//				continue;
//			}
//			Timer ti1;
//			init1();
//			puts("\ninit done");
//			iDAStarSearch();
//			double y1 = ti1.getElapsedTime();
//			fprintf(fresult, "%-9.0f %-3i %6.4f s\n",numOfNode, deep, y1);
//			fclose(fresult);
//			printf("done %c of test %i.\n", choice, startState);
//			if (deep >= 0){
//				iDASolved[sttHeuristic]++;
//				if (startState == 0){
//					iDAMaxTime[sttHeuristic] = iDAMinTime[sttHeuristic] = y;
//					iDAMinNode[sttHeuristic] = iDAMaxNode[sttHeuristic] = numOfNode;
//				}
//				else{
//					iDAMaxTime[sttHeuristic] = iDAMaxTime[sttHeuristic] < y ? y : iDAMaxTime[sttHeuristic];
//					iDAMinTime[sttHeuristic] = iDAMinTime[sttHeuristic] > y ? y : iDAMinTime[sttHeuristic];
//					iDAMaxNode[sttHeuristic] = iDAMaxNode[sttHeuristic] < numOfNode ? numOfNode : iDAMaxNode[sttHeuristic];
//					iDAMinNode[sttHeuristic] = iDAMinNode[sttHeuristic] > numOfNode ? numOfNode : iDAMinNode[sttHeuristic];
//				}
//				iDASumTime[sttHeuristic] += y;
//				iDASumNode[sttHeuristic] += numOfNode;
//				iDASumDeep[sttHeuristic] += deep;
//			}
//		}
//		FILE *fresult = fopen("result.txt", "a");
//		if (!fresult){
//				return 0;
//		}
//		fprintf(fresult, "\n");
//		fclose(fresult);
//	}
//	system("cls");
//	f = fopen("analysis-8puzzle.txt", "w");
//	if (!f){
//		return 0;
//	}
//	puts("Start writing result.");
//	fprintf(f, "%-10s%8s  %8s %10s %10s     %10s  %10s%10s%10s\n", "Heuristic", "Solved", "MinTime", "MaxTime", "AvgTime", "MinNode", "MaxNode", "AvgNode", "AvgDeep");
//	for(int i = 3; i < NUM_HEURISTIC; i++){
//		fprintf(f, "%8i: ", i + 1);
//		fprintf(f, "%4i/%-4i   ", aSolved[i], FINISH_STATE - START_STATE);
//		fprintf(f, "%-10.4f %-10.4f ", aMinTime[i], aMaxTime[i]);
//		fprintf(f, "%-10.4f ", aSumTime[i] / aSolved[i]);
//		fprintf(f, "%10.0f %10.0f ", aMinNode[i], aMaxNode[i]);
//		fprintf(f, "%10.1f ", aSumNode[i] / aSolved[i]);
//		fprintf(f, "%5i\n\n", aSumDeep[i] / aSolved[i]);
//	}
//	
//	for(int i = 3; i < NUM_HEURISTIC; i++){
//		fprintf(f, "%8i: ", i + 1);
//		fprintf(f, "%4i/%-4i   ", iDASolved[i], FINISH_STATE - START_STATE);
//		fprintf(f, "%-10.4f %-10.4f ", iDAMinTime[i], iDAMaxTime[i]);
//		fprintf(f, "%-10.4f ", iDASumTime[i] / iDASolved[i]);
//		fprintf(f, "%10.0f %10.0f ", iDAMinNode[i], iDAMaxNode[i]);
//		fprintf(f, "%10.1f ", iDASumNode[i] / iDASolved[i]);
//		fprintf(f, "%5i\n\n", iDASumDeep[i] / iDASolved[i]);
//	}
//	puts("Start freeing");
//	set<Node*>::iterator it = setNode.begin();
//	Node *nodeToFree = NULL;
//	for(; it != setNode.end(); it++){
//		nodeToFree = *it;
//		free(nodeToFree);
//	}
//	puts("End loop");
//	listNode.clear();
//	setNode.clear();
//	
//	Sleep(1000);
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
		
//		for(int k = 0; k < size; k++){
//			printf("%3i", a[k]);
//		}
//		puts("");
		stt++;
		//printf("dit me may %i\n", stt);
//		if (stt > 10000){
//			return 0;
//		}
//		puts("clgt");
		if (initTheorem2() != -1){
//			printf("start solve %i\n", stt);
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
//	if (node){
////		puts("node need to be freed");
//		freeNode(node);
//	}
//	puts("1");
	node = newNode();
	if (!node){
		puts("not enough memory.");
		return 0;
	}
//	puts("alloc ok");
//	printf("cal N ok N = %i\n", N);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
//			printf("%i %i ", a[i * N + j], node->cell[i][j]);
			node->cell[i][j] = a[i * N + j];
//			printf("node[i][j] = %i", node->cell[i][j]);
		}
	}
//	puts("2");
	if (!checkSolvable(node)){
		unsolvable++;
		return -1;
	}
//	puts("3");
	node->parent = NULL;
	node->action = 0;
	node->f = 0;
	node->g = 0;
	calculate(node);
//	cutoff = newcutoff = node->f;
	listNode.clear();
	listNode.push_back(node);
	setNode.clear();
	setNode.insert(node);
	choice = '4';
//	puts("init ok");
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
//	puts("free");
	set<Node*>::iterator it = setNode.begin();
//	puts("set begin");
	Node *nodeToFree = NULL;
//	puts("set null");
	for(; it != setNode.end(); it++){
		nodeToFree = *it;
//		puts("set nodeToFree");
		freeNode(nodeToFree);
//		puts("free nodeToFree");
	}
//	puts("End loop");
	listNode.clear();
	setNode.clear();
//	puts("free ok");
}
