#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "cc.h"

int main(int argc, char *argv[]){
	CComp target;
	FILE *fp;
	char *fname;
	int index;

	if (argc != 2){
		fprintf(stderr, "Usage: ccitem id\n");
		exit(1);
	}

	index = atoi(argv[1]);
	
	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";
	fp = fopen(fname, "r");
	if (!fp){
		perror("fopen");
		exit(2);
	}
	flock(fileno(fp), LOCK_SH);
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	fread(&target, sizeof(CComp), 1, fp);
	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	if (index != target.id){
		fprintf(stderr, "No such item\n");
		exit(3);
	}

	printf("Maker: %s\n", target.maker);
	printf("Model: %s\n", target.model);
	printf("Year: %d\n", target.year);
	printf("CPU: %s\n", target.cpu);
	printf("ID: %d\n", target.id);
	printf("Desc: %s\n", target.desc);
	exit(0);
}
