#include <stdio.h>

int main(void)
{
	int x = 87;
	int y = 0;

	printf("-------------before--------------\n");
	printf("address of x = %p, address of y = %p\n", &x, &y);
	printf("value of x = %d, value of y = %d\n", x, y);

	//y = x;
	asm volatile("mov %0, %1"
		: "=r" (y)
		: "r" (x)
		: "memory"
		);

	printf("-------------after--------------\n");
	printf("address of x = %p, address of y = %p\n", &x, &y);
	printf("value of x = %d, value of y = %d\n", x, y);
	
	return 0;
}
