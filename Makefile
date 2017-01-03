SRC_DIR=src/
SOURCES=$(wildcard $(SRC_DIR)*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
bfcc=bin/bfcc

SOURCES+=lib/parg/parg.c

CC=gcc
CFLAGS=-O2 -DNDEBUG -fomit-frame-pointer -Ilib/parg/

all: setup $(bfcc)

$(bfcc): $(OBJECTS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

setup:
	-@echo "Setting up bin dir."
	-@mkdir -p bin

clean:
	-@rm $(OBJECTS)
	-@rm -rf bin

help:
	-@echo "Targets: all clean helloworld"

helloworld: all
	./bin/bfcc examples/helloworld.bf > helloworld.bf.c
	gcc helloworld.bf.c -o helloworld

.PHONY : all setup clean help
