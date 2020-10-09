#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "cc.h"

int main(int argc, char *argv[]){
	CComp target;
	FILE *fp;
	char *fname;
	int index;

	if (argc != 2){
		fprintf(stderr, "Usage: ccdel id\n");
		exit(1);
	}

	index = atoi(argv[1]);
	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";
	fp = fopen(fname, "r+");
	if (!fp){
		perror("fopen\n");
		exit(2);
	}

	flock(fileno(fp), LOCK_EX);
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	fread(&target, sizeof(CComp), 1, fp);
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	if (index != target.id){
		fprintf(stderr, "No such item\n");
		exit(2);
	}

	memset(&target, 0, sizeof(CComp));
	fwrite(&target, sizeof(CComp), 1, fp);
	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	exit(0);
}
