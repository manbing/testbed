#include <stdio.h>

int main(void)
{
    int var1 = 12;
    int var2 = 10;

    printf("[%s]\n", __FILE__);

    asm("mov %[my_var1], %[my_var2] \n  \
         add %[my_var3], %[my_var4], $1"
	 : [my_var1] "=r" (var1), [my_var3] "=r" (var2)
	 : [my_var2] "r"  (var2), [my_var4] "r"  (var1)
	 : );
    
    printf("var1 = %d, var2 = %d\n", var1, var2);

    asm("ldr r5, %[my_var1] \n":: [my_var1] "m"(var1): "r5");
    asm("str r4, %[my_var1]": [my_var1] "=m" (var2):: "r4");
    return 0;
}
