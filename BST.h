// Interface to binary search tree module
// Taken from Lab03

#ifndef BST_H
#define BST_H

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "List.h"


struct Node {
	char value[MAX_WORD_LENGTH];
	struct list *urlList;
	struct Node *left;
	struct Node *right;
};

////////////////////////////////////////////////////////////////////////

// Creates a new empty BST
struct Node *bstNew(void);

// Frees all the memory associated with the given BST
void bstFree(struct Node *t);

// Inserts a new value into the given BST and returns the root of the
// updated BST. Does not insert duplicate values.
struct Node *bstInsert(struct Node *t, char *val, char *url);

// Checks whether a value is in the given BST.
bool bstFind(struct Node *t, char *val);

// Prints a struct Node *to stdout.
void bstShow(struct Node *t);

// Prints the in-order traversal of the given BST
void bstInOrder(struct Node *t, FILE *invertedIndex);

////////////////////////////////////////////////////////////////////////

#endif

