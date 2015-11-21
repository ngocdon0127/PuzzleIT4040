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
#define FINISH_STATE 1000

using namespace std;

typedef unsigned char datatype;


char fi[] = "npuzzle.txt";
char fn[1000];
char tmp[1000];


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
	N = 3;
//	function[0] = &up;
//	function[2] = &down;
//	function[1] = &left;
//	function[3] = &right;
//	startState = atoi(argv[1]);
//	itoa(numShuffle, tmp, 10);
//	strcpy(fn, "shuffle_");
//	strcat(fn, tmp);
//	strcat(fn, ".txt");
//	char command[1000];
//	strcpy(command, "shuffle_arg.exe ");
//	strcat(command, tmp);
//	system(command);
	//readData()
	//init();
	FILE *f = fopen("startState.txt", "w");
	if (!f){
		return 0;
	}
	f = fopen("result.txt", "w");
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
		FILE *f = fopen("startState.txt", "a");
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
		for(choice = '1'; choice <= '5'; choice++){
	//		if (choice == '4')
	//			continue;
			
			// A*
			numOfNode = 0;
			FILE *fresult;
			fresult = fopen("result.txt", "a");
			if (!fresult){
				continue;
			}
			Timer ti;
			init();
//			puts("\ninit done");
			treeSearch();
			double y = ti.getElapsedTime();
			fprintf(fresult, "%-3i: heuristic %c :      %-9.0f %-3i %10.4f s | ", startState, choice, numOfNode, deep, y);
			fclose(fresult);
			printf("A*  : done Heuristic %c of test %i.\n", choice, startState);
			int sttHeuristic = choice - 49;
			if (deep >= 0){
				aSolved[sttHeuristic]++;
				if (startState == 0){
					aMaxTime[sttHeuristic] = aMinTime[sttHeuristic] = y;
					aMinNode[sttHeuristic] = aMaxNode[sttHeuristic] = numOfNode;
				}
				else{
					aMaxTime[sttHeuristic] = aMaxTime[sttHeuristic] < y ? y : aMaxTime[sttHeuristic];
					aMinTime[sttHeuristic] = aMinTime[sttHeuristic] > y ? y : aMinTime[sttHeuristic];
					aMaxNode[sttHeuristic] = aMaxNode[sttHeuristic] < numOfNode ? numOfNode : aMaxNode[sttHeuristic];
					aMinNode[sttHeuristic] = aMinNode[sttHeuristic] > numOfNode ? numOfNode : aMinNode[sttHeuristic];
				}
				aSumTime[sttHeuristic] += y;
				aSumNode[sttHeuristic] += numOfNode;
				aSumDeep[sttHeuristic] += deep;
			}
			freeMemory();
			node = newNode();
			copy(node, backup);
			
			// IDA*
			numOfNode = 0;
			fresult = fopen("result.txt", "a");
			if (!fresult){
				continue;
			}
			Timer ti1;
			init();
			//puts("\ninit done");
			treeSearch1();
			double y1 = ti1.getElapsedTime();
			fprintf(fresult, "%-9.0f %-3i %10.4f s\n",numOfNode, deep, y1);
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
		FILE *fresult = fopen("result.txt", "a");
		if (!fresult){
				return 0;
		}
		fprintf(fresult, "\n");
		fclose(fresult);
		
	}
	system("cls");
	f = fopen("analysis-8puzzle.txt", "w");
	if (!f){
		return 0;
	}
	puts("Start writing result.");
	fprintf(f, "%-10s%8s  %8s %10s %10s     %10s  %10s%10s%10s\n", "Heuristic", "Solved", "MinTime", "MaxTime", "AvgTime", "MinNode", "MaxNode", "AvgNode", "AvgDeep");
	for(int i = 0; i < NUM_HEURISTIC; i++){
		fprintf(f, "%8i: ", i + 1);
		fprintf(f, "%4i/%-4i   ", aSolved[i], FINISH_STATE - START_STATE);
		fprintf(f, "%-10.4f %-10.4f ", aMinTime[i], aMaxTime[i]);
		fprintf(f, "%-10.4f ", aSumTime[i] / aSolved[i]);
		fprintf(f, "%10.0f %10.0f ", aMinNode[i], aMaxNode[i]);
		fprintf(f, "%10.1f ", aSumNode[i] / aSolved[i]);
		fprintf(f, "%5i\n\n", aSumDeep[i] / aSolved[i]);
	}
	
	for(int i = 0; i < NUM_HEURISTIC; i++){
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
