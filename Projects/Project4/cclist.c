#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "cc.h"

int main(int argc, char *argv[]){
	CComp comp;
	char *fname;
	int index;
	FILE *fp;

	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";
	fp = fopen(fname, "r");
	if (!fp){
		perror("fopen");
		exit(1);
	}
	flock(fileno(fp), LOCK_SH);

	index = 1;
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	while((fread(&comp, sizeof(CComp), 1, fp)) > 0){
		if (index == comp.id){
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
