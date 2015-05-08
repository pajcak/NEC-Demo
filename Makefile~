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
