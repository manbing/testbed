#include <stdio.h>

int main(void){
	char test <:8:> = "12345";
	int i = 0;
#pragma GCC poison printf sprintf fprintf

	for(i = 0; i < 5; i++){
		printf("test <:%d:> = %c\n", i, test<:i:>);
	}

	return 0;
}
