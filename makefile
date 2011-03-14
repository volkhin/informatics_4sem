CC=g++
CC_FLAGS=$(CXXFLAGS)
CC_FLAGS+=-I.
CC_FLAGS+=-Wall -Werror
CC_FLAGS+=-lgtest_main -lgtest -lpthread
OUTDIR=bin/

all: 2sat
2sat: 2sat/sat_test.cpp
	$(CC) $(CC_FLAGS) $< -o $(OUTDIR)$@.out
