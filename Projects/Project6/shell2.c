#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

enum {
	BUF_SIZE = 256,
	ARG_SIZE = 16,
	CMD_SIZE = 8,
};

typedef struct Command Command;

struct Command {
	char cmd[CMD_SIZE * ARG_SIZE];
	int bg, output, indices[ARG_SIZE];
	char *infile, *outfile;
};

int main(){
	char line[BUF_SIZE], *args[ARG_SIZE];
	char *cleanargs[CMD_SIZE], *pipedcmds[CMD_SIZE];
	int status, chpid, bgpid, i, j, k, numcommands;
	Command command, commands[CMD_SIZE];
	int infd, outfd;
	int pfd1[2], pfd2[2], pipecount, index1;

	while(1){
		printf(">>> ");

		if (fgets(line, BUF_SIZE, stdin) == NULL){
			printf("\n");
			break;
		}

		if (line[0] == '\n'){
			continue;
		}

		if (line[strlen(line) - 1] == '\n'){
			line[strlen(line) - 1] = '\0';
		}

		j = numcommands = 0;	
		for (i = 0; i < strlen(line); i++){
			if (strchr(line + j, ';') || strchr(line + j, '&')){
				if (line[i] == ';'){
					command.bg = 0;
					strncpy(command.cmd, line + j, i - j);
					command.cmd[i - j] = '\0';
					commands[numcommands] = command;
					numcommands++;
					j = i + 1;
				} else if (line[i] == '&'){
					command.bg = 1;
					strncpy(command.cmd, line + j, i - j);
					command.cmd[i - j] = '\0';
					commands[numcommands] = command;
					numcommands++;
					j = i + 1;
				}
			} else {
				command.bg = 0;
				strncpy(command.cmd, line + j, strlen(line) - j);
				command.cmd[strlen(line) - j] = '\0';
				commands[numcommands] = command;
				numcommands++;
				break;
			}
		}

		for (i = 0; i < numcommands; i++){
			args[0] = strtok(commands[i].cmd, " \t\n");
			for (j = 1; j < ARG_SIZE; j++){
				args[j] = strtok(NULL, " \t\n");
				if (args[j] == NULL){
					break;
				}
			}

			pipecount = commands[i].indices[0] = 0;
			for (index1 = 0; index1 < j; index1++){
				if (args[index1] == NULL){
					break;
				}
				if (strcmp(args[index1], "<") == 0){
					commands[i].infile = args[index1 + 1];
					args[index1] = NULL;
				} else if (strcmp(args[index1], ">>") == 0){
					commands[i].outfile = args[index1 + 1];
					commands[i].output = 2;
					args[index1] = NULL;
				} else if (strcmp(args[index1], ">") == 0){
					commands[i].outfile = args[index1 + 1];
					commands[i].output = 1;
					args[index1] = NULL;
				} else if (strcmp(args[index1], "|") == 0){
					args[index1] = NULL;
					commands[i].indices[pipecount + 1] = index1 + 1;
					pipecount++;
				} else {
					commands[i].indices[index1] = index1;
				}
			}

			for (k = 0; k <= pipecount; k++){
				if (strcmp(args[commands[i].indices[k]], "exit") == 0){
					return(0);
				}
				if (strcmp(args[0], "cd") == 0){
					if (args[2]){
						fprintf(stderr, "Too Many Arguments.\n");
						continue;
					}
					if (!args[1] || strcmp(args[1], "~") == 0){
						chdir(getenv("HOME")); // looked this up
					} else {
						chdir(args[1]);
					}
				} else {
					if (pipecount && k != pipecount){
						pipe(pfd2);
					}
					chpid = fork();
					if (chpid < 0){
						perror("fork");
						return(1);
					}
					if (chpid == 0){
						// redirection
						if (k == 0 && commands[i].infile){ 
							infd = open(commands[i].infile, O_RDONLY);
							dup2(infd, 0);
							close(infd);
						}
						if (k == pipecount && commands[i].outfile){
							if (commands[i].output == 1){
								outfd = open(commands[i].outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
								dup2(outfd, 1);
								close(outfd);
							} else if (commands[i].output == 2){
								outfd = open(commands[i].outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
								dup2(outfd, 1);
								close(outfd);
							}
						}
						if (pipecount){
							if (k == 0){
								dup2(pfd2[1], 1);
								close(pfd2[0]);
								close(pfd2[1]);
							} else if (k < pipecount){
								dup2(pfd1[0], 0);
								close(pfd1[0]);
								close(pfd1[1]);
								dup2(pfd2[1], 1);
								close(pfd2[0]);
								close(pfd2[1]);
							} else {
								dup2(pfd1[0], 0);
								close(pfd1[0]);
								close(pfd1[1]);
							}
						}
						execvp(args[commands[i].indices[k]], &args[commands[i].indices[k]]);
						perror("execvp");
						return(2);
					} else {
						if (k > 0){
							close(pfd1[0]);
							close(pfd1[1]);
						}
						pfd1[0] = pfd2[0];
						pfd1[1] = pfd2[1];
						if (!commands[i].bg){
							wait(&status);
						} else {
							printf("Running %s in the background.\n", args[commands[i].indices[k]]);
							
							bgpid = waitpid(chpid, &status, WNOHANG);
							printf("PID = %d\n", bgpid);
							if (bgpid == chpid){
								printf("Ran %s successfully.\n", args[commands[i].indices[k]]);
								continue;
							}
						}
					}
				}
			}
			if (pipecount){
				close(pfd1[0]);
				close(pfd1[1]);
			}
		}
		for (i = 0; i < j; i++){
			args[i] = NULL;
		}
	}
	return(0);
}

