COMPILER=g++
CFLAGS=-g -I cd2mem -I tests -std=gnu++11

CD2MEM=cd2mem/map_pointers.o cd2mem/cd2mem.o cd2mem/singly_linked.o cd2mem/exploit.o
TESTS=tests/data_structures.o tests/exploit.o
FUNCPTR=cd2mem/funcptr.cpp cd2mem/funcptr.h cd2mem/funcptr_test.cpp 

.PHONY: all clean cd2mem map_pointers tests

all: cd2mem map_pointers tests

clean:
	rm -f $(CD2MEM)
	rm -f $(TESTS)
	rm -f bin/* 

%.o: %.cpp
	$(COMPILER) $(CFLAGS) -c -o $@ $<

map_pointers: $(CD2MEM)
	$(COMPILER) $(CFLAGS) $(CD2MEM) -o bin/map_pointers

tests: tests/data_structures.o tests/exploit.o
	$(COMPILER) $(CFLAGS) $(TESTS) -o bin/tests

funcptr: cd2mem/funcptr.cpp cd2mem/funcptr.h cd2mem/funcptr_test.cpp 
	$(COMPILER) $(CFLAGS) $(FUNCPTR) -o bin/funcptr_test
