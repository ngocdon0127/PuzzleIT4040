#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <list>
#include <malloc.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include <Timer.h>
#include <Timer.cpp>
#include <ctype.h>

#define MAX 6
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

using namespace std;
typedef unsigned char datatype;

struct Node{
	datatype cell[MAX][MAX];
	int f;
	int g;
	Node *parent;
	int action;
};

int N;
Node *node;
int count = 0;

int up(void);
int down(void);
int left(void);
int right(void);
int swap(datatype& x, datatype &y);
int blank_x(Node *p);
int blank_y(Node *p);
int printPuzzle(void);

int main(void){
	printf("N? : ");
	scanf("%i", &N);
	node = new Node;
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			node->cell[i][j] = i * N + j;
	printPuzzle();
	char move;
	do{
		move = toupper(getch());
		switch (move){
			case 'W':
				up();
				printPuzzle();
				count++;
				break;
			case 'S':
				down();
				printPuzzle();
				count++;
				break;
			case 'A':
				left();
				printPuzzle();
				count++;
				break;
			case 'D':
				right();
				printPuzzle();
				count++;
				break;
			default:
				break;
		}
	} while (move != 'E');
	FILE *f = fopen("npuzzle.txt", "w");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++)
			fprintf(f, "%i ", node->cell[i][j]);
		fprintf(f, "\n");
	}
	fclose(f);
	return 0;
}

int printPuzzle(void){
	system("cls");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++)
			printf("%i ", node->cell[i][j]);
		printf("\n");
	}
	printf("count: %i\n", count);
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
