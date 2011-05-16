CC=g++
CC_FLAGS=$(CXXFLAGS)
CC_FLAGS+=-I.
CC_FLAGS+=-Wall -Werror
CC_FLAGS+=-lgtest_main -lgtest -lpthread
OUTDIR=bin/

all: suffix_tree

suffix_tree: suffix_tree/*
	$(CC) $(CC_FLAGS) suffix_tree/suffix_tree_test.cpp -o $(OUTDIR)suffix_tree.bin

2sat: 2sat/sat_test.cpp 2sat/sat.h
	$(CC) $(CC_FLAGS) 2sat/sat_test.cpp -o $(OUTDIR)2sat.bin
