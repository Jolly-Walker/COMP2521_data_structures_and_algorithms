// This is the helper function file
// It will contain a few graph functions and other helper functions

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"


// -------------------------- Functions for Part 1 ---------------------------
Node newNode(char *str, int id) {
	Node new = malloc (sizeof *new);
	if (new == NULL) {
		fprintf(stderr, "couldn't allocate Memory\n");
		exit(1);
	}
	new->urlName = str;
	new->urlID = id;
	new->next = NULL;

	return new;
}

// When calling this function make sure head is not NULL
void insertNode(Node head, Node new) {
	if (head == NULL) {
		fprintf(stderr, "ERROR, head is null\n");
		exit(1);
	}
	Node curr = head;
	while (curr->next != NULL) {
		if (strcmp(curr->urlName, new->urlName) == 0) {
			free(new);
			return;
		}
		curr = curr->next;
	}
	curr->next = new;
}
// Create a new empty graph
Graph newGraph(void) {
	Graph g = malloc (sizeof *g);
	if (g == NULL) {
		fprintf(stderr, "couldn't allocate Memory\n");
		exit(1);
	}

	(*g) = (struct graph){
		.nV = 0,
		.nE = 0,
		.files = { NULL }
	};
	return g;
}


// Remove an existing graph
void dropGraph(Graph g) {
	if (g == NULL) return;
	for (int i = 0; i < g->nV; i++) {
		Node curr = g->files[i];
		while (curr != NULL) {
			Node next = curr->next;
			free(curr);
			curr = next;
		}
	}
	free(g);
}


double calcIn(Graph g, int v, int u, int *inDeg) {
	double divisor = 0.0;
	Node curr = g->files[v];
	while (curr != NULL) {
		divisor += inDeg[curr->urlID];
		curr = curr->next;
	}
	return inDeg[u]/divisor;
}

double calcOut(Graph g, int v, int u, int *outDeg) {
	double divisor = 0;
	Node curr = g->files[v];
	while (curr != NULL) {
		if (outDeg[curr->urlID] == 0) {
			divisor += 0.5;
		} else {
			divisor += outDeg[curr->urlID];
		}
		curr = curr->next;
	}
	if (outDeg[u] == 0) {
		return 0.5/divisor;
	} else {
		return outDeg[u]/divisor;
	}
}

double calcDiff(double *prev, double *pr, int maxNode) {

	double val = 0;
	for (int i = 0; i < maxNode; i++) {
		double curr = fabs(prev[i] - pr[i]);
		val += curr;
	}

	return val;
}
// ----------------------- End functions for Part 1 ------------------------



// -------------------------- Functions for Part 2 ---------------------------

Rank newRank(char *str, int place) {
	Rank new = malloc (sizeof *new);
	if (new == NULL) {
		fprintf(stderr, "couldn't allocate Memory\n");
		exit(1);
	}
	new->urlName = str;
	new->placing = place;
	new->next = NULL;

	return new;
}

List newList(char *str) {
    List new = malloc (sizeof *new);
	if (new == NULL) {
		fprintf(stderr, "couldn't allocate Memory\n");
		exit(1);
	}
	new->urlName = str;
	new->matchCount = 1;
	new->next = NULL;

	return new;
}

// inserts into a sorted list keeping it sorted

List insertList(List head, char *str, Rank rankList) {
	char *buff2 = my_strdup(str);

	if (head == NULL) {
		List newNode = newList(buff2);
		head = newNode;
		return head;
	} else if (strcmp(str, head->urlName) == 0){
		head->matchCount++;
		return head;
	}

	List prev = head;
	List curr = head->next;
	while (curr != NULL) {
		if (strcmp(str, curr->urlName) == 0) {
			curr->matchCount++;
			prev->next = curr->next;
			curr->next = NULL;
			return reInsert(head, curr, rankList);
			break;
		}
		prev = curr;
		curr = curr->next;
	}

	if (curr == NULL) {
		List newNode = newList(buff2);
		return reInsert(head, newNode, rankList);
	}

	return head;
}

// head is not NULL
// node is not in list already
List reInsert(List head, List node, Rank rankList) {
	// if node has higher # of matches than head
	if (head->matchCount < node->matchCount) {
		node->next = head;
		return node;
	} else if (head->matchCount == node->matchCount) {
		//node and head same # of matched words, node higher pagerank
		if (isHigerRank(node->urlName, head->urlName, rankList)) {
			node->next = head;
			return node;
		} else {
			if (head->next == NULL) {
				head->next = node;
				return head;
			}
			List prev = head;
			List curr = head->next;
			while (curr->matchCount == node->matchCount && curr->next != NULL) {
				if (isHigerRank(node->urlName, curr->urlName, rankList)) {
					prev->next = node;
					node->next = curr;
					return head;
				}
				prev = curr;
				curr = curr->next;
			}
			if (curr->matchCount == node->matchCount) {
				if (isHigerRank(node->urlName, curr->urlName, rankList)) {
					prev->next = node;
					node->next = curr;
					return head;
				} else {
					prev = curr;
					curr = curr->next;
				}
			}
			prev->next = node;
			node->next = curr;
			return head;
		}
	} else {
		if (head->next == NULL) {
			head->next = node;
			return head;
		}

		List prev = head;
		List curr = head->next;
		while (curr != NULL) {
			if (curr->matchCount == node->matchCount &&
				isHigerRank(node->urlName, curr->urlName, rankList)) {
				prev->next = node;
				node->next = curr;
				return head;
			}
			if (curr->matchCount < node->matchCount) {
				prev->next = node;
				node->next = curr;
				return head;
			}
			prev = curr;
			curr = curr->next;
		}
		prev->next = node;
		node->next = curr;
		return head;
	}
}

// is a's Pagerank higher than b's Pagerank
int isHigerRank(char *a, char *b, Rank rankList) {
	Rank node = rankList;
	//abusing the fact that rankList is in ascending order
	//so whatever thats seen first has a lower rank
	while (node != NULL) {
		if (strcmp(a, node->urlName) == 0) {
			return 0;
		}
		if (strcmp(b, node->urlName) == 0) {
			return 1;
		}
		node = node->next;
	}
	fprintf(stderr, "ERROR isHigerRank couldnt find %s or %s in list\n", a, b);
	return 0;
}



// ----------------------- End functions for Part 2 ------------------------


// strdup not available so heres one
char *my_strdup(char *str) {
    int size = strlen(str) + 1;
    char *new = malloc(size);
    char *newp = new;
    while (*str) {
        *newp++ = *str++;
    }
    *newp = '\0';
    return new;
}

