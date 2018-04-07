#include <iso646.h>
#include <stdio.h>

int main(void)
{
	int tmp = 87;

	if (1 and 1)
		printf("and ok\n");
	
	if (1 or 0)
		printf("or ok\n");

	tmp and_eq 1;
	printf("tmp = %d\n", tmp);
	
	tmp = 87;
	tmp and_eq (compl 1);
	printf("tmp = %d\n", tmp);


	return 0;
}
