#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

int redirect;
int sfd1, sfd2;

void runchild(int, int);
void runparent(int, int, int);

int main(int argc, char *argv[]){
	int chpid;
	int pfd[2];

	if (argc != 2){
		fprintf(stderr, "Usage: chat n\n");
		exit(1);
	}

	pipe(pfd);
	chpid = fork();
	if (chpid < 0){
		perror("fork");
		exit(2);
	}
	if (!chpid){
		close(pfd[1]);
		runchild(atoi(argv[1]), pfd[0]);
	} else {
		close(pfd[0]);
		runparent(atoi(argv[1]), chpid, pfd[1]);
	}
	exit(0);
}

int runcmd(char *buf, int fdw, int fdr){
	int i, chpid;
	char *tokens[32];

	chpid = fork();
	if (chpid > 0){
		return chpid;
	} else {
		signal(SIGINT, SIG_DFL);
		tokens[0] = strtok(buf + 1, " \t\n");
		for (i = 1; i < 32; i++){
			tokens[i] = strtok(NULL, " \t\n");
			if (tokens[i] == NULL){
				break;
			}
		}
		if (fdw != -1){
			close(1);
			dup(fdw);
		}
		if (fdr != -1){
			close(0);
			dup(fdr);
		}
		execvp(tokens[0], tokens);
		perror("exec");
		exit(4);
	}
	return(0);
}


void runparent(int sign, int chpid, int pfd){
	int fdw, n, status;
	char buf[128];

	if (sign == 1){
		fdw = open("chatpipe1", O_WRONLY);
	} else {	
		fdw = open("chatpipe2", O_WRONLY);
	}
	
	if (fdw < 0){
		perror("parent open");
		exit(3);
	}

	signal(SIGINT, SIG_IGN);
	
	while((n = read(0, buf, 127)) > 0){
		buf[n] = '\0';
		switch(buf[0]){
			case '!':
				runcmd(buf, -1, -1);
				wait(&status);
				break;
			case '>':
				runcmd(buf, fdw, -1);
				wait(&status);
				break;
			case '<':
				if (buf[1] == '$'){
					kill(redirect, SIGINT);
				} else {
					kill(chpid, SIGUSR1);
					write(pfd, buf, n + 1);
				}
				break;
			default:
				write(fdw, buf, n);
				break;
		}
	}
	close(fdw);
}

void usr1handlr(int x){
	char buf[128];
	int status;

	read(sfd1, buf, 128);
	redirect = runcmd(buf, -1, sfd2);
	wait(&status);
}

void regchild(int fdr){
   int n;
   char buf[128];

   while((n = read(fdr, buf, 128)) > 0){
      write(1, ">>> ", 4);
		write(1, buf, n);
   }
}

void runchild(int sign, int pfd){
   int fdr, chpid, status;
	char buf[128];

	signal(SIGUSR1, usr1handlr);
	signal(SIGINT, SIG_IGN);

	if (sign == 1){
      fdr = open("chatpipe2", O_RDONLY);
   } else {
      fdr = open("chatpipe1", O_RDONLY);
   }

	sfd1 = pfd;
	sfd2 = fdr;

	if (fdr < 0){
		perror("child open");
		exit(3);
	}
	regchild(fdr);
   close(fdr);
}
