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

#define N 4
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define NUM_HEURISTIC 4
#define START_STATE 0
#define FINISH_STATE 10

using namespace std;

typedef unsigned char datatype;

struct Node{
	datatype cell[N][N];
	int f;
	int g;
	Node *parent;
	int action;
};

char fi[] = "npuzzle.txt";
char fn[1000];
char tmp[1000];

char *existArr;
float numOfNode = 1;
int sizeAlloc = 0;
list<Node*> listNode;
set<Node*> setNode;
Node *node;
int deep = 0;
char choice;
int timeAStar = 5;
int timeIDAStar = 5;

int cutoff = 0;
int newcutoff = 0;
int startState = 0;

double aMinTime[NUM_HEURISTIC] = {0.0};
double aMaxTime[NUM_HEURISTIC] = {0.0};
double aSumTime[NUM_HEURISTIC] = {0.0};
float aMinNode[NUM_HEURISTIC] = {0.0};
float aMaxNode[NUM_HEURISTIC] = {0.0};
float aSumNode[NUM_HEURISTIC] = {0.0};
int aSumDeep[NUM_HEURISTIC] = {0};
int aSolved[NUM_HEURISTIC] = {0};

double iDAMinTime[NUM_HEURISTIC] = {0.0};
double iDAMaxTime[NUM_HEURISTIC] = {0.0};
double iDASumTime[NUM_HEURISTIC] = {0.0};
float iDAMinNode[NUM_HEURISTIC] = {0.0};
float iDAMaxNode[NUM_HEURISTIC] = {0.0};
float iDASumNode[NUM_HEURISTIC] = {0.0};
int iDASumDeep[NUM_HEURISTIC] = {0};
int iDASolved[NUM_HEURISTIC] = {0};

int (*function[4])(void);

int generatePuzzle(void);
int readData(void);
int init(void);
int init1(void);
int print(void);
int mapIndex(int row, int column);
int treeSearch(void);
int treeSearch1(void);
//int calculateCost(Node *p);
int calculate(Node *p);
int heuristic1(Node *p);
int heuristic2(Node *p);
int heuristic3(Node *p);
int heuristic4(Node *p);
int check(Node *p);
int checkSolvable(Node *p);
int result(Node *p);
int blank_x(Node *p);
int blank_y(Node *p);
int key(Node *p);
int exist(Node *p);
int mark(Node *p);
int swap(datatype& x, datatype &y);
int up(void);
int down(void);
int left(void);
int right(void);
Node* pick(void);

