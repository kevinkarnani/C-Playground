/*
   This assignment is to help you learn how to debug
   compiler warnings/errors and other common errors
   in your code. For each part labeled P(n), there is
   a warning/error/problem that goes with it. Write
   down what the issue was in the `Error:` section of
   each problem. Submit `segfault.c` along with your
   fixes and error comments.
 */

// P0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Error: There are string functions being applied that require string.h
 * Solution: Include string.h
 */

void fib(int* A, int n);

int
main(int argc, char *argv[]) {
	int buf[10];
	unsigned int i;
	char *str;
	char *printThisOne;
	char *word;
	int *integers;
	int foo;
	int *bar;
	char *someText;
	long x;

	// P1
	for (i = 0; i < 10; ++i) {
		buf[i] = i;
	}
	for (i = 0; i < 10; ++i) {
		printf("Index %d = %d\n", i, buf[i]);
	}
	
	/*
	 * Error: We allocated 10 slots for the array, indexed from 0 to 9. However, the for loops above go from indices 0 to 10.
	 			 Also, in the printf, we are using '%s' for the iterator variable and array index, both of which are of type int.
	 * Solution: Make i < 10 or i <= 9 in the "stop condition" part of the for loop. Also, change each '%s' to '%d'.
	 */

	// P2
	printThisOne = malloc(sizeof(char) * 18);
	str = "Something is wrong";
	strcpy(printThisOne, str);
	printf("%s\n", printThisOne);
	
	/* 
	 * Error: We allocated 10 characters worth of memory, giving us a max length of 10. However, the string provided has more than 10 characters.
	 * Solution: Allocate more space, set to correct variable, and print the correct variable.
	 */

	// P3
	word = malloc(sizeof(char) * 6);
	strcpy(word, "Part 3");
	*(word + 4) = '-';
	printf("%s\n", word);
	
	/* 
	 * Error: We are trying to modify a string that has been declared with pointer notation.
	 * Solution: Use malloc beforehand.
	 */

	// P4
	integers = malloc(sizeof(int) * 11);
	*(integers + 10) = 10;
	printf("Part 4: %d\n", *(integers + 10));
	free(integers);

	/* 
	 * Error: Since no memory was allocated, we are trying to go 10 memory spaces ahead, which is restricted.
	 * Solution: Use malloc beforehand.
	 */

	// P5
	printf("Print this whole line\n");
	
	/* 
	 * Error: By definition, '\0' is how strings end in C. Since '\0' is in the printf, it ends the string early.
	 * Solution: Remove the '\0'.
	 */

	// P6
	x = 2147483647;
	printf("%ld is positive\n", x); 
	x += 1000000000;
	printf("%ld is positive\n", x); 
	
	/* 
	 * Error: x is undeclared. With the addition, x is no longer 8 bytes, so printing as an int will make it negative. 
	 * Solution: Declare x as type long and change '%d' to '%ld'.
	 */

	// P7
	printf("Cleaning up memory from previous parts\n");
	free(printThisOne);
	free(word);

	/* 
	 * Error: We didn't use malloc with buf, instead we declared it as an array. So, buf is not a heap object.
	 * Solution: Free a variable only if you have allocated memory for it.
	 */

	// P8
	fib(&foo, 7);
	printf("fib(7) = %d\n", foo);
	
	/* 
	 * Error: foo is of type int, and the first parameter of the fib function requires and int array or a pointer to an int.
	 * Solution: Pass in the address to foo.
	 */

	// P9
	bar = malloc(sizeof(int));
	*bar = 123;
	printf("bar = %d\n", *bar);
	free(bar);

	/*
	 * Error: Setting bar to 0 instead of allocating space for it. You cannot use space you do not own/allocate.
	 * Solution: Allocate enough space to hold an int.
	 */

	// P10
	someText = malloc(10);
	strcpy(someText, "testing");
	printf("someText = %s\n", someText);
	free(someText);
	
	/* 
	 * Error: Freed the variable before the printf, so all data stored in there was cleared.
	 * Solution: Free the variable after all operations are done.
	 */
	exit(0);
}

// fib calculates the nth fibonacci number and puts it in A.
// There is nothing wrong with this function.
void fib(int *A, int n)
{
	int temp;
	if (n == 0 || n == 1)
		*A = 1;
	else {
		fib(A, n - 1);
		temp = *A;
		fib(A, n - 2);
		*A += temp;
	}
}
