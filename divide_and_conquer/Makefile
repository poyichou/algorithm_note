.PHONY: all clean
CC=gcc
FLAG=-g -Wall
EXECFILE=counting_inversions merge_sort
all: $(EXECFILE)
clean:
	rm $(EXECFILE)
%: %.c
	$(CC) $(FLAG) -o $@ $^
