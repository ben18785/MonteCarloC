CC=g++
CFLAGS=-I "/Volumes/OS X EL CAPITAN/Dropbox/Useful_C++/eigen-eigen-b9cd8366d4e8/" -std=c++14
SOURCES=src/*.c*
OBJECTS=*.o

everything:
	$(CC) $(CFLAGS) -c $(SOURCES)
	$(CC) $(OBJECTS) -o bin/test
	rm -rf *.o

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o bin/test

main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp

mosquito.o: src/mosquito.cpp src/mosquito.hpp src/mathematical.hpp src/mathematical.cpp
	$(CC) $(CFLAGS) -c src/mosquito.cpp

kdtree.o: src/kdtree.c src/kdtree.h
	$(CC) $(CFLAGS) -c src/kdtree.c

mathematical.o: src/mathematical.cpp src/mathematical.hpp
	$(CC) $(CFLAGS) -c src/mathematical.cpp

clean:
	rm -rf *.o
