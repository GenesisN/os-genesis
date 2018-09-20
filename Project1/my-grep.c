// This program mimics the grep command in UNIX
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {

//check if there are up to 3 arguments (executable output, word,file). If not then exit
	if (argc < 3){
		exit(0);
	}
	// open the file and check. If there is no such file in the directory, print error message
	for (int i=2; i<argc; i++){
		FILE* fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("cannot open file");
			return 0;	
		}
	// if the file contains anyline,check if it contains the phrase in argument 1.
	// if yes, then print the line
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
