CXX      := g++
CXXFLAGS := --std=gnu++0x -D_GNU_SOURCE -Wall -Wextra -g -O0 -MMD
CXXFLAGS += `pkg-config --cflags sfml-graphics`
LDFLAGS  := `pkg-config --libs sfml-graphics`

all: main

main: main.o

.PHONY: clean
clean:
	rm -f main *.o *.d
