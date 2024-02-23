CC = gcc
CFLAGS = -Wall -g -ansi -pedantic

all: header

myTar: myTar.c header.c
	$(CC) $(CFLAGS) -o myTar myTar.c header.c

header: header.c
	$(CC) $(CFLAGS) -o header header.c

test: myTar
	./myTar

clean: 
	rm -f header
