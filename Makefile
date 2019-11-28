COMPILER=g++
CFLAGS=-g

.PHONY: all clean cd2mem tests

all: cd2mem map_pointers tests

clean:
	rm -f bin/cd2mem

cd2mem: cd2mem/cd2mem.cpp cd2mem/cd2mem.h cd2mem/demo_cd2mem.cpp
	$(COMPILER) $(CFLAGS) -o bin/cd2mem cd2mem/demo_cd2mem.cpp cd2mem/cd2mem.cpp

map_pointers: cd2mem/cd2mem.cpp cd2mem/cd2mem.h cd2mem/map_pointers.cpp
	$(COMPILER) $(CFLAGS) -o bin/map_pointers cd2mem/map_pointers.cpp cd2mem/cd2mem.cpp

tests: tests/data_structures.c tests/data_structures.h
	$(COMPILER) $(CFLAGS) -o bin/linked_lists tests/data_structures.c
