.PHONY: all clean sub
CC=gcc
FLAG=-g -Wall
SUBDIR=shortest_path minimum_spanning_tree
EXECFILES=interval-scheduling interval-partitioning interval-scheduling-with-deadline
all: $(EXECFILES) sub
clean:
	rm $(EXECFILES)
	for dir in $(SUBDIR); do\
		$(MAKE) -C $$dir clean;\
	done
sub:
	for dir in $(SUBDIR); do\
		$(MAKE) -C $$dir;\
	done
%: %.c
	$(CC) $(FLAG) -o $@ $^

