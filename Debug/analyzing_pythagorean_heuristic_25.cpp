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

#define NUM_HEURISTIC 5
#define START_STATE 0
#define FINISH_STATE 10

using namespace std;

typedef unsigned char datatype;


char fProblemName[] = "pythagorean_start_state_25.txt";
char fResultName[] = "pythagorean_result_25.txt";
char fAnalysisName[] = "pythagorean_analysis_25.txt";


int startState = 0;

double aMinTime[NUM_HEURISTIC] = {0.0};
double aMaxTime[NUM_HEURISTIC] = {0.0};
double aSumTime[NUM_HEURISTIC] = {0.0};
float aMinNode[NUM_HEURISTIC] = {0.0};
float aMaxNode[NUM_HEURISTIC] = {0.0};
float aSumNode[NUM_HEURISTIC] = {0.0};
int aSumDeep[NUM_HEURISTIC] = {0};
int aSolved[NUM_HEURISTIC] = {0};
Node *backup;

double iDAMinTime[NUM_HEURISTIC] = {0.0};
double iDAMaxTime[NUM_HEURISTIC] = {0.0};
double iDASumTime[NUM_HEURISTIC] = {0.0};
float iDAMinNode[NUM_HEURISTIC] = {0.0};
float iDAMaxNode[NUM_HEURISTIC] = {0.0};
float iDASumNode[NUM_HEURISTIC] = {0.0};
int iDASumDeep[NUM_HEURISTIC] = {0};
int iDASolved[NUM_HEURISTIC] = {0};


int generatePuzzle(void);
int readData(void);
int init(void);
int init1(void);
int print(void);
int result(Node *p);
int key(Node *p);
int exist(Node *p);
int mark(Node *p);
int swap(datatype& x, datatype &y);
int freeMemory(void);
//int up(void);
//int down(void);
//int left(void);
//int right(void);

int main(int argc, char **argv){
	srand(time(NULL));
	N = 5;
	FILE *f = fopen(fProblemName, "w");
	if (!f){
		return 0;
	}
	f = fopen(fResultName, "w");
	if (!f){
		return 0;
	}
	for(startState = START_STATE; startState < FINISH_STATE; startState++){
		do{
			generatePuzzle();
//			puts("generated");
		} while (!checkSolvable(node));
		backup = newNode();
		copy(backup, node);
		FILE *f = fopen(fProblemName, "a");
		if (!f){
			return 0;
		}
		fprintf(f, "%i\n", startState);
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				fprintf(f, "%3i", node->cell[i][j]);
			}
			fprintf(f, "\n");
		}
		fprintf(f, "\n");
		fclose(f);
		for(choice = '4'; choice <= '4'; choice++){
			
			// IDA*
			numOfNode = 0;
			FILE *fresult = fopen(fResultName, "a");
			if (!fresult){
				continue;
			}
			int sttHeuristic = choice - 49;
			Timer ti1;
			init();
			//puts("\ninit done");
			iDAStarSearch();
			double y1 = ti1.getElapsedTime();
			fprintf(fresult, "%-3i: heuristic %c :      %-9.0f %-3i %10.4f s\n", startState, choice, numOfNode, deep, y1);
			fclose(fresult);
			printf("IDA*: done Heuristic %c of test %i.\n", choice, startState);
			if (deep >= 0){
				iDASolved[sttHeuristic]++;
				if (startState == 0){
					iDAMaxTime[sttHeuristic] = iDAMinTime[sttHeuristic] = y1;
					iDAMinNode[sttHeuristic] = iDAMaxNode[sttHeuristic] = numOfNode;
				}
				else{
					iDAMaxTime[sttHeuristic] = iDAMaxTime[sttHeuristic] < y1 ? y1 : iDAMaxTime[sttHeuristic];
					iDAMinTime[sttHeuristic] = iDAMinTime[sttHeuristic] > y1 ? y1 : iDAMinTime[sttHeuristic];
					iDAMaxNode[sttHeuristic] = iDAMaxNode[sttHeuristic] < numOfNode ? numOfNode : iDAMaxNode[sttHeuristic];
					iDAMinNode[sttHeuristic] = iDAMinNode[sttHeuristic] > numOfNode ? numOfNode : iDAMinNode[sttHeuristic];
				}
				iDASumTime[sttHeuristic] += y1;
				iDASumNode[sttHeuristic] += numOfNode;
				iDASumDeep[sttHeuristic] += deep;
			}
			
			freeMemory();
			node = newNode();
			copy(node, backup);
		}
		FILE *fresult = fopen(fResultName, "a");
		if (!fresult){
				return 0;
		}
		fprintf(fresult, "\n");
		fclose(fresult);
		
	}
	system("cls");
	f = fopen(fAnalysisName, "w");
	if (!f){
		return 0;
	}
	puts("Start writing result.");
	fprintf(f, "%-10s%8s  %8s %10s %10s     %10s  %10s%10s%10s\n", "Heuristic", "Solved", "MinTime", "MaxTime", "AvgTime", "MinNode", "MaxNode", "AvgNode", "AvgDeep");
	
	for(int i = 3; i < 4; i++){
		fprintf(f, "%8i: ", i + 1);
		fprintf(f, "%4i/%-4i   ", iDASolved[i], FINISH_STATE - START_STATE);
		fprintf(f, "%-10.4f %-10.4f ", iDAMinTime[i], iDAMaxTime[i]);
		fprintf(f, "%-10.4f ", iDASumTime[i] / iDASolved[i]);
		fprintf(f, "%10.0f %10.0f ", iDAMinNode[i], iDAMaxNode[i]);
		fprintf(f, "%10.1f ", iDASumNode[i] / iDASolved[i]);
		fprintf(f, "%5i\n\n", iDASumDeep[i] / iDASolved[i]);
	}
//	puts("Start freeing");
	freeMemory();
	
//	Sleep(1000);
	puts("writing result successfully.");
	return 0;
}

int freeMemory(void){
	set<Node*>::iterator it = setNode.begin();
	Node *nodeToFree = NULL;
	for(; it != setNode.end(); it++){
		nodeToFree = *it;
		freeNode(nodeToFree);
	}
//	puts("End loop");
	listNode.clear();
	setNode.clear();
}

int generatePuzzle(void){
	node = newNode();
	int num;
	int range = N * N;
	int a[range];
	for(int i = 0; i < range; i++)
		a[i] = 0;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			do{
				num = rand() % range;
//				printf("num = %i\n", num);
			} while (a[num]);
			node->cell[i][j] = num;
			a[num] = 1;
		}
	}
}


int swap(datatype &x, datatype &y){
	datatype buf = x;
	x = y;
	y = buf;
	return 0;
}

int init(void){
	node->g = 0;
	node->f = 0;
	node->parent = NULL;
	node->action = 0;
	calculate(node);
	newcutoff = cutoff = node->f;
	listNode.clear();
	listNode.push_back(node);
	setNode.clear();
	setNode.insert(node);
}
