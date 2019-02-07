#include <stdio.h>

int main(void)
{
        int a = 16;

        printf("%%#x = %#x\n", a);
        printf("%%+d = %+d\n", a);
        printf("%%#o = %#o\n", a);
        printf("%%08x = %08x\n", a);
        printf("%%-x = %-x\n", a);

        return 0;
}
