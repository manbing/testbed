#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	int content [] = {5, 89, 46, 72, 37, 93, 59, 34, 22, 16};
	int swap_flag = 1, i = 0, order_flag = 0, j = 0, tmp;
	int array_len = sizeof(content)/sizeof(typeof(*content));

	for (i = 0; i < array_len && swap_flag; i++) {
		order_flag = array_len - i - 1;
		swap_flag = 0;

		for (j = 0; j < order_flag; j++) {
			if (content[j] > content[j+1]) {
				swap_flag = 1;
				tmp = content[j];
				content[j] = content[j+1];
				content[j+1] = tmp;
			}
		}
	}

	for (i = 0 ; i < array_len; i++) {
		printf("content[%d] = %d\n", i, content[i]);
	}

	return 0;
}
