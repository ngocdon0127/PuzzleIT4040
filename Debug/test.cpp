#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define N 9
#define START 0
#define FINISH 15

int main(void){
	char fn[] = "npuzzle.txt";
	int p[20];
	int b[20];
	int count = 0;
	FILE *f = fopen(fn, "r");
	for(int i = START; i <= FINISH; i++)
		fscanf(f, "%i", p + i);
	fclose(f);
	int buf;
	int index1 = 0;
	int index2 = 0;
	int check = 0;
	for(; 1;){
		for(int i = START; i <= FINISH; i++)
			b[p[i]] = i;
		for(int i = START; i <= FINISH; i++)
			printf("%i ", p[i]);
		printf(" : ");
		for(int i = START; i <= FINISH; i++)
			printf("%i ", b[i]);
		puts("");
		index1 = b[0];
		index2 = b[index1];
		buf = p[index1];
		p[index1] = p[index2];
		p[index2] = buf;
		for(int i = START; i <= FINISH; i++)
			printf("%i ", p[i]);
		printf(" : ");
		for(int i = START; i <= FINISH; i++)
			printf("%i ", b[i]);
		puts("");
		check = 1;
		for(int i = START; i <= FINISH; i++)
			if (p[i] != i){
				printf("sai %i\n", i);
				check = 0;
				break;
			}
		printf("check %i\n", check);
		if (check){
			puts("break");
			break;
		}
		count++;
		Sleep(300);
	}
	printf("%i\n", count);
	for(int i = START; i <= FINISH; i++)
		printf("%i ", b[i]);
}
