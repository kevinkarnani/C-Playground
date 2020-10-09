#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "cc.h"

void fgetstr(char *, int, FILE *);

int main(int argc, char *argv[]){
	CComp target;
	char *fname, temp[Ndesc];
	int index;
	FILE *fp;

	if (argc != 2){
		fprintf(stderr, "Usage: ccedit id\n");
		exit(1);
	}

	index = atoi(argv[1]);

	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";
	fp = fopen(fname, "r+");
	if (!fp){
		perror("fopen");
		exit(2);
	}
	flock(fileno(fp), LOCK_EX);
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	fread(&target, sizeof(CComp), 1, fp);
	fseek(fp, index * sizeof(CComp), SEEK_SET);
	if (index != target.id){
		fprintf(stderr, "No such item\n");
		exit(3);
	}

	printf("\nMaker: %s\n", target.maker);
  	printf("New Maker: ");
	fgetstr(temp, Nmaker, stdin);
	strncpy(target.maker, temp, Nmaker);
	printf("Model: %s\n", target.model);
  	printf("New Model: ");
	fgetstr(temp, Nmodel, stdin); 
  	strncpy(target.model, temp, Nmodel);
	printf("Year: %d\n", target.year);
  	printf("New Year: ");
	fgetstr(temp, sizeof(temp), stdin);	
	target.year = strtol(temp, NULL, 10);
	printf("CPU: %s\n", target.cpu);
	printf("New CPU: ");
	fgetstr(temp, Ncpu, stdin);
	strncpy(target.cpu, temp, Ncpu);
   printf("Desc: %s\n", target.desc);
  	printf("New Desc: ");
	fgetstr(temp, Ndesc, stdin);
	strncpy(target.desc, temp, Ndesc);
	printf("----------------------\n");

	fwrite(&target, sizeof(CComp), 1, fp);
	flock(fileno(fp), LOCK_UN);
	fclose(fp);
	exit(0);
}

void fgetstr(char *s, int n, FILE *stream){
	fgets(s, n, stream);
	if (strlen(s) > 0 && s[strlen(s) - 1] == '\n'){
		s[strlen(s) - 1] = '\0';
	}
}
