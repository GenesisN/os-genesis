// This program mimics the grep command in UNIX
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc < 3){
		exit(0);
	}
	
	for (int i=2; i<argc; i++){
		FILE* fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("cannot open file");
			return 0;	
		}
		
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		
		while ((read = getline(&line, &len, fp)) != -1 ){
			if(strstr(line, argv[1])){
				printf("%s", line);
				
			}
		}
		
		fclose(fp);
	}
	return 0;
}
