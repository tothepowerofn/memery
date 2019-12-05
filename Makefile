COMPILER=g++
CFLAGS=-g -I cd2mem -I tests -std=c++11 

CD2MEM=cd2mem/map_pointers.o cd2mem/cd2mem.o cd2mem/singly_linked.o

.PHONY: all clean cd2mem map_pointers tests

all: cd2mem map_pointers tests

clean:
	rm -f cd2mem/demo_cd2mem.o
	rm -f cd2mem/cd2mem.o
	rm -f cd2mem/map_pointers.o
	rm -f tests/data_structures.o
	rm -f bin/demo_cd2mem
	rm -f bin/map_pointers
	rm -f bin/tests

%.o: %.c
	gcc -c -o $@ $<

map_pointers: $(CD2MEM)
	$(COMPILER) $(CFLAGS) $(CD2MEM) -o bin/map_pointers

tests: tests/data_structures.o
	$(COMPILER) $(CFLAGS) tests/data_structures.o -o bin/tests

