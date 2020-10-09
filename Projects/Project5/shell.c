#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

enum {
	BUF_SIZE = 256,
	ARG_SIZE = 16,
};

int main(){
	char line[BUF_SIZE], *args[ARG_SIZE];
	int status, chpid, i, bg, bgpid;

	
	while(1){
		printf(">>> ");

		if (fgets(line, BUF_SIZE, stdin) == NULL){
			printf("\n");
			break;
		}

		if (line[0] == '\n'){
			continue;
		}

		bg = (line[strlen(line) - 2] == '&') ? 1 : 0;
		line[strlen(line) - 2] = bg ? '\0' : line[strlen(line) - 2];
		
		if (line[strlen(line) - 1] == '\n'){
			line[strlen(line) - 1] = '\0';
		}

		args[0] = strtok(line, " \t\n");
		for (i = 1; i < ARG_SIZE; i++){
			args[i] = strtok(NULL, " \t\n");
			if (args[i] == NULL){
				break;
			}
		}

		if (strcmp(args[0], "cd") == 0){
			if (args[2]){
				fprintf(stderr, "Too Many Arguments\n");
				continue;
			}
			if (!args[1] || strcmp(args[1], "~") == 0){
				chdir(getenv("HOME")); // had to look up how to get home directory
			} else {
				chdir(args[1]);
			}
		} else if (strcmp(args[0], "exit") == 0) {
			return(0);
		} else {
			chpid = fork();
			if (chpid < 0){
				perror("fork");
				return(1);
			}
			if (chpid == 0){
				execvp(args[0], args);
				perror("execvp");
				return(2);
			}
			if (!bg){
				wait(&status);
			} else {
				printf("Running %s in the background\n", args[0]);
				
				bgpid = waitpid(chpid, &status, WNOHANG);
				printf("PID = %d\n", bgpid);
				if (bgpid == chpid){
					printf("Ran %s successfully.\n", args[0]);
					continue;
				}
			}
		}
	}
	return(0);
}
