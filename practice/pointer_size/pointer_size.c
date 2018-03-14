#include <stdio.h>

/*
   Operation system version (32/64 bits) decision pointer type size.

   if operation system is 32 bits, the identifier size of pointer type is 32 bits, and vice versa.

*/


struct tmp
{
	void *ptr;
	char *ptr2;
	int *ptr3;
};

int main(void)
{
	void *ptr;
	char *ptr2;
	int *ptr3;

	printf("[%s][%d] = %lu\n", __func__, __LINE__, sizeof(((struct tmp*)0)->ptr));
	printf("[%s][%d] = %lu\n", __func__, __LINE__, sizeof(((struct tmp*)0)->ptr2));
	printf("[%s][%d] = %lu\n", __func__, __LINE__, sizeof(((struct tmp*)0)->ptr3));

	printf("[%s][%d] = %lu\n", __func__, __LINE__, sizeof(ptr));
	printf("[%s][%d] = %lu\n", __func__, __LINE__, sizeof(ptr2));
	printf("[%s][%d] = %lu\n", __func__, __LINE__, sizeof(ptr3));
	return 0;
}
