#include <stdio.h>
#include <string.h>

#ifndef __uint32_t_defined
typedef unsigned int            uint32_t;
#endif

__attribute__((always_inline)) static inline uint32_t __get_register(void)
{
	unsigned int pc = 0;
	unsigned int xpsr = 0;
	unsigned int i = 1;

	//__asm volatile("adds %2, #1 \n
	__asm volatile("sub %2, #-1 \n \
			mrs %0, cpsr \n \
			mov %1, pc" :
			"=r" (xpsr), "=r" (pc) :
			"r" (i):
			"memory"
		      );

	printf("value of pc = %x, value of xPSR = %x\n", pc, xpsr);
}

int main(void)
{
	int a = 10;
	
	__get_register();

	a = a - 100;
	
	__get_register();

	return 0;
}
