#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv){
	int N = atoi(argv[1]);
	srand(time(NULL));
	srand(time(NULL) + rand());
	srand(time(NULL) + rand() + rand());
	char fn[1000];
	char tmp[1000];
	itoa(N, tmp, 10);
	strcpy(fn, "shuffle_");
	strcat(fn, tmp);
	strcat(fn, ".txt");
	FILE *f = fopen(fn, "w");
	fprintf(f, "%i\n", N);
	int oldNum = rand() % 4;;
	int newNum;
	for(int i = 0; i < N; i++){
		do{
			newNum = rand() % 4;
		} while (abs(newNum - oldNum) % 2 == 0);
		fprintf(f, "%i\n", newNum);
		oldNum = newNum;
	}
	fclose(f);
	return 0;
}
