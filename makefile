CC=g++
CFLAGS=-I "/Volumes/OS X EL CAPITAN/Dropbox/Useful_C++/eigen-eigen-b9cd8366d4e8/" -std=c++14

prog: src/main.cpp
	$(CC) $(CFLAGS) src/kdtree.c -o bin/test src/main.cpp
