COMPILER=g++
CFLAGS=-g -I cd2mem -I tests -std=c++11 

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

demo_cd2mem.o: cd2mem/demo_cd2mem.cpp cd2mem/cd2mem.h
	$(COMPILER) $(CFLAGS) -c cd2mem/demo_cd2mem.cpp -o cd2mem/demo_cd2mem.o

cd2mem.o: cd2mem/cd2mem.cpp cd2mem/cd2mem.h
	$(COMPILER) $(CFLAGS) -c cd2mem/cd2mem.cpp -o cd2mem/cd2mem.o

map_pointers.o: cd2mem/map_pointers.cpp cd2mem/cd2mem.h
	$(COMPILER) $(CFLAGS) -c cd2mem/map_pointers.cpp -o cd2mem/map_pointers.o

data_structures.o: tests/data_structures.c tests/data_structures.h
	$(COMPILER) $(CFLAGS) -c tests/data_structures.c -o tests/data_structures.o

demo_cd2mem: demo_cd2mem.o cd2mem.o
	$(COMPILER) $(CFLAGS) cd2mem/demo_cd2mem.o cd2mem/cd2mem.o -o bin/demo_cd2mem

map_pointers: map_pointers.o cd2mem.o
	$(COMPILER) $(CFLAGS) cd2mem/map_pointers.o cd2mem/cd2mem.o -o bin/map_pointers

tests: data_structures.o
	$(COMPILER) $(CFLAGS) tests/data_structures.o -o bin/tests

