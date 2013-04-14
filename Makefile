SRC_DIR=src/
SOURCES=$(wildcard $(SRC_DIR)*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
bfcc=bin/bfcc

CC=gcc
CFLAGS=-O2 -DNDEBUG -fomit-frame-pointer

all: setup $(bfcc)

$(bfcc): $(OBJECTS)
	$(CC) $^ -o $@

$(SRC_DIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

setup:
	-echo "Setting up bin dir."
	-@mkdir bin

clean:
	-@rm $(OBJECTS)
	-@rm -rf bin
