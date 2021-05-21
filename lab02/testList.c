// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

void test_empty_list(DLList L);
void test_list(DLList L);

int main (void)
{

	DLList myList = getDLList (stdin);
	putDLList (stdout, myList);
	assert (validDLList (myList));
	if (DLListLength(myList) != 0) {
        printf("For usage, just type ctr + d\n");
        exit(1);
    }
    test_empty_list(myList);
    printf("\n--------Test complete!--------\n");
	freeDLList (myList);
	return EXIT_SUCCESS;
}

void test_empty_list(DLList L) {
    printf("---List is empty, adding a line using before---\n");

    printf("Before:\n");
    putDLList (stdout, L);
    DLListBefore(L, "@@@@1");
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));

    printf("---Deleting list with single item---\n");
    printf("Before:\n");
    putDLList (stdout, L);
    DLListDelete(L);
    printf("After:\n");

    printf("---Using Delete on an empty List---\n");
    DLListDelete(L);
    assert (validDLList (L));
    printf("Passed!\n");

    printf("---List is empty, adding a line using after---\n");
    printf("Before:\n");
    putDLList (stdout, L);
    DLListAfter(L, "@@@@2");
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));

    printf("---Adding a Line before first point of list---\n");

    printf("Before:\n");
    putDLList (stdout, L);
    DLListBefore(L, "@@@@1");
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));

    DLListMove(L,1);
    printf("---Adding a Line after last point of list---\n");
    printf("Before:\n");
    putDLList (stdout, L);
    DLListAfter(L, "@@@@3");
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));

    printf("---Adding Line before inside list---\n");
	printf("Before:\n");
	putDLList (stdout, L);
    DLListBefore(L, "@@@2.1");
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));

    printf("---Adding Line after inside list---\n");
	printf("Before:\n");
	putDLList (stdout, L);
    DLListAfter(L, "@@@2.2");
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));

    printf("---Deleting Line inside list---\n");
	printf("Before:\n");
	putDLList (stdout, L);
    DLListDelete(L);
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));

    printf("---Deleting last Line---\n");
	printf("Before:\n");
	putDLList (stdout, L);
    DLListDelete(L);
    printf("After:\n");
    putDLList (stdout, L);
    assert (validDLList (L));
}

