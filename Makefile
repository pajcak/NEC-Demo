#DIR=../NEC-Controller
CC=g++
CFLAGS=-c -std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb
LDFLAGS=-lncurses

SOURCES=$(wildcard *.cpp) $(wildcard ../NEC-Controller/*.cpp)
SOURCES:=$(filter-out ../NEC-Controller/main.cpp, $(SOURCES))

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
