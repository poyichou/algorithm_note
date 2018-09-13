.PHONY: all clean
CC=gcc
FLAG=-g -Wall
all: shortest_path
clean:
	rm shortest_path
shortest_path: shortest_path.c heap.c
	$(CC) $(FLAG) -o $@ $^

