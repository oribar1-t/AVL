#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

/*declerations*/
int isLeaf(AVLNodePtr root);					  /*check if leaf*/
AVLNodePtr left_rotate(AVLNodePtr root);		  /* left rotate function*/
AVLNodePtr right_rotate(AVLNodePtr root);		  /*right rotate function*/
AVLNodePtr avl_balance(AVLNodePtr root);		  /*Balancing function*/
void adjust_height(AVLNodePtr root);			  /*Adjusts the height of the given pointer*/
int heavy_child(AVLNodePtr root);				  /*returning which son height is bigger*/
AVLNodePtr get_min_parent(AVLNodePtr root);		  /* returns the parent of the node with the minimum key useful for deletion*/
void swap_min(AVLNodePtr root, AVLNodePtr p_min); /*swap between a root and the minimum of its right subtree useful for deletion*/

/* Do not change this. You can use but do not touch.*/
int _max(int x, int y)
{
	return (x < y) ? y : x;
}

/* Do not change this. You can use but do not touch.*/
int _abs(int x)
{
	return (x < 0) ? -x : x;
}
/* Operations*/
AVLNodePtr avl_search(AVLNodePtr root, int x)
{ /*preorder traversal recursive function*/

	if (root == NULL)
		return NULL;
	if (root->key == x)
		return root;
	else if (!(isLeaf(root)))
		return (root->key > x ? avl_search(root->child[0], x) : avl_search(root->child[1], x));
	else
		return NULL;
}
AVLNodePtr new_avl_node(int x)
{ /*create new node*/
	AVLNodePtr n = (AVLNodePtr)malloc(sizeof(AVLNode));
	if (n != NULL)
	{
		n->key = x;
		n->height = 0;
		n->child[0] = NULL;
		n->child[1] = NULL;
	}
	return n;
}
AVLNodePtr avl_insert(AVLNodePtr root, int x)
{

	if (root == NULL)
	{ /* return new node after traveling the right place*/
		return new_avl_node(x);
	}
	else if (root->key == x) /*there is already such a key in the tree- nothing to do*/
		return root;
	else if (root->key > x)
	{ /*go left*/
		root->child[0] = avl_insert(root->child[0], x);
		adjust_height(root);
	}
	else if (root->key < x)
	{ /*go right*/
		root->child[1] = avl_insert(root->child[1], x);
		adjust_height(root);
	}
	return avl_balance(root); /*returning pointer to balanced avl tree*/
}
void adjust_height(AVLNodePtr root)
{ /*adjusting the height field of a node*/
	if (!root)
		root->height = -1;
	else if (!root->child[0] && !root->child[1])
	{
		root->height = 0;
	}
	else if (root->child[0] == NULL)
	{
		root->height = root->child[1]->height + 1;
	}
	else if (root->child[1] == NULL)
	{
		root->height = root->child[0]->height + 1;
	}
	else
	{
		root->height = _max(root->child[0]->height, root->child[1]->height) + 1;
	}
}
AVLNodePtr left_rotate(AVLNodePtr root)
{
	AVLNodePtr temp1 = root->child[1]->child[0];
	AVLNodePtr temp2 = root->child[1];
	root->child[1]->child[0] = root;
	root->child[1] = temp1;
	adjust_height(root);
	adjust_height(temp2);
	return temp2;
}
AVLNodePtr right_rotate(AVLNodePtr root)
{
	AVLNodePtr temp1 = root->child[0]->child[1];
	AVLNodePtr temp2 = root->child[0];
	root->child[0]->child[1] = root;
	root->child[0] = temp1;
	adjust_height(root);
	adjust_height(temp2);
	return temp2;
}
int heavy_child(AVLNodePtr root)
{ /*return -1 if left is heavy, 0 if equal, 1 if right is heavy*/
	if (root->child[1] == NULL && root->child[0] == NULL)
		return 0;
	else if (root->child[1] == NULL)
		return -1;
	else if (root->child[0] == NULL)
		return 1;
	else if (root->child[0]->height == root->child[1]->height)
		return 0;
	else if (root->child[0]->height > root->child[1]->height)
		return -1;
	else
		return 1;
}
AVLNodePtr avl_balance(AVLNodePtr root)
{
	if (root->child[0] == NULL && root->child[1] == NULL)
	{
		return root;
	}
	else if (root->child[0] == NULL && root->child[1]->height < 1)
		return root;
	else if (root->child[1] == NULL && root->child[0]->height < 1)
		return root;
	else if (root->child[1] && root->child[0] && (_abs((root->child[0]->height - root->child[1]->height)) <= 1)) /*if balanced*/
		return root;
	else
	{ /*Balance is required*/
		if (heavy_child(root) == 1)
		{										   /*right heavy*/
			if (heavy_child(root->child[1]) == -1) /*check for right-left heavy*/
				root->child[1] = right_rotate(root->child[1]);
			return left_rotate(root);
		}
		else
		{										  /*left-heavy*/
			if (heavy_child(root->child[0]) == 1) /*check for left-right heavy*/
				root->child[0] = left_rotate(root->child[0]);
			return right_rotate(root);
		}
	}
}
AVLNodePtr get_min_parent(AVLNodePtr root)
{ /* useful for the deletion operation*/
	if (root->child[0]->child[0] == NULL)
		return root;
	else
		return get_min_parent(root->child[0]);
}
AVLNodePtr avl_delete(AVLNodePtr root, int x)
{
	AVLNodePtr temp, min;
	if (root == NULL)
		return NULL;
	if (!root->child[0] && !root->child[1] && root->key == x)
	{
		free(root);
		return NULL;
	}
	else if (root->key > x) /*search*/
	{
		root->child[0] = avl_delete(root->child[0], x);
	}
	else if (root->key < x)
	{
		root->child[1] = avl_delete(root->child[1], x);
	}
	else /*key was found*/
	{
		if (root->child[0] == NULL) /* the one child cases*/
		{
			temp = root;
			root = root->child[1];
			free(temp);
			return avl_balance(root);
		}
		else if (root->child[1] == NULL)
		{
			temp = root;
			root = root->child[0];
			free(temp);
			return avl_balance(root);
		}
		else /* two children*/
		{
			if (root->child[1]->child[0]) /* if right child has a left child*/
			{
				temp = get_min_parent(root->child[1]); /* swap some pointers*/
				min = temp->child[0];
				temp->child[0] = root;
				swap_min(root, min);
			}
			else /* swap some pointers*/
			{
				min = root->child[1];
				int h_temp = min->height;
				min->height = root->height;
				root->height = h_temp;
				min->child[0] = root->child[0];
				root->child[0] = NULL;
				root->child[1] = min->child[1];
				min->child[1] = root;
			}
			min->child[1] = avl_delete(min->child[1], x); /*now do same as one child or leaf*/
			return avl_balance(min);					  /* return the minimum balanced*/
		}
	}
	adjust_height(root);
	return avl_balance(root);
}
void swap_min(AVLNodePtr root, AVLNodePtr p_min) /*adjust the right pointers and heights*/
{
	AVLNodePtr temp = p_min->child[1];
	int h_temp = p_min->height;
	p_min->height = root->height;
	root->height = h_temp;
	p_min->child[1] = root->child[1];
	root->child[1] = temp;
	p_min->child[0] = root->child[0];
	root->child[0] = NULL;
	return;
}
int isLeaf(AVLNodePtr root) /* return 1 if node is a leaf*/
{
	return (root->child[0] == NULL && root->child[1] == NULL);
}
/*deleting the tree*/
void delete_avl_tree(AVLNodePtr root)
{
	if (root == NULL)
		return;
	delete_avl_tree(root->child[0]);
	delete_avl_tree(root->child[1]);
	free(root);
	return;
}
AVLNodePtr avl_join(AVLNodePtr root1, AVLNodePtr xnode, AVLNodePtr root2)
{
	int h_root1 = (root1 != NULL ? root1->height : -1);
	int h_root2 = (root2 != NULL ? root2->height : -1);
	if (_abs(h_root1 - h_root2) <= 1)
	{							 /* easy case */
		xnode->child[0] = root1; /* left child of x would be T1*/
		xnode->child[1] = root2; /* right child of x would be T2*/
		adjust_height(xnode);
		xnode = avl_balance(xnode);
		return xnode;
	}
	else if ((h_root1) > (h_root2 + 1))
	{
		root1->child[1] = avl_join(root1->child[1], xnode, root2); /* slide right side of T1*/
		adjust_height(root1);
		root1 = avl_balance(root1);
		return root1;
	}
	else
	{															   /* ((root1->height + 1) < (root2->height)) */
		root2->child[0] = avl_join(root1, xnode, root2->child[0]); /* slide left side of T2*/
		adjust_height(root2);
		root2 = avl_balance(root2);
		return root2;
	}
}
AVLNodePtr avl_split(AVLNodePtr root, int x, AVLNodePtr trees_out[2])
{
	AVLNodePtr temp = NULL;
	if (x == root->key)
	{
		trees_out[0] = root->child[0]; /* the left sub-tree would be T0*/
		trees_out[1] = root->child[1]; /* the right sub-tree would be T1*/
		temp = root;				   /*return the node with the x key*/
	}
	else if (x < root->key)
	{
		temp = avl_split(root->child[0], x, trees_out); /* go to the left child*/
		trees_out[1] = avl_join(trees_out[1], root, root->child[1]);
	}
	else
	{													/*(x > root->key)*/
		temp = avl_split(root->child[1], x, trees_out); /* go to the right child*/
		trees_out[0] = avl_join(root->child[0], root, trees_out[0]);
	}
	return temp;
}
