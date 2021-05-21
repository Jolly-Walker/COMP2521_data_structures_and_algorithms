// Build and display Map/Graph of Europe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"
#include "Places.h"

int main (void)
{
	puts (
		"Map of Europe\n"
		"=============\n");

	Map europe = newMap ();
	showMap (europe);
	dropMap (europe);

	return EXIT_SUCCESS;
}
