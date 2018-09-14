.PHONY: all clean
CC=gcc
FLAG=-g -Wall
EXECFILE=flow_network
all: $(EXECFILE)
clean:
	rm $(EXECFILE)
%: %.c
	$(CC) $(FLAG) -o $@ $^
