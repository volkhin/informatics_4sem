all: 2sat
2sat: 2sat/sat_test.cpp
	g++ -O2 -lgtest_main -lpthread -Wall 2sat/sat_test.cpp -o bin/sat_test.bin
