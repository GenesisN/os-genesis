//This program mimics the cat command in UNIX

#include <stdio.h>
#include <stdlib.h>
// check is there are up to 2 arguments. If not, exit
int main(int argc, char** argv) {
	if (argc < 2){
		exit(0);
	}
//check if the file provided exists in the directory. If not, print error message
	for (int i=1; i<argc; i++){
		FILE* fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("cannot open file");
			return 0;	
		}
//if the file contains anyline, print them out in order; if not print nothing
		int j;
		while ((j = getc(fp)) !=EOF){
			putchar(j);
		}
		fclose(fp);
	}
	return 0;
}


















