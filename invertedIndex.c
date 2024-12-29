//Written by Tony Bui
//z5448625
//Program Description: Scan collection and outputs search terms and their urls

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "List.h"
#include "BST.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Libraries       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int normalize(char *searchTerm);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Prototypes       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
	//Access collection.txt to see all the urls that needs to be visisted
    FILE *collection = fopen("collection.txt", "r");
	FILE *invertedIndex = fopen("invertedIndex.txt", "w");

	assert(collection != NULL && invertedIndex != NULL);

    char searchUrl[MAX_URL_LENGTH];
    struct list *urlList = createList();

    //Get list of urls from collection.txt
    while (fscanf(collection, "%s", searchUrl) == 1) {
        urlList->nodes = appendNode(urlList, searchUrl);
    }

    //Access each url and get each word into a BST
	struct Node *bst = bstNew();
    struct node *currentUrl = urlList->nodes;

    while (currentUrl != NULL) {
		char tempUrl[MAX_URL_LENGTH];
		strcpy(tempUrl, currentUrl->url);
        char *openUrl = strcat(tempUrl, ".txt");
        FILE *urlFile = fopen(openUrl, "r");
		assert(urlFile != NULL);
        char searchTerm[MAX_WORD_LENGTH];
		char readLine[MAX_LINE_LENGTH];
		char section2[] = "#start Section-2\n";
		
		//Skip to start of section 2
		fgets(readLine, MAX_LINE_LENGTH, urlFile);

		while (strcmp(readLine, section2) != 0) {
			fgets(readLine, MAX_LINE_LENGTH, urlFile);
		}

		//Beginning of section 2 with all the words
		while (fscanf(urlFile, "%s", searchTerm) == 1 && 
		strcmp(searchTerm, "#end") != 0) {
			//Normalize the searchTerm
			//Put words and their url into a BST
			if (normalize(searchTerm) == 1) {
				bst = bstInsert(bst, searchTerm, currentUrl->url);
			}
		}
        
		//Go to next file
        currentUrl = currentUrl->next;
		fclose(urlFile);
	}

	//Traverse the ADT, print out the word and urls to invertedIndex.txt
	bstInOrder(bst, invertedIndex);

    //Free all allocated memory
	freeList(urlList);

    //Close Files after Opening them
	fclose(collection);
	fclose(invertedIndex);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Main       ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int normalize(char *searchTerm) {
	//Loop over each letter
	for (int i = 0; i < strlen(searchTerm); i++) {
		//If punctuation marks were found, remove if 
		// at end of list, with no more alphanumerical letter after 
		int hasPunctuation = 0;
		int isAtEnd = 0;
		if (searchTerm[i] == '.' || searchTerm[i] == ',' || searchTerm[i] == ':' ||
		searchTerm[i] == ';' || searchTerm[i] == '?' || searchTerm[i] == '*') {
			hasPunctuation = 1;
			for (int j = i + 1; j < strlen(searchTerm); j++) {
				if ((!ispunct(searchTerm[j])) || (searchTerm[j] >= '!' && searchTerm[j] <= ')') ||
				 (searchTerm[j] >= '@' && searchTerm[j] <= '~') || 
				(searchTerm[j] >= '<' && searchTerm[j] <= '>') ||
				searchTerm[j] == '+' || searchTerm[j] == '-' ||
				searchTerm[j] == '/') {
					isAtEnd = 1;
				}
			}
		}
		if (isAtEnd == 0 && hasPunctuation == 1) {
			searchTerm[i] = 0;
		}
	}
	
	//Lower case
	for (int i = 0; i < strlen(searchTerm); i++) {
		searchTerm[i] = tolower(searchTerm[i]);
	}

	//If Whole word is deleted after normalized, don't insert
	if (strlen(searchTerm) == 0) {
		return 0;
	} else {
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Helper Functions       ////////////////////////
////////////////////////////////////////////////////////////////////////////////