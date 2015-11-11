#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main(void){
	FILE *f = fopen("test.bat", "w");
	if (!f)
		return 0;
	for(int i = 1; i < 200; i++){
		for(int j = 0; j < 10; j++){
			fprintf(f, "arg_npuzzle1.exe %i\n", 10 * i);
		}
	}
	fclose(f);
	return 0;
}
