// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "part3.h"

#define strEQ(s, t) (strcmp ((s), (t)) == 0)
#define strLT(s, t) (strcmp ((s), (t)) < 0)

// Function signatures

static Link newNode (char *);
static void dropNode (Link);
static int findNode (Link, char *, Link *, Link *);
static char *private_strdup(char *str);

// newSet()
// - create an initially empty Set
Set newSet (void)
{
	Set new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate Set");
	*new = (SetRep){.nelems = 0, .elems = NULL};
	return new;
}

// dropSet(Set)
// - clean up memory associated with Set
void dropSet (Set s)
{
	if (s == NULL)
		return;
	Link curr = s->elems;
	while (curr != NULL) {
		Link next = curr->next;
		dropNode (curr);
		curr = next;
	}

	free (s);
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto (Set s, char *str)
{
	assert (s != NULL);
	Link curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (found)
		return; // already in Set

	Link new = newNode (str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	} else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}

// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom (Set s, char *str)
{
	assert (s != NULL);
	Link curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (!found)
		return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	dropNode (curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem (Set s, char *str)
{
	assert (s != NULL);
	Link curr, prev;
	return findNode (s->elems, str, &curr, &prev);
}

// nElems(Set)
// - return # elements in Set
int nElems (Set s)
{
	assert (s != NULL);
	return s->nelems;
}

// showSet(Set)
// - display Set (for debugging)
void showSet (Set s) {
	Link curr;
	if (s->nelems == 0)
		printf ("Set is empty\n");
	else {
		printf ("Set has %d elements:\n", s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			printf ("[%03d] %s\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}


// Helper functions

static Link newNode (char *str)
{
	Link new = malloc (sizeof *new);
	assert (new != NULL);
	new->val = private_strdup(str);
	new->next = NULL;
	return new;
}

static void dropNode (Link curr)
{
	assert (curr != NULL);
	free (curr->val);
	free (curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode (Link list, char *str, Link *cur, Link *pre)
{
	Link curr = list, prev = NULL;
	while (curr != NULL && strLT (curr->val, str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr;
	*pre = prev;
	return (curr != NULL && strEQ (str, curr->val));
}

// END OF CODE TAKEN FROM COMP2521 LABS

// strdup not available so heres one
static char *private_strdup(char *str) {
    int size = strlen(str) + 1;
    char *new = malloc(size);
    char *newp = new;
    while (*str) {
        *newp++ = *str++;
    }
    *newp = '\0';
    return new;
}

//Counts the Number of valid zeros for each row, aka element
int countZerosRow(int nEle, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet) {
	int largestX = 0;
	for (int i = 0; i < nEle; i++) {
        int lineX = 0;
		if (eleSet[i] == 1) continue;
        for (int j = 0; j < nEle; j++) {
			if (posSet[j] == 1) continue;
            if (scaledDistance[i][j] == 0) {
				lineX++;
			}
        }
        if (lineX > largestX) largestX = lineX;
    }
	return largestX;
}

//same function but for Column, aka position
int countZerosCol(int nEle, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet) {
	int largestY = 0;
	for (int i = 0; i < nEle; i++) {
        int lineY = 0;
		if (posSet[i] == 1) continue;
        for (int j = 0; j < nEle; j++) {
			if (eleSet[j] == 1) continue;
            if (scaledDistance[j][i] == 0) lineY++;
        }
        if (lineY > largestY) largestY = lineY;
    }
	return largestY;
}

// crosses all rows/elements where the row has number of zeros equal to num
int crossRow(int nEle, int num, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet) {
	int crossCount = 0;
	for (int i = 0; i < nEle; i++) {
        int lineX = 0;
		if (eleSet[i] == 1) continue;

        for (int j = 0; j < nEle; j++) {
			if (posSet[j] == 1) continue;
            if (scaledDistance[i][j] == 0) lineX++;
        }

        if (num == lineX) {
			crossCount++;
			eleSet[i] = 1;
		}
    }
	return crossCount;
}

// crosses all col/position where the row has number of zeros equal to num
int crossCol(int nEle, int num, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet) {
	int crossCount = 0;
	for (int i = 0; i < nEle; i++) {
        int lineY = 0;
		if (posSet[i] == 1) continue;

        for (int j = 0; j < nEle; j++) {
			if (eleSet[j] == 1) continue;
            if (scaledDistance[j][i] == 0) lineY++;
        }

        if (num == lineY) {
			crossCount++;
			posSet[i] = 1;
		}

    }
	return crossCount;
}

