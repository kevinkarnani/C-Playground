#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "cc.h"

int main(){
	CComp target;
	FILE *fp;
	char *fname;
	int index;

	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";
	fp = fopen(fname, "r+");
	flock(fileno(fp), LOCK_SH);

	printf("Item Number: ");
	scanf("%d", &index);	

	fseek(fp, index * sizeof(CComp), SEEK_SET);
	fread(&target, sizeof(CComp), 1, fp);
	flock(fileno(fp), LOCK_UN);
	fclose(fp);

	if (index != target.id){
		fprintf(stderr, "No such item\n");
		exit(2);
	}

	printf("Maker: %s\n", target.maker);
	printf("Model: %s\n", target.model);
	printf("Year: %d\n", target.year);
	printf("CPU: %s\n", target.cpu);
	printf("ID: %d\n", target.id);
	printf("Desc: %s\n", target.desc);
	exit(0);
}
