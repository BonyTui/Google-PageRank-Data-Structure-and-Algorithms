// Graph ADT interface ... From COMP2521 Lecture Code

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

#include "List.h"

typedef struct GraphRep *Graph;

// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   Vertex v;
   Vertex w;
} Edge;

//Create and return new graph of with V vertices
Graph newGraph(int V);
//Insert edge e into graph
void insertEdge(Graph g, Edge e);
//Remove edge e from graph
void removeEdge(Graph g, Edge e);
//Check if vertex v and w are connected
bool adjacent(Graph g, Vertex v, Vertex w);
//Print out edges in graph
void showGraph(Graph g, struct list *urlList);
//Print out adjacency matrix representation of graph
void printGraphMatrix(Graph g, int size, struct list *urlList);
//Free graph memory
void freeGraph(Graph g);
//Convert array index to url name
char *indexToUrl(int index, struct list *urlList);

#endif