C = g++
F = -g -Wextra -Wall -std=c++0x
O = Graph

all: Graph.a Driver.o
	$C $F -o Graph Driver.o Graph.a
Graph.o: Graph.cpp Graph.h
	$C $F -c Graph.cpp

Graph.a: Graph.o
	ar -rcs Graph.a Graph.o
	rm Graph.o

Driver.o: Driver.cpp
	$C $F -c Driver.cpp

clean:
	rm -rf *.o $O
