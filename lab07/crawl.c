// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders
// <vince@kyllikki.org>

#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "graph.h"
#include "html.h"
#include "set.h"
#include "stack.h"
#include "url_file.h"
#include "queue.h"

#define BUFSIZE 1024

static void setFirstURL (char *, char *);

int main (int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int maxURLs;

	if (argc > 2) {
		strcpy (baseURL, argv[1]);
		setFirstURL (baseURL, firstURL);
		maxURLs = atoi (argv[2]);
		if (maxURLs < 40)
			maxURLs = 40;
	} else {
		fprintf (stderr, "Usage: %s BaseURL MaxURLs\n", argv[0]);
		exit (1);
	}

	// You need to modify the code below to implement:
	//
	// add firstURL to the ToDo list
	Queue myQueue = newQueue();
	enterQueue(myQueue, firstURL);
	// initialise Graph to hold up to maxURLs
	Graph myGraph = newGraph((size_t)maxURLs);
	// initialise set of Seen URLs
	Set mySet = newSet();
	insertInto(mySet, firstURL);
	// while (ToDo list not empty and Graph not filled) {
	while (!emptyQueue(myQueue) && nVertices(myGraph) < (size_t)maxURLs) {
	//    grab Next URL from ToDo list
	    char *myString = leaveQueue(myQueue);
	//    if (not allowed) continue
	    if (strstr(myString, "unsw.")) {free(myString); continue;}
	    URL_FILE *myFile = url_fopen( myString, "r");
	//    foreach line in the opened URL {
	    while (!url_feof(myFile)) {
	        url_fgets(buffer, sizeof(buffer), myFile);
	        int pos = 0;
	        char result[BUFSIZE];
	        memset(result, 0, BUFSIZE);
	//       foreach URL on that line {
	        while ((pos = GetNextURL(buffer, firstURL, result,pos)) > 0) {
	            printf("Found: %s\n", result);
	//          if (Graph not filled or both URLs in Graph)
	//             add an edge from Next to this URL
	            if ((nVertices(myGraph) < (size_t)maxURLs) || 
	            (isConnected(myGraph,myString,result) == 0)) {
	                addEdge(myGraph, myString, result);
	            }
	//          if (this URL not Seen already) {
	//             add it to the Seen set
	//             add it to the ToDo list
	//          }
	            if (!isElem(mySet, result)) {
	                insertInto(mySet, result);
	                enterQueue(myQueue, result);
	            }
	            memset(result, 0, BUFSIZE);
	//       }
	//    }
            }
        }
	//    close the opened URL
	    url_fclose(myFile);
	//    sleep(1)
	    free(myString);
	    sleep(1);
	// }
	}
	if (!(handle = url_fopen (firstURL, "r"))) {
		fprintf (stderr, "Couldn't open %s\n", next);
		exit (1);
	}
	while (!url_feof (handle)) {
		url_fgets (buffer, sizeof (buffer), handle);
		// fputs(buffer,stdout);
		int pos = 0;
		char result[BUFSIZE];
		memset (result, 0, BUFSIZE);
		while ((pos = GetNextURL (buffer, firstURL, result, pos)) > 0) {
			printf ("Found: '%s'\n", result);
			memset (result, 0, BUFSIZE);
		}
	}
	url_fclose (handle);
	dropSet(mySet);
	dropQueue(myQueue);
	dropGraph(myGraph);
	sleep (1);
	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
static void setFirstURL (char *base, char *first)
{
	char *c;
	if ((c = strstr (base, "/index.html")) != NULL) {
		strcpy (first, base);
		*c = '\0';
	} else if (base[strlen (base) - 1] == '/') {
		strcpy (first, base);
		strcat (first, "index.html");
		base[strlen (base) - 1] = '\0';
	} else {
		strcpy (first, base);
		strcat (first, "/index.html");
	}
}
