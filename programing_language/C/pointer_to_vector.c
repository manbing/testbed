#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static void test(int size, int arg [][size])
{
	int i = 0, j = 0;
	int result = 100;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 2; j++) {
			arg[i][j] = result++;
		}
	}
}

int main(void)
{
	int *input = (int *)calloc(10, sizeof(int));
	int i = 0;
	//int(*input2)[2] = (int (*)[2])input;

	//test((int (*)[2])input);
	test(2, (int (*)[100])input);

	for (i = 0; i < 10; i++) {
		printf("%d, %d\n", i, *(input + i));
	}

	return 0;
}
