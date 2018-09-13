.PHONY: all clean
CC=gcc
FLAG=-g -Wall
SUBDIR=kruskal_algorithm prim_algorithm
all:
	for dir in $(SUBDIR); do\
		$(MAKE) -C $$dir;\
	done
clean:
	for dir in $(SUBDIR); do\
		$(MAKE) -C $$dir clean;\
	done

