#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int numlines(char *, int);
void setlines(char *[], char *, int);

int main(int argc, char *argv[]){
	int cap, used, n, i;
	char *buf, *p, **lines;

	used = 0;
	cap = 128;

	buf = malloc(cap);
	p = buf;
	while((n = read(0, p, cap - used)) > 0){
		used += n;
		p += n;
		if (cap - used < 32){
			cap *= 2;
			buf = realloc(buf, cap);
			p = buf + used;
		}
	}
	
	n = numlines(buf, used);
	lines = malloc(n * sizeof(char *));
	setlines(lines, buf, used);
	
	if (lines[n - 1] != buf + used - 1){
		write(1, lines[n - 1] + 1, buf + used - lines[n - 1]);
	}

	for (i = n - 1; i > 0; i--){
		write(1, lines[i - 1] + 1, lines[i] - lines[i - 1]);
	}

	write(1, buf, lines[0] - buf + 1);
}

int numlines(char *buf, int nchar){
	int i, nline;
	char *p;

	p = buf;
	nline = 0;
	for (i = 0; i < nchar; i++){
		if (*p == '\n'){
			nline++;
		}
		p++;
	}
	return nline;
}


void setlines(char *lines[], char *buf, int nchar){
	int i, n;
	char *p;

	p = buf;
	n = 0;
	for (i = 0; i < nchar; i++){
		if (*p == '\n'){
			lines[n++] = p;
		}
		p++;
	}
}
