#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>

int main(){
	struct stat ccstat;
	char *fname;
	int n;

	fname = malloc(sizeof(char) * 9);
	fname = "ccomp.db";

	n = stat(fname, &ccstat);
	if (n < 0){
		perror("stat");
		exit(1);
	}

	printf("Device: %d %d\n", major(ccstat.st_dev), minor(ccstat.st_dev));
	printf("I-Number: %lu\n", ccstat.st_ino);
	printf("Mode: %o\n", ccstat.st_mode);
	printf("Number of Links: %ld\n", ccstat.st_nlink);
	printf("User ID: %d\n", ccstat.st_uid);
	printf("Group ID: %d\n", ccstat.st_gid);
	printf("Size: %ld\n", ccstat.st_size);
	
	exit(0);
}
