#NAME: Andrew Gendreau
#DATE: Oct 14th 2016
#VERSION: 1.0
#FILENAME: makefile
#PURPOSE: This is the makefile for mys15p.c 
#AUTHOR: Andrew Gendreau

SOURCES = pathfinder.c hashList.c yggdrasil.c queue.c puzzleSolver.c functions-2.c arrayReader.c mys15p.c
OBJECTS = pathfinder.o hashList.o yggdrasil.o queue.o puzzleSolver.o functions-2.o arrayReader.o mys15p.o
CC = gcc
CFLAGS = -g -Wall

#all rule, says to create an executable from all the compiled object files called myhost.
#AUTHOR: Andrew Gendreau
all: mys15p

mys15p: $(OBJECTS)
	$(CC) $(CFLAGS) -o mys15p $(OBJECTS)

pathfinder.o: pathfinder.c pathfinder.h
		$(CC) $(CFLAGS)  -c pathfinder.c

hashList.o: hashList.c hashList.h
		$(CC) $(CFLAGS)  -c hashList.c

yggdrasil.o: yggdrasil.c yggdrasil.h
		$(CC) $(CFLAGS) -c yggdrasil.c

queue.o: queue.c queue.h
		$(CC) $(CFLAGS) -c queue.c

functions-2.o: functions-2.c functions-2.h
	$(CC) $(CFLAGS) -c functions-2.c

arrayReader.o: arrayReader.c arrayReader.h
	$(CC) $(CFLAGS) -c arrayReader.c

mys15p.o: mys15p.c
	$(CC) $(CFLAGS) -c mys15p.c

#clean rule
#AUTHOR: Andrew Gendreau
clean:
	-rm mys15p $(OBJECTS) 
