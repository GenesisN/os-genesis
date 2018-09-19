//This program mimics the cat command in UNIX

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc < 2){
		exit(0);
	}
	for (int i=1; i<argc; i++){
		FILE* fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("cannot open file");
			return 0;	
		}
		int j;
		while ((j = getc(fp)) !=EOF){
			putchar(j);
		}
		fclose(fp);
	}
	return 0;
}


















