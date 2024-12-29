#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "List.h"

//SOME parts referred from https://www.youtube.com/watch?v=pdMPvMM12G0&list=WL&index=1
struct node *sortUrl(struct list *urlList) {
	//Bubble sort
	struct node *previous;
	struct node *current1;
	struct node *current2;
	struct node *temp;
	struct node *end;

	for (end = NULL; end != urlList->nodes->next; end = current1) {
		for (previous = current1 = urlList->nodes; current1->next != end;
		     previous = current1, current1 = current1->next) {
			current2 = current1->next;
			if (current1->counter < current2->counter ||
			    (current1->counter == current2->counter &&
			     current1->rank < current2->rank)) {
				current1->next = current2->next;
				current2->next = current1;
				// Average swap cases
				if (current1 != urlList->nodes) {
					previous->next = current2;
				} else {
					//If swap at head
					urlList->nodes = current2;
				}
				//Reset pointer position
				temp = current2;
				current1 = current2;
				current2 = temp;
			}
		}
	}
	return urlList->nodes;
}

struct node *sortUrlAlphabetically(struct list *urlList) {
	//Bubble sort
	struct node *previous;
	struct node *current1;
	struct node *current2;
	struct node *temp;
	struct node *end;

	for (end = NULL; end != urlList->nodes->next; end = current1) {
		for (previous = current1 = urlList->nodes; current1->next != end;
		     previous = current1, current1 = current1->next) {
			current2 = current1->next;
			if (strcmp(current1->url, current2->url) > 0) {
				current1->next = current2->next;
				current2->next = current1;
				// Average swap cases
				if (current1 != urlList->nodes) {
					previous->next = current2;
				} else {
					//If swap at head
					urlList->nodes = current2;
				}
				//Reset pointer position
				temp = current2;
				current1 = current2;
				current2 = temp;
			}
		}
	}
	return urlList->nodes;
}

void removeDuplicates(struct node *head) {
	struct node *current1 = head;
	struct node *current2;
	struct node *temp;

	while (current1 != NULL && current1->next != NULL) {
		current2 = current1;
		while (current2->next != NULL) {
			if (strcmp(current1->url, current2->next->url) == 0) {
				current1->counter++;
				temp = current2->next;
				current2->next = current2->next->next;
				free(temp);
			} else {
				current2 = current2->next;
			}
		}
		current1 = current1->next;
	}
}

void freeList(struct list *urlList) {
	struct node *currentNode = urlList->nodes;
	while (currentNode != NULL) {
		struct node *removedNode = currentNode;
		currentNode = currentNode->next;
		free(removedNode);
	}
	free(urlList);
}

void printList(struct list *urlList) {
	int printed = 0;
	struct node *current = urlList->nodes;
	while (current != NULL && printed < 30) {
		printf("%s\n", current->url);
		current = current->next;
		printed++;
	}
}

struct list *createList() {
	struct list *new_list = malloc(sizeof(struct list));
	assert(new_list != NULL);
	new_list->nodes = NULL;
	return new_list;
}

struct node *appendNode(struct list *urlList, char *token) {
	struct node *newNode = malloc(sizeof(struct node));
	assert(newNode != NULL);
	strcpy(newNode->url, token);
	newNode->counter = 1;
	newNode->rank = 0;
	newNode->outdegrees = 0;
	newNode->next = NULL;

	struct node *current = urlList->nodes;

	if (current == NULL) {
		return newNode;
	} else {
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
		return urlList->nodes;
	}
}