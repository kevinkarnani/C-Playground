#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include "cc.h"

void print();
int fgetint(char*, int, FILE*);
void cases(int);

int main(){
	int choice;
	char input[sizeof(int)];
	
	print();
	choice = fgetint(input, sizeof(input), stdin);
	cases(choice);
	return(0);
}

void print(){
	printf("1. ccadd\n");
	printf("2. ccitem\n");
	printf("3. cclist\n");
	printf("4. ccdel\n");
	printf("5. ccmatch\n");
	printf("6. ccyear\n");
	printf("7. ccedit\n");
	printf("8. Quit\n");
	printf("Enter Choice: ");
}

int fgetint(char *s, int n, FILE *stream){
	fgets(s, n, stream);
	if (!isdigit(s[0]) || strlen(s) != 2){
		fprintf(stderr, "Invalid Input. Exiting Now.\n");
		exit(1);
	}

	if (strlen(s) > 1 && s[1] != '\n'){
		s[1] = '\0';
	}

	return strtol(s, NULL, 10);
}

void fgetstr(char *s, int n, FILE *stream){
   fgets(s, n, stream);
   if (strlen(s) > 0 && s[strlen(s) - 1] == '\n'){
      s[strlen(s) - 1] = '\0';
   }
}

void ccadd(){
	char input[Ndesc];
	char *args[8] = {"./ccadd"};
	
	printf("Usage: ccadd (id | -a) maker(16) model(32) year cpu(8) desc(192)\n");

	args[1] = malloc(10);
	printf("ID or \"-a\": ");
	fgetstr(input, 10, stdin);
	strncpy(args[1], input, 10);

	args[2] = malloc(Nmaker);
	printf("Maker: ");
	fgetstr(input, Nmaker, stdin);
	strncpy(args[2], input, Nmaker);
	
	args[3] = malloc(Nmodel);
	printf("Model: ");
	fgetstr(input, Nmodel, stdin);
	strncpy(args[3], input, Nmodel);

	args[4] = malloc(10);
	printf("Year: ");
	fgetstr(input, 10, stdin);
	strncpy(args[4], input, 10);

	args[5] = malloc(Ncpu);
	printf("CPU: ");
	fgetstr(input, Ncpu, stdin);
	strncpy(args[5], input, Ncpu);

	args[6] = malloc(Ndesc);
	printf("Desc: ");
	fgetstr(input, Ndesc, stdin);
	strncpy(args[6], input, Ndesc);
		
	args[7] = NULL;

	execvp(args[0], args);
	perror("execvp");
	exit(3);
}

void ccitem(){
	char input[10];
	char *args[3] = {"./ccitem"};

	printf("Usage: ccitem id\n");

	args[1] = malloc(10);
	printf("ID: ");
	fgetstr(input, 10, stdin);
	strncpy(args[1], input, 10);

	args[2] = NULL;

	execvp(args[0], args);
	perror("execvp");
	exit(3);
}

void cclist(){
	char *args[2] = {"./cclist", NULL};

	execvp(args[0], args);
	perror("execvp");
	exit(3);
}


void ccdel(){
	char input[10];
	char *args[3] = {"./ccdel"};

	printf("Usage: ccdel id\n");

	args[1] = malloc(10);
	printf("ID: ");
	fgetstr(input, 10, stdin);
	strncpy(args[1], input, 10);

	args[2] = NULL;

	execvp(args[0], args);
	perror("execvp");
	exit(3);
}

void ccmatch(){
	char input[Ndesc];
	char *args[3] = {"./ccmatch"};

	printf("Usage: ccmatch description\n");

	args[1] = malloc(Ndesc);
	printf("Desc: ");
	fgetstr(input, Ndesc, stdin);
	strncpy(args[1], input, Ndesc);

	args[2] = NULL;

	execvp(args[0], args);
	perror("execvp");
	exit(3);
}

void ccyear(){
	char input[10];
	char *args[4] = {"./ccyear"};

	printf("Usage: ccyear year1 year2\n");

	args[1] = malloc(10);
	printf("Start: ");
	fgetstr(input, 10, stdin);
	strncpy(args[1], input, 10);
	
	args[2] = malloc(10);
	printf("End: ");
	fgetstr(input, 10, stdin);
	strncpy(args[2], input, 10);

	args[3] = NULL;

	execvp(args[0], args);
	perror("execvp");
	exit(3);
}

void ccedit(){
	char input[10];
	char *args[3] = {"./ccedit"};

	printf("Usage: ccdedit id\n");

	args[1] = malloc(10);
	printf("ID: ");
	fgetstr(input, 10, stdin);
	strncpy(args[1], input, 10);

	args[2] = NULL;

	execvp(args[0], args);
	perror("execvp");
	exit(3);
}

void cases(int choice){
	int chpid, status;

	chpid = fork();
	if (chpid < 0){
		perror("fork");
		exit(2);
	}
	if (chpid == 0){
		switch (choice){
			case 1:
				ccadd();
				break;
			case 2:
				ccitem();
				break;
			case 3:
				cclist();
				break;
			case 4:
				ccdel();
				break;
			case 5:
				ccmatch();
				break;
			case 6:
				ccyear();
				break;
			case 7:
				ccedit();
				break;
			case 8:
				printf("You chose to quit, quitting now.\n");
				break;
			default:
				fprintf(stderr, "Invalid Input. Exiting Now.\n");
				exit(1);
		}
	}
	wait(&status);
}
