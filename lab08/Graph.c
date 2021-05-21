// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"
#define CITY_COUNT 30

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);
	if (src == dest) {
	    path[0] = src;
	    return 1;
	}
	
	int prevCity[CITY_COUNT];
	for (int i = 0; i < CITY_COUNT; i++) {
	    prevCity[i] = -1;
	}
	
	int visited[CITY_COUNT] = {0};
	
	Queue myQueue = newQueue();
	QueueJoin(myQueue, src);
	while (!QueueIsEmpty(myQueue)) {
	    Vertex myVertex = QueueLeave(myQueue);
	    visited[myVertex] = 1;
	    Vertex next;
	    
	    for (next = 0; next < CITY_COUNT; next++) {
	        if (g->edges[myVertex][next] != 0 && g->edges[myVertex][next] < max) {
	            //skip if the vertex is visited
	            if (visited[next]) continue;
	            
	            prevCity[next] = myVertex; 
	            visited[next] = 1;
	            QueueJoin(myQueue, next);
	            
	            if (next == dest) break;
	        }
	    }
	}
	dropQueue(myQueue);
	int i = 0;
	int *reverse_path = calloc(g->nV, sizeof(int));
	Vertex mover = dest;
	while (prevCity[mover] != -1) {
	    reverse_path[i] = mover;
	    i++;
	    mover = prevCity[mover];
	}
	reverse_path[i] = src;
	
	int j = 0;
	int total = i;
	
	while (j <= total && total != 0) {
	    path[j] = reverse_path[i];
	    i--;
	    j++;
	}
	
	free(reverse_path);
	return j; 
	// never find a path ... you need to fix this
}
