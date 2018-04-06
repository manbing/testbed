#include <assert.h>
#include <stdio.h>

int main(void)
{
	int i = 0;

	for (i = 0; i < 10; i++) {
		assert(i < 5); // contiguous condition
		printf("i = %d\n", i);
	}

	return 0;
}
