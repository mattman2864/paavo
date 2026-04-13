CC = gcc
CFLAGS = -Wall -O2 -Isrc
LIBS = -lraylib -lm

TARGET = build/paavo
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

all: build $(TARGET)

build:
	mkdir -p build

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

clean:
	rm -rf build

.PHONY: all clean
