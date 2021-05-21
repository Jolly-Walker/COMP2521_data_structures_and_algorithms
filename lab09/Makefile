# COMP2521 19T1 ... lab 5

CC	= 2521 3c

.PHONY: all
all: pl euro conn

pl:		pl.o Places.o
pl.o:		pl.c Places.h

euro:		euro.o Map.o Places.o
euro.o:		euro.c Map.c Places.h

conn:		conn.o Map.o Places.o
conn.o:		conn.c Map.c Places.h

Places.o:	Places.c Places.h
Map.o:		Map.c Map.h Places.h
Queue.o:	Queue.c Queue.h Item.h

.PHONY: clean
clean:
	-rm -f pl pl.o
	-rm -f euro euro.o
	-rm -f conn conn.o
	-rm -f Places.o Map.o Queue.o

.PHONY: give
give: Map.c
	give cs2521 lab05 $^
