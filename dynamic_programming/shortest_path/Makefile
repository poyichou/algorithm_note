.PHONY: all clean
CC=gcc
FLAG=-g -Wall
EXECFILE= negative_cycle_detection shortest_path
all: $(EXECFILE)
clean:
	rm $(EXECFILE)
%: %.c
	$(CC) $(FLAG) -o $@ $^
