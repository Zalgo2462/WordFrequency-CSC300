# Makefile for zipf program.

# Author: Brady Shimp
# Class: CSC300 Data Structures, Fall 2015

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
LINK = g++

# Preprocessor and compiler flags (turn on warnings, optimization and debugging):
# CPPFLAGS = <preprocessor flags go here>
CFLAGS = -Wall -O -g -std=c++0x
CXXFLAGS = $(CFLAGS)

#-----------------------------------------------------------------------
# Specific targets:

# MAKE allows the use of "wildcards", to make writing compilation instructions
# a bit easier. GNU make uses $@ for the target and $^ for the dependencies.

all:	zipf

# zipf program
zipf:	zipf.o hashmap.o tokenize.o
	$(LINK) -o $@ $^

clean:
	rm -f *.o *~
