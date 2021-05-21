# COMP2521 19T1 ... lab 8

CC	= 2521 3c

.PHONY: all
all: travel

travel:		travel.o Graph.o Queue.o
travel.o:	travel.c Graph.h
Graph.o:	Graph.c Graph.h Queue.h
Queue.o:	Queue.c Queue.h Item.h

.PHONY: clean
clean:
	-rm -f travel travel.o Graph.o Queue.o

.PHONY: give
give: Graph.c
	give cs2521 lab08 $^
