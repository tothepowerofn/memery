COMPILER=g++
CFLAGS=-g -I cd2mem -I tests -std=gnu++11

MEMERY=src/memery.o src/cd2mem.o src/singly_linked.o src/exploit.o src/funcptr.o src/multi_linked.o
TESTS=tests/binary_tree tests/circular_doubly_linked_list tests/circular_singly_linked_list tests/reverse_binary_tree tests/non_consecutive_doubly_linked_list tests/non_consecutive_singly_linked_list tests/scattered_doubly_linked_list tests/scattered_singly_linked_list tests/linear_doubly_linked_list tests/linear_singly_linked_list
FUNCPTR=src/funcptr.cpp src/funcptr.h src/funcptr_test.cpp 
STLTEST=tests/stl_test.cpp tests/exploit.h tests/exploit.c

.PHONY: all clean cd2mem map_pointers tests

all: memery map_pointers tests

clean:
	rm -f $(MEMERY)
	rm -f $(TESTS)
	rm -rf bin/* 

%.o: %.cpp
	$(COMPILER) $(CFLAGS) -c -o $@ $<

memery: $(MEMERY)
	$(COMPILER) $(CFLAGS) $(MEMERY) -o bin/memery

$(TESTS): tests/%: tests/%.c tests/exploit.c tests/tests.c
	mkdir -p bin/tests
	$(COMPILER) $(CFLAGS) $< tests/exploit.c tests/tests.c -o bin/$@

tests: $(TESTS)

funcptr: memery/funcptr.cpp memery/funcptr.h memery/funcptr_test.cpp 
	$(COMPILER) $(CFLAGS) $(FUNCPTR) -o bin/funcptr_test

stltest: $(STLTEST)
	$(COMPILER) $(CFLAGS) $(STLTEST) -o bin/stl_test
