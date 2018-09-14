.PHONY: all clean
CC=gcc
FLAG=-g -Wall
EXECFILE= fibonacci weighted_interval_scheduling
SUBDIR=knapsack shortest_path
all: $(EXECFILE) sub
clean:
	rm $(EXECFILE)
	for dir in $(SUBDIR); do\
		$(MAKE) -C $$dir clean;\
	done
%: %.c
	$(CC) $(FLAG) -o $@ $^
sub:
	for dir in $(SUBDIR); do\
		$(MAKE) -C $$dir;\
	done
