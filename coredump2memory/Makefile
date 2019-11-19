CFLAGS=
COMPILER=/usr/local/bin/g++-9
NOSEFLAGS=-v -s

.PHONY: all
all: cd2mem

.PHONY: clean
clean:
	rm -f cd2mem

cd2mem: cd2mem.cpp cd2mem.h demo_cd2mem.cpp
	$(COMPILER) $(CFLAGS) demo_cd2mem.cpp cd2mem.cpp cd2mem.h -o cd2mem
