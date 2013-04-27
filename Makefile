CXX      := g++
CXXFLAGS := --std=gnu++0x -D_GNU_SOURCE -Wall -Wextra -Werror -g -O0 -MMD
CXXFLAGS += `pkg-config --cflags sfml-graphics`
LDFLAGS  := `pkg-config --libs sfml-graphics` -lm

all: main
	./main

main: main.o GameSprite.o Fish.o Hook.o

.PHONY: clean
clean:
	rm -f main *.o *.d

-include *.d
