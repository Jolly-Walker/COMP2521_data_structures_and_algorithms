//Set implementation taken from lab
// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

#define MAX_LIST 100
typedef struct SetRep *Set;

typedef struct linkNode *Link;

struct linkNode {
	char *val;
	Link next;
};

typedef struct SetRep {
	int nelems;
	Link elems;
} SetRep;


// Function signatures

Set newSet (void);
void dropSet (Set);
void insertInto (Set, char *);
void dropFrom (Set, char *);
int isElem (Set, char *);
int nElems (Set);
void showSet (Set);

// My own functions
char *my_strdup(char *str);
int countZerosRow(int nEle, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet);
int countZerosCol(int nEle, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet);
int crossRow(int nEle, int num, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet);
int crossCol(int nEle, int num, double scaledDistance[MAX_LIST][MAX_LIST], int *eleSet, int *posSet);
#endif