int main(int argc, char **argv){
	srand(time(NULL));
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
			puts("generated");
		} while (!checkSolvable(node));
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
		for(choice = '4'; choice <= '4'; choice++){
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
			init1();
			//puts("\ninit done");
			treeSearch();
			double y = ti.getElapsedTime();
			fprintf(fresult, "%-3i: heuristic %c :      %-9.0f %-3i %6.4f s | ", startState, choice, numOfNode, deep, y);
			fclose(fresult);
			printf("done %c of test %i.\n", choice, startState);
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
			
			// IDA*
			numOfNode = 0;
			fresult = fopen("result.txt", "a");
			if (!fresult){
				continue;
			}
			Timer ti1;
			init1();
			//puts("\ninit done");
			treeSearch1();
			double y1 = ti1.getElapsedTime();
			fprintf(fresult, "%-9.0f %-3i %6.4f s\n",numOfNode, deep, y1);
			fclose(fresult);
			printf("done %c of test %i.\n", choice, startState);
			if (deep >= 0){
				iDASolved[sttHeuristic]++;
				if (startState == 0){
					iDAMaxTime[sttHeuristic] = iDAMinTime[sttHeuristic] = y;
					iDAMinNode[sttHeuristic] = iDAMaxNode[sttHeuristic] = numOfNode;
				}
				else{
					iDAMaxTime[sttHeuristic] = iDAMaxTime[sttHeuristic] < y ? y : iDAMaxTime[sttHeuristic];
					iDAMinTime[sttHeuristic] = iDAMinTime[sttHeuristic] > y ? y : iDAMinTime[sttHeuristic];
					iDAMaxNode[sttHeuristic] = iDAMaxNode[sttHeuristic] < numOfNode ? numOfNode : iDAMaxNode[sttHeuristic];
					iDAMinNode[sttHeuristic] = iDAMinNode[sttHeuristic] > numOfNode ? numOfNode : iDAMinNode[sttHeuristic];
				}
				iDASumTime[sttHeuristic] += y;
				iDASumNode[sttHeuristic] += numOfNode;
				iDASumDeep[sttHeuristic] += deep;
			}
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
	for(int i = 3; i < NUM_HEURISTIC; i++){
		fprintf(f, "%8i: ", i + 1);
		fprintf(f, "%4i/%-4i   ", aSolved[i], FINISH_STATE - START_STATE);
		fprintf(f, "%-10.4f %-10.4f ", aMinTime[i], aMaxTime[i]);
		fprintf(f, "%-10.4f ", aSumTime[i] / aSolved[i]);
		fprintf(f, "%10.0f %10.0f ", aMinNode[i], aMaxNode[i]);
		fprintf(f, "%10.1f ", aSumNode[i] / aSolved[i]);
		fprintf(f, "%5i\n\n", aSumDeep[i] / aSolved[i]);
	}
	
	for(int i = 3; i < NUM_HEURISTIC; i++){
		fprintf(f, "%8i: ", i + 1);
		fprintf(f, "%4i/%-4i   ", iDASolved[i], FINISH_STATE - START_STATE);
		fprintf(f, "%-10.4f %-10.4f ", iDAMinTime[i], iDAMaxTime[i]);
		fprintf(f, "%-10.4f ", iDASumTime[i] / iDASolved[i]);
		fprintf(f, "%10.0f %10.0f ", iDAMinNode[i], iDAMaxNode[i]);
		fprintf(f, "%10.1f ", iDASumNode[i] / iDASolved[i]);
		fprintf(f, "%5i\n\n", iDASumDeep[i] / iDASolved[i]);
	}
	puts("Start freeing");
	set<Node*>::iterator it = setNode.begin();
	Node *nodeToFree = NULL;
	for(; it != setNode.end(); it++){
		nodeToFree = *it;
		free(nodeToFree);
	}
	puts("End loop");
	listNode.clear();
	setNode.clear();
	
//	Sleep(1000);
	return 0;
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

int generatePuzzle(void){
	node = new Node;
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
//	print();
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
	getch();
}

int readData(void){
	FILE *f = fopen(fi, "r");
	if (!f){
		exit(0);
	}
	node = (Node*) malloc(sizeof(Node));
	datatype *p = (datatype*) &(node->cell[0][0]);
	for(int i = 0; i < N * N; i++){
		fscanf(f, "%i", p + i);
	}
	fclose(f);
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

int swap(datatype &x, datatype &y){
	datatype buf = x;
	x = y;
	y = buf;
	return 0;
}

int init(void){
	node->g = 0;
	node->parent = NULL;
	node->action = 0;
	calculate(node);
	newcutoff = cutoff = node->f;
	listNode.clear();
	listNode.push_back(node);
	setNode.clear();
	setNode.insert(node);
	sizeAlloc = 0;
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

int init1(void){
//	node = new Node;
//	srand(time(NULL));
//	datatype *m = &(node->cell[0][0]);
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
	node->g = 0;
	node->parent = NULL;
	node->action = 0;
	calculate(node);
	newcutoff = cutoff = 0;
	listNode.clear();
	listNode.push_back(node);
	setNode.clear();
	setNode.insert(node);
}

int key(Node *p){
	int value = 0;
	datatype *x = (datatype*) &(p->cell[0][0]);
	datatype m = *x;
	int k = 0;
	int l1 = N * N;
	int l =  l1 - 1;
	for(int i = 0; i < l; i++){
		m = *(x + i);
		k = m * (int) pow((l1), i);
		//printf("m = %i k = %i\n", m, k);
		value += k;
	}
	if (value < 0)
		puts("hehe");
	if (value > sizeAlloc)
		puts("hihi");
	return value;
}

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
	}
	p->f = p->g + sum;
}

int heuristic1(Node *p){ // Manhattan
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

int heuristic2(Node *p){ // Manhattan + Linear Conflict
	
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

int heuristic4(Node *p){ // Pythagorean - not admissable
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

int heuristic3(Node *p){ // Tiles out of row and column
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
		if (y > timeAStar){
			deep = -1;
			return 0;
		}
		p = pick();
		if (check(p)){
			result(p);
			deep = p->g;
			return 0;
		}
		x = blank_x(p);
		y = blank_y(p);
		// blank up
		if ((x > 0) && (p->action != DOWN)){
			p1 = new Node;
			setNode.insert(p1);
			*p1 = *p;
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
			p1 = new Node;
			setNode.insert(p1);
			*p1 = *p;
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
			p1 = new Node;
			setNode.insert(p1);
			*p1 = *p;
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
			p1 = new Node;
			setNode.insert(p1);
			*p1 = *p;
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
				free(nodeToFree);
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
				result(p);
				deep = p->g;
				return 0;
			}
			x = blank_x(p);
			y = blank_y(p);
			
			// blank up
			if ((x > 0) && (p->action != DOWN)){
				p1 = new Node;
				numOfNode++;
				setNode.insert(p1);
				*p1 = *p;
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
				p1 = new Node;
				numOfNode++;
				setNode.insert(p1);
				*p1 = *p;
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
				p1 = new Node;
				numOfNode++;
				setNode.insert(p1);
				*p1 = *p;
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
				p1 = new Node;
				numOfNode++;
				setNode.insert(p1);
				*p1 = *p;
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
//	switch (p->action){
//		case UP:
//			puts("up");
//			break;
//		case DOWN:
//			puts("down");
//			break;
//		case LEFT:
//			puts("left");
//			break;
//		case RIGHT:
//			puts("right");
//			break;
//	}
//	system("cls");
//	for(int i = 0; i < N; i++){
//		for(int j = 0; j < N; j++)
//			printf("%2i ", p->cell[i][j]);
//		puts("");
//	}
//	Sleep(10);
}

int exist(Node *p){
	//return existArr[key(p)];
	return 0;
}

int mark(Node *p){
	//existArr[key(p)] = 1;
	return 1;
}
