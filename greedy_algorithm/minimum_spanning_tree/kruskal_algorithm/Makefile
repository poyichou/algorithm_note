.PHONY: all clean
CC=gcc
FLAG=-g -Wall
all: kruskal_algorithm
clean:
	rm kruskal_algorithm
kruskal_algorithm: kruskal_algorithm.c
	$(CC) $(FLAG) -o $@ $^
