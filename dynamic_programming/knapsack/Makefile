.PHONY: all clean
CC=gcc
FLAG=-g -Wall
EXECFILE=subset_sum knapsack
all: $(EXECFILE)
clean:
	rm $(EXECFILE)
%: %.c
	$(CC) $(FLAG) -o $@ $^
