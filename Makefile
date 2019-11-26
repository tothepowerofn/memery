COMPILER=g++
CFLAGS=

.PHONY: all clean cd2mem tests

all: cd2mem tests

clean:
	rm -f bin/cd2mem

cd2mem: cd2mem/cd2mem.cpp cd2mem/cd2mem.h cd2mem/demo_cd2mem.cpp
	$(COMPILER) $(CFLAGS) -o bin/cd2mem cd2mem/demo_cd2mem.cpp cd2mem/cd2mem.cpp

tests: tests/data_structures.c tests/data_structures.h
	$(COMPILER) $(CFLAGS) -o bin/linked_lists tests/data_structures.c
