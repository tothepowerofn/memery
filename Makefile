COMPILER=g++
CFLAGS=-g -I cd2mem -I tests -std=gnu++11

CD2MEM=cd2mem/map_pointers.o cd2mem/cd2mem.o cd2mem/singly_linked.o cd2mem/exploit.o cd2mem/funcptr.o
TESTS=tests/binary_tree tests/circular_doubly_linked_list tests/circular_singly_linked_list tests/linked_tree tests/non_consecutive_doubly_linked_list tests/non_consecutive_singly_linked_list tests/scattered_doubly_linked_list tests/scattered_singly_linked_list tests/simple_doubly_linked_list tests/simple_singly_linked_list
FUNCPTR=cd2mem/funcptr.cpp cd2mem/funcptr.h cd2mem/funcptr_test.cpp 

.PHONY: all clean cd2mem map_pointers tests

all: cd2mem map_pointers tests

clean:
	rm -f $(CD2MEM)
	rm -f $(TESTS)
	rm -rf bin/* 

%.o: %.cpp
	$(COMPILER) $(CFLAGS) -c -o $@ $<

map_pointers: $(CD2MEM)
	$(COMPILER) $(CFLAGS) $(CD2MEM) -o bin/map_pointers

$(TESTS): tests/%: tests/%.c tests/exploit.c tests/tests.c
	mkdir -p bin/tests
	$(COMPILER) $(CFLAGS) $< tests/exploit.c tests/tests.c -o bin/$@

tests: $(TESTS)

funcptr: cd2mem/funcptr.cpp cd2mem/funcptr.h cd2mem/funcptr_test.cpp 
	$(COMPILER) $(CFLAGS) $(FUNCPTR) -o bin/funcptr_test
