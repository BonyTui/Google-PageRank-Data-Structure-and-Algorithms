//Written by Tony Bui
//z5448625
//Program Description: Takes one or more search terms and outputs relevant URLs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "List.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Libraries       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
	//Access invertedIndex.txt to see all the urls that mentions searchTerm
	FILE *invertedIndex = fopen("invertedIndex.txt", "r");
	char searchTerm[MAX_WORD_LENGTH];
	char urlLine[MAX_LINE_LENGTH];

	assert(invertedIndex != NULL);

	//Store urls read on urlLine into urlList;
	struct list *urlList = createList();
	char delim[] = " ,\n";

	while (fscanf(invertedIndex, "%s", searchTerm) == 1) {
		for (int i = 1; i < argc; i++) {
			//When found searchTerm in file
			if (strcmp(searchTerm, argv[i]) == 0) {
				fgets(urlLine, MAX_LINE_LENGTH, invertedIndex);
				char *token = strtok(urlLine, delim);

				while (token != NULL) {
					//Separate urls on the same line using strtok, and add each to list
					urlList->nodes = appendNode(urlList, token);
					token = strtok(NULL, delim);
				}
			}
		}
	}

	//Remove duplicate urls from list
	removeDuplicates(urlList->nodes);

	//Access pagerankList.txt to set ranking of each url
	FILE *pagerankList = fopen("pagerankList.txt", "r");
	char searchUrl[MAX_WORD_LENGTH];
	char pagerankLine[MAX_LINE_LENGTH];

	assert(pagerankList != NULL);

	while (fscanf(pagerankList, "%s", searchUrl) == 1) {
		struct node *current = urlList->nodes;
		searchUrl[strlen(searchUrl) - 1] = 0;
		while (current != NULL) {
			//When found searchUrl in file
			if (strcmp(searchUrl, current->url) == 0) {
				fgets(pagerankLine, MAX_LINE_LENGTH, pagerankList);
				char *token = strtok(pagerankLine, delim);
				while (token != NULL) {
					//Get only the rating and store into struct
					if (atof(token) < 1 && atof(token) > 0) {
						current->rank = atof(token);
					}
					token = strtok(NULL, delim);
				}
			}
			current = current->next;
		}
	}

	//Sort by ascending number of search terms/counters and page ranking
	urlList->nodes = sortUrl(urlList);

	//Output final list
	printList(urlList);

	//Free all allocated memory
	freeList(urlList);

	//Close Files after Opening them
	fclose(invertedIndex);
	fclose(pagerankList);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Main       ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////