#include <stdlib.h>
#include <stdio.h>

//static int content [] = {5, 89, 46, 72, 37, 93, 59, 34, 22, 16};
static int content [] = {99, 88, 77, 66, 55, 44, 33, 22, 11, 0};

void swap(int *a, int *b)
{
	int tmp;
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void quick_sort(int start, int end)
{
	int i = start - 1, j = 0, pivot = 0;

	pivot = content[end];

	for (j = start; j < end; j++) {
		if (content[j] < pivot) {
			i++;
			swap(&content[i], &content[j]);
		}
	}

	swap(&content[i + 1], &content[end]);

	if (start < i)
		quick_sort(start, i);

	if ((i + 2) < end)
		quick_sort(i + 2, end);
}

int main(void)
{
	int array_len, i;
	
	array_len = sizeof(content)/sizeof(typeof(*content));

	quick_sort(0, array_len - 1);
	
	for (i = 0 ; i < array_len; i++) {
		printf("content[%d] = %d\n", i, content[i]);
	}

	return 0;
}
