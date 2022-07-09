#ifndef _MAIN_H_
#define _MAIN_H_

enum menu_index {
	INSERT,
	DELETE,
	TRAVERSE,
	SELF_TEST,
	EXIT,
	MAX
};

enum rbtree_color {
	BLACK,
	RED,
};

struct rbtree_node {
	int value;
	int color;
	struct rbtree_node *parents;
	struct rbtree_node *left;
	struct rbtree_node *right;
};

#endif /* !_MAIN_H_ */
