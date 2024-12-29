//Written by Tony Bui
//z5448625
//Program Description: Scan collection and calculate pagerank of each url

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "List.h"
#include "Graph.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Libraries       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void PageRank(double damp, double diffPr, int maxIterations, 
int numUrl, Graph internet, struct list *urlList, FILE *pagerankList);
void printPageRank(struct list *urlList, FILE *pagerankList);
int urlToIndex (struct list *urlList, char *url);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Prototypes       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    //Get the variables for pagerank
    assert(argc == 4);
    double damp = atof(argv[1]);
    double diffPr = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    //Access collection.txt to see all the urls that needs to be visisted
    FILE *collection = fopen("collection.txt", "r");
    FILE *pagerankList = fopen("pagerankList.txt", "w");

    assert(collection != NULL && pagerankList != NULL);

    char searchUrl[MAX_URL_LENGTH];
    struct list *urlList = createList();

    //Get list of urls from collection.txt
    int numUrl = 0;
    while (fscanf(collection, "%s", searchUrl) == 1) {
        urlList->nodes = appendNode(urlList, searchUrl);
        numUrl++;
    }
    
    // Put all websites into a graph
    Graph internet = newGraph(numUrl);
    //Access each url and get their outdegrees
    struct node *currentUrl = urlList->nodes;
    while (currentUrl != NULL) {
        char tempUrl[MAX_URL_LENGTH];
		strcpy(tempUrl, currentUrl->url);
        char *openUrl = strcat(tempUrl, ".txt");
        FILE *urlFile = fopen(openUrl, "r");
        assert(urlFile != NULL);
        char searchUrl[MAX_WORD_LENGTH];
		char readLine[MAX_LINE_LENGTH];
		char section1[] = "#start Section-1\n";
        
        //Skip to start of section 1
        fgets(readLine, MAX_LINE_LENGTH, urlFile);

        while (strcmp(readLine, section1) != 0) {
            fgets(readLine, MAX_LINE_LENGTH, urlFile);
        }

        //Store all outdegrees into a list to remove duplicates
        struct list *tempList = createList();
        while (fscanf(urlFile, "%s", searchUrl) == 1 && 
        strcmp(searchUrl, "#end") != 0) {
            if (strcmp(searchUrl, currentUrl->url) != 0) {
                tempList->nodes = appendNode(tempList, searchUrl);
            }
        }
        
        removeDuplicates(tempList->nodes);
        struct node *current = tempList->nodes;
        while (current != NULL) {
            // Register edges between urls inside graph
            struct Edge e;
            e.v = urlToIndex(urlList, currentUrl->url);
            e.w = urlToIndex(urlList, current->url);
            insertEdge(internet, e);
            currentUrl->outdegrees++;
            current = current->next;
        }
        freeList(tempList);
        
        //Go to next file
        currentUrl = currentUrl->next;
        fclose(urlFile);
    }

    //Calculate pagerank
    PageRank(damp, diffPr, maxIterations, numUrl, internet, urlList,
    pagerankList);

    //Print out results to file
    sortUrl(urlList);
    printPageRank(urlList, pagerankList);

    //Free all allocated memory
	freeList(urlList);
    freeGraph(internet);

    //Close Files after Opening them
	fclose(collection);
    fclose(pagerankList);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Main       ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void PageRank(double damp, double diffPr, int maxIterations, 
int numUrl, Graph internet, struct list *urlList, FILE *pagerankList) {
    int iteration = 0;
    double diff = 0;
    double dampFactor = ((1.0-damp)/numUrl);
    double initial = 1.0/numUrl;

    //Initialize every vertex's rank to 1/numUrl
    struct node *current1 = urlList->nodes;
    while (current1 != NULL) {
        current1->rank = initial;
        current1 = current1->next;
    }

    //Update ranking of all vertices using ranking in prev iteration
    while (iteration < maxIterations) {
        //Initialize prevRanking array
        struct node *prevRanking = malloc(sizeof(struct node) * numUrl);
        struct node *prev = urlList->nodes;
        for (int i = 0; i < numUrl; i++) {
            prevRanking[i] = *prev;
            prev = prev->next;
        }

        //Loop over all vertices
        struct node *current = urlList->nodes;
        int i = 0;
        while (current != NULL) {
            //Loop over all indegrees of url at index i
            struct node *current2 = urlList->nodes;
            double newPR = 0.0;
            for (int j = 0; j < numUrl; j++) {
                if (adjacent(internet, j, i)) {
                    //If an indegree of i, add its old Pr/outdegrees
                    newPR += (prevRanking[j].rank/current2->outdegrees);
                }
                current2 = current2->next;
            }
            double newRank = dampFactor + damp * newPR;
            diff += fabs(current->rank - newRank);
            current->rank = newRank;

            i++;
            current = current->next;
        }

        if (diff < diffPr) {
            free(prevRanking);
            break;
        }
        diff = 0;
        iteration++;
        free(prevRanking);
    }
}

void printPageRank(struct list *urlList, FILE *pagerankList) {
    struct node *current2 = urlList->nodes;
    while (current2 != NULL) {
        fprintf(pagerankList, "%s, ", current2->url);
        fprintf(pagerankList, "%d, ", current2->outdegrees);
        fprintf(pagerankList, "%.7f\n", current2->rank);
        current2 = current2->next;
    }
}

int urlToIndex (struct list *urlList, char *url) {
    int i = 0;
    struct node *current = urlList->nodes;
    while (current != NULL) {
        if (strcmp(url, current->url) == 0) {
            break;
        }
        i++;
        current = current->next;
    }
    return i;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Helper Functions       ////////////////////////
////////////////////////////////////////////////////////////////////////////////