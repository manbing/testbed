#include <stdio.h>

void change_parameter_by_pointer(int *a)
{
	*a = *a+1;
}

void change_parameter_by_instance(int a)
{
	a = a+1;
}

int main(void)
{
	int a = 1, tmp = 0;

	tmp = a;
	change_parameter_by_pointer(&a);
	printf("change_parameter_by_pointer() %s \n", (a == tmp)? "change value failure":"change value successfully");

	tmp = a;
	change_parameter_by_instance(a);
	printf("change_parameter_by_instance() %s \n", (a == tmp)? "change value failure":"change value successfully");

	return 0;
}
