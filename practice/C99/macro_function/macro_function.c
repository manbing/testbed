#include <stdio.h>

#define printf(format, ...) printf("(%s:%d) "format, __func__, __LINE__, ##__VA_ARGS__)

int main(void)
{
	printf("macro_function\n");
	return 0;
}
