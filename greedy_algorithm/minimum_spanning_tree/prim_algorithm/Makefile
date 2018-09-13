.PHONY: all clean
CC=gcc
FLAG=-g -Wall
all: minimum_spanning_tree
clean:
	rm minimum_spanning_tree
minimum_spanning_tree: minimum_spanning_tree.c heap.c
	$(CC) $(FLAG) -o $@ $^

