#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define N 1000

int main(void){
	srand(time(NULL));
	char fn[1000];
	char tmp[1000];
	itoa(N, tmp, 10);
	strcpy(fn, "shuffle_");
	strcat(fn, tmp);
	strcat(fn, ".txt");
	FILE *f = fopen(fn, "w");
	fprintf(f, "%i\n", N);
	for(int i = 0; i < N; i++){
		fprintf(f, "%i\n", rand() % 4);
	}
	fclose(f);
	return 0;
}
