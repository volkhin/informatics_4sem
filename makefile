CC=g++
CC_FLAGS=$(CXXFLAGS)
CC_FLAGS+=-I.
CC_FLAGS+=-Wall -Werror
CC_FLAGS+=-lgtest_main -lgtest -lpthread
OUTDIR=bin/

all: suffix_tree 2sat

suffix_tree: suffix_tree/suffix_tree_test.cpp
	$(CC) $(CC_FLAGS) $< -o $(OUTDIR)$@.bin

2sat: 2sat/sat_test.cpp
	$(CC) $(CC_FLAGS) $< -o $(OUTDIR)$@.bin
