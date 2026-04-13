CC = gcc
CFLAGS = -Wall -O2
LIBS = -lraylib -lm

TARGET = game
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
