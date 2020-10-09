#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <string.h>
#include "cc.h"

int main(int argc, char *argv[]){
	CComp comp;
	char *fname;
	int index, year1, year2, temp;
	FILE *fp;
	
	if (argc != 3){
		fprintf(stderr, "Usage: ./ccyear year1 year2\n");
		exit(1);
	}

	year1 = atoi(argv[1]);
	year2 = atoi(argv[2]);
	if (year1 > year2){
		printf("Wrong order of input, dealt with either way\n");
		temp = year1;
		year1 = year2;
		year2 = temp;
	}

	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";
	fp = fopen(fname, "r");
	if (!fp){
		perror("fopen");
		exit(3);
	}
	flock(fileno(fp), LOCK_SH);

	index = 1;
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	while ((fread(&comp, sizeof(CComp), 1, fp)) > 0){
		if (comp.year >= year1 && comp.year <= year2){
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
