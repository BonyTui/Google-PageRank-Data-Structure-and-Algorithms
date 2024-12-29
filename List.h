// Singly linked list ADT interface

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH 1000
#define MAX_LINE_LENGTH 1000
#define MAX_URL_LENGTH 100

struct node {
	char url[MAX_WORD_LENGTH];
	int counter;
	double rank;
	int outdegrees;
	struct node *next;
};

struct list {
	struct node *nodes;
};

//Add node to end of list
struct node *appendNode(struct list *urlList, char *token);
//Create and return empty list
struct list *createList();
//Free all malloc from list
void freeList(struct list *urlList);
//Print url of all nodes in list
void printList(struct list *urlList);
//Remove nodes with duplicated url
void removeDuplicates(struct node *head);
//Bubble sorting a linked list for searchPagerank
struct node *sortUrl(struct list *urlList);
//Bubble sorting a linked list for invertedIndex
struct node *sortUrlAlphabetically(struct list *urlList);

#endif
