#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <string.h>
#include "cc.h"

int main(int argc, char *argv[]){
	CComp comp;
	char *fname;
	int index;
	FILE *fp;
	
	if (argc != 2){
		fprintf(stderr, "Usage: ./ccmatch description\n");
		exit(1);
	}

	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";
	fp = fopen(fname, "r");
	if (!fp){
		perror("fopen\n");
		exit(2);
	}
	flock(fileno(fp), LOCK_SH);

	index = 1;
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	while ((fread(&comp, sizeof(CComp), 1, fp)) > 0){
		if (strstr(comp.maker, argv[1]) || strstr(comp.model, argv[1]) || strstr(comp.desc, argv[1])){
			printf("\nMaker: %s\n", comp.maker);
   		printf("Model: %s\n", comp.model);
   		printf("Year: %d\n", comp.year);
   		printf("CPU: %s\n", comp.cpu);
   		printf("ID: %d\n", comp.id);
   		printf("Desc: %s\n", comp.desc);
			printf("----------------------\n");
		}
		index++;
	}
	flock(fileno(fp), LOCK_UN);
	fclose(fp);
	exit(0);
}
