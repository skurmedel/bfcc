SRC_DIR=src/
SOURCES=$(wildcard $(SRC_DIR)*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
TST_DIR=test/
TEST_SOURCES=$(wildcard $(TST_DIR)*.c)
TEST_OBJECTS=$(patsubst %.c, %.o, $(TEST_SOURCES))
TESTS=$(patsubst %.c, %.test, $(TEST_SOURCES))
bfcc=bin/bfcc

SOURCES+=lib/parg/parg.c

CC=gcc
CFLAGS=-O2 -DNDEBUG -fomit-frame-pointer -Ilib/parg/ -Wall

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
	-@rm $(TEST_OBJECTS)
	-@rm $(TESTS)

help:
	-@echo "Targets: all clean helloworld"

helloworld: all
	./bin/bfcc examples/helloworld.bf > helloworld.bf.c
	gcc helloworld.bf.c -o helloworld

$(TST_DIR)%.test: $(filter-out %main.o,$(OBJECTS)) $(TST_DIR)%.o lib/unity/src/unity.o
	$(CC) $(CFLAGS) $^ -o $@
	@echo "---------------------------------"
	@echo "RUNNING TEST $@"
	@echo "---------------------------------"
	-./$@
	@echo "---------------------------------"
	rm $@

tests: CFLAGS+=-Isrc/ -Ilib/unity/src -DTESTING
tests: $(TESTS) 
	

.PHONY : all setup clean help
