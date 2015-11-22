#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main(void){
	FILE *f = fopen("test.bat", "w");
	if (!f)
		return 0;
	for(int i = 1; i < 1000; i++){
			fprintf(f, "arg_npuzzle1.exe %i\n", i);
	}
	fclose(f);
	return 0;
}
