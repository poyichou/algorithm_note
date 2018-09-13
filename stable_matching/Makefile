.PHONY: all clean
CC=gcc
FLAG=-g -Wall
all: stable_matching
clean:
	rm stable_matching
%: %.c
	$(CC) $(FLAG) -o $@ $^
