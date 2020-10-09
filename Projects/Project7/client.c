#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

typedef struct Client Client;

struct Client {
	int sock;
	pthread_t tid;
	char name[16];
};

void *sendmessage(void *);

int main(int argc, char *argv[]){
	int sock;
	char buf[1024];
	struct sockaddr_in listener;
	struct hostent *host;
	pthread_t tid;
	Client *c;

	if (argc != 2){
		fprintf(stderr, "Usage: client name\n");
		exit(1);
	}
	host = gethostbyname("tux2.cs.drexel.edu");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	listener.sin_family = AF_INET;
	listener.sin_port = htons(2020);
	listener.sin_addr.s_addr = *((long *)host->h_addr_list[0]);
	if (connect(sock, (struct sockaddr *)&listener, sizeof(struct sockaddr_in)) < 0){
		perror("connect");
		exit(2);
	}
	c = malloc(sizeof(Client));
	strncpy(c->name, argv[1], 15);
	c->name[15] = '\0';
	c->sock = sock;
	pthread_create(&tid, NULL, sendmessage, c);
	
	while(1){
		if (recv(sock, buf, 1024, 0) < 0){
			pthread_exit(NULL);
		}
		fwrite(buf, sizeof(char), strlen(buf) + 1, stdout);
		memset(buf, 0, strlen(buf));
	}
	close(sock);
	pthread_exit(NULL);
	return(0);
}

void *sendmessage(void *p){
	Client *c;
	char buf[1024];
	int prefix;

	c = p;
	c->tid = pthread_self();
	send(c->sock, c->name, 16, 0);
	prefix = sprintf(buf, "%s>", c->name);
	
	while(1){
		if (fgets(buf, 1024 - prefix, stdin) == NULL){
			if (buf[0] == '\n'){
				continue;
			} else {
				sprintf(buf, "%s has left the chat.\n", c->name);
				send(c->sock, buf, strlen(buf), 0);
				exit(0);
			}
		}
		buf[strlen(buf)] = '\0';
		if (send(c->sock, buf, strlen(buf) + 1, 0) < 0){
			pthread_exit(NULL);
		}
	}
}
