CXX      := g++
CXXFLAGS := --std=gnu++0x -D_GNU_SOURCE -Wall -Wextra -Werror -g -O0 -MMD
CXXFLAGS += `pkg-config --cflags sfml-graphics sfml-audio`
LDFLAGS  := `pkg-config --libs sfml-graphics sfml-audio` -lm

all: main
	./main

main: main.o GameSprite.o Fish.o Hook.o TextParticle.o

.PHONY: clean
clean:
	rm -f main *.o *.d

-include *.d
