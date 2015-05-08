#    File: Makefile
#    Copyright (C) 2015  Patrik Faistaver
#    
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

CC=g++
CFLAGS=-c -std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb
LDFLAGS=-lncurses -lpthread -ldl

SOURCES=$(wildcard *.cpp)

OBJS=$(SOURCES:.cpp=.o)
EXEC=demo

all: compile

compile: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -L. -o $@ $(LDFLAGS)

run:
	./$(EXEC)

%.o: %.cpp 
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJS) $(EXEC)
