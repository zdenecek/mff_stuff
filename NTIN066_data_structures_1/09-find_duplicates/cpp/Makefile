test: find_duplicates_test
	./$<

INCLUDE ?= .
CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -g -Wno-sign-compare -I$(INCLUDE)

find_duplicates_test: find_duplicates_test.cpp find_duplicates.h test_main.cpp
	$(CXX) $(CXXFLAGS) $(filter %.cpp,$^) -o $@

clean:
	rm -f find_duplicates_test

.PHONY: clean test
