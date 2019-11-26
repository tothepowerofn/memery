COMPILER=g++
CFLAGS=

.PHONY: all clean cd2mem

all: cd2mem

clean:
	rm -f bin/cd2mem

cd2mem: cd2mem/cd2mem.cpp cd2mem/cd2mem.h cd2mem/demo_cd2mem.cpp
	$(COMPILER) $(CFLAGS) -o bin/cd2mem cd2mem/demo_cd2mem.cpp cd2mem/cd2mem.cpp
