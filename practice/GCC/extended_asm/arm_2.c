#include <stdio.h>

int main(void)
{
    int var1 = 12;
    int var2 = 10;

    printf("[%s]\n", __FILE__);
    
    asm("mov %0, %1 \n  \
         add %1, %0, $1"
	 : "=r"(var1), "=r"(var2)
	 : "r"(var2), "r"(var1)
	 : );

    printf("var1 = %d, var2 = %d\n", var1, var2);

    asm("ldr r5, %0 \n":           : "m"(var1): "r5");
    asm("str r4, %0"   : "=m"(var2):          : "r4");
    return 0;
}
