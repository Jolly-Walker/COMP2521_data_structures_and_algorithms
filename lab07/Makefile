# COMP2521 19T1 ... week 7 lab

CC	= 2521 3c

.PHONY: all
all: ts tq tk tg crawl

crawl:		crawl.o url_file.o html.o set.o queue.o stack.o graph.o
crawl:		LDFLAGS	= $(shell pkg-config --libs-only-L libcurl)
crawl:		LDLIBS	= $(shell pkg-config --libs-only-l libcurl)

crawl.o:	crawl.c url_file.h html.h set.h queue.h stack.h graph.h
crawl.o:	CFLAGS	= $(shell pkg-config --cflags libcurl)
url_file.o:	url_file.c url_file.h
url_file.o:	CFLAGS	= $(shell pkg-config --cflags libcurl)
html.o:		html.c html.h

ts:		ts.o set.o
ts.o:		ts.c set.h
set.o:		set.c set.h

tq:		tq.o queue.o
tq.o:		tq.c queue.h
queue.o:	queue.c queue.h

tk:		tk.o stack.o
tk.o:		tk.c stack.h
stack.o:	stack.c stack.h

tg:		tg.o graph.o
tg.o:		tg.c graph.h
graph.o:	graph.c graph.h

.PHONY: clean
clean:
	-rm -f crawl crawl.o url_file.o html.o
	-rm -f ts ts.o set.o
	-rm -f tq tq.o queue.o
	-rm -f tk tk.o stack.o
	-rm -f tg tg.o graph.o

.PHONY: give
give: crawl.c
	give cs2521 lab07 $^
