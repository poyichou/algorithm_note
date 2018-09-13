.PHONY: all clean
CC=gcc
FLAG=-g -Wall
EXECFILE=undirected-bipartiteness-and-connectivity DAG-topology-ordering
all: $(EXECFILE)
clean:
	rm $(EXECFILE)
%: %.c ../tool/queue.c
	$(CC) $(FLAG) -o $@ $^

