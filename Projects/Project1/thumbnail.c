/*
 * Author: Kevin Karnani
 * Class: CS 283 (Brian Stuart)
 * Date: July 12, 2020
 * Purpose: Uses the C Language to downscale STDIN (assuming it is in PGM format) into a thumbnail.
 * The thumbnail is then written to STDOUT.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int width, height, max, newWidth, newHeight;
	int scale, i, j, shift, newShift;
	unsigned char *buf, *output, 

	n = scanf("P5 %d %d %d", &width, &height, &max);
	if (n != 3){
		fprintf(stderr, "Invalid Input");
		exit(1);
	}

	buf = malloc(width * height * sizeof(char));
	
	if ((width / 200) > 0){
		scale = width / 200;
	} else {
		fprintf(stderr, "Image too small");
		exit(2);
	}
	newWidth = width / scale;
	newHeight = height / scale;
	
	output = malloc(newWidth * newHeight * sizeof(char));
	printf("P5\n%d %d\n%d\n", newWidth, newHeight, max);
	
	getchar(); // need to get rid of additional space
	fread(buf, sizeof(char), width * height, stdin);
	if (!*buf){
		fprintf(stderr, "fread error: %d words", n);
		exit(3);
	}

	for (i = 0; i < newHeight; i++){
		for (j = 0; j < newWidth; j++){
			shift = (i * width) + j;
			newShift = (i * newWidth) + j;
			*(output + newShift) = *(buf + (shift * scale));
		}
	}

	n = fwrite(output, sizeof(char), newWidth * newHeight, stdout);
	if (n == 0 || n == -1){
		fprintf(stderr, "fwrite error");
		exit(4);
	}
	exit(0);
}
