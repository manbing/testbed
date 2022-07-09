#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "printf_bt.h"

#define TEXT_COLOR_RED "\033[0;31m"

char *menu [] = {
	[INSERT] = "Insert",
	[DELETE] = "Delete",
	[TRAVERSE] = "Traverse",
	[SELF_TEST] = "Self test",
	[EXIT] = "Exit",
	[MAX] = NULL,
};

static struct rbtree_ops {
	struct rbtree_node *	(*new)(int value);
	bool			(*insert)(struct rbtree_node **root, int value);
	bool			(*delete)(struct rbtree_node **root, int value);
	void			(*traverse)(const struct rbtree_node *root);
	struct rbtree_node *	(*rotate)(struct rbtree_node *root);
	void			(*free)(struct rbtree_node *root);
} rbtree_ops;

static struct rbtree_node *__new(int value)
{
	struct rbtree_node *retval = NULL;
	retval = (struct rbtree_node *) malloc(1 * sizeof(struct rbtree_node));
	if (retval) {
		memset(retval, 0, sizeof(struct rbtree_node));
		retval->value = value;
	}

	return retval;
}

static bool __insert(struct rbtree_node **root, int value)
{
	bool retval = false; 

	if (!(*root)) {
		*root = rbtree_ops.new(value);
		return true;
	}

	if (value > (*root)->value) {
		retval = __insert(&(*root)->right, value);
	} else if (value < (*root)->value) {
		retval = __insert(&(*root)->left, value);
	} else { //root->value == new_value
		retval = false;
	}

	return retval;
}

static bool __delete(struct rbtree_node **root, int value)
{
	bool retval = false;

	return retval;
}

#if 0
static void __traverse(const struct rbtree_node * const *root)
{
	const struct rbtree_node *curr = *root;

	if (!curr)
		return;

	printf("%d  -->  ", curr->value);

	if (curr->left) {
		__traverse((const struct rbtree_node * const *) &(curr->left));
	}

	if (curr->right) {
		 __traverse((const struct rbtree_node * const *) &(curr->right));
	}
}
#endif

static struct rbtree_node *__rotate(struct rbtree_node *root)
{
	struct rbtree_node *retval = NULL;

	return retval;
}

static void __free(struct rbtree_node *root)
{
	if (!root)
		return;

	if (!root->left) {
		__free(root->left);
	}

	if (!root->right) {
		__free(root->right);
	}

	free(root);
}

static void test_mode(struct rbtree_node **root)
{
	int i = 0;
	int input [] = {513, 8, 524, 79, 514, 709, 43, 627, 1000};
	int size = sizeof(input)/sizeof(input[0]);

	for (i = 0; i < size; i++) {
		rbtree_ops.insert(root, input[i]);
		//printf("[%s:%d] start\n", __func__, __LINE__);
		//rbtree_ops.traverse((const struct rbtree_node * const *) root);
		//printf("\n\n\n[%s:%d] end\n", __func__, __LINE__);
	}
}

int main(void)
{
	struct rbtree_node *root = NULL;
	int input = 0, data = 0;

	rbtree_ops.new = __new;
	rbtree_ops.insert = __insert;
	rbtree_ops.delete = __delete;
	rbtree_ops.traverse = print2D;
	rbtree_ops.rotate = __rotate;
	rbtree_ops.free = __free;

	while (1) {
		printf("\n\n\n");
		printf("%d, %s\n", INSERT, menu[INSERT]);
		printf("%d, %s\n", DELETE, menu[DELETE]);
		printf("%d, %s\n", TRAVERSE, menu[TRAVERSE]);
		printf("%d, %s\n", SELF_TEST, menu[SELF_TEST]);
		printf("%d, %s\n", EXIT, menu[EXIT]);
		printf("\nEnter your choice:");
		scanf("%d", &input);

		switch (input) {
			case INSERT:
				printf("Enter the element to insert:");
				scanf("%d", &data);
				rbtree_ops.insert(&root, data);
				break;

			case DELETE:
				printf("Enter the element to insert:");
				scanf("%d", &data);
				rbtree_ops.delete(&root, data);
				break;

			case TRAVERSE:
				rbtree_ops.traverse((const struct rbtree_node *) root);
				printf("\n\n\n");
				break;

			case SELF_TEST:
				test_mode(&root);
				break;

			case EXIT:
				goto out_free;
				break;

			default:
				break;
		}
	}

out_free:
	rbtree_ops.free(root);

	return 0;
}
