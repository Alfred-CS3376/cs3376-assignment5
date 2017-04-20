#MakeFile for Assg5
#Created on 4/8/17

CC = g++

TwoPipesTwoChildren.o : TwoPipesTwoChildren.cpp
	$(CC) -c TwoPipesTwoChildren.cpp
TwoPipesTwoChildren : TwoPipesTwoChildren.o
	$(CC) -o TwoPipesTwoChildren TwoPipesTwoChildren.o
clean:
	rm -f core TwoPipesTwoChildren TwoPipesTwoChildren.o
