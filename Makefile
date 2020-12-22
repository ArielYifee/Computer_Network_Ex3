	# Makefile for TCP project

all: server client

server: measure.c
	gcc -o server measure.c

client: sender.c
	gcc -o client sender.c
	
.PHONY: clean all
clean:
	rm -f  *.o *.a *.so server client

runs:
	./server

runc:
	./client
