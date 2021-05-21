#ifndef GRAPH_H_
#define GRAPH_H_
#define MAX_NODE 100

// Part-1 Structures
typedef struct node *Node;

struct node {
	char *urlName; // name of url;
    int urlID; // id of the url
	Node next; // link to next node
};

typedef struct graph *Graph;

struct graph {
	int nV; // #vertices
	int nE; // #edges
	Node files[MAX_NODE]; // array of lists of files
};
// End Part-1 Structures


// Part-2 Structures
typedef struct listNode *List;

struct listNode {
	char *urlName; // name of url;
    int matchCount; // id of the url
	List next; // link to next node
};

typedef struct rankNode *Rank;

struct rankNode {
	char *urlName; // name of url;
    int placing; // rank of url, i.e 0th place is highest Pagerank, n-1th place is lowest Pagerank
	Rank next; // link to next node
};

// End Part-2 Structures

// Part-1 functions
// Node functions
Node newNode(char *str, int id);
void insertNode(Node head, Node new);

// Graph functions
Graph newGraph(void);
void dropGraph(Graph g);

//other functions
double calcIn(Graph g, int v, int u, int *inDeg);
double calcOut(Graph g, int v, int u, int *outDeg);
double calcDiff(double *prev, double *pr, int maxNode);

// End Part-1 functions

// Part-2 functions
Rank newRank(char *str, int place);
List newList(char *str);
List insertList(List head, char *str, Rank rankList);
List reInsert(List head, List node, Rank rankList);
int isHigerRank(char *a, char *b, Rank rankList);
// End Part-2 functions

char *my_strdup(char *str);
#endif
