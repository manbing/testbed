#include <stdio.h>

int show_message(int data)
{
	printf("[%s][%d] data = %d\n", __func__, __LINE__, data);
}

int main(void)
{
	int (*fp)(int);

	fp = show_message;

	fp(1);
	(*fp)(3);

	return 0;
}
