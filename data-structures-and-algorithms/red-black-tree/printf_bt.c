// Program to print binary tree in 2D
#include <stdio.h>
#include <malloc.h>

#include "printf_bt.h"

#define COUNT 10

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(const struct rbtree_node *root, int space)
{
	// Base case
	if (root == NULL)
		return;

	// Increase distance between levels
	space += COUNT;

	// Process right child first
	print2DUtil(root->right, space);

	// Print current node after space
	// count
	printf("\n");
	for (int i = COUNT; i < space; i++)
		printf(" ");
	printf("%d\n", root->value);

	// Process left child
	print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(const struct rbtree_node *root)
{
	// Pass initial space count as 0
	print2DUtil(root, 0);
}
