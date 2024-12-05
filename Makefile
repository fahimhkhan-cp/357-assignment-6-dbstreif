CC = gcc
CFLAGS = -Wall -g
TARGET = corgis_processor

HEAD = corgis_struct.h helpers.h operations.h
SRC = corgis_processor.c helpers.c operations.c
OBJ = corgis_processor.o helpers.o operations.o

all: $(TARGET)

$(TARGET): $(SRC) $(OBJ)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm $(OBJ) $(TARGET)

.PHONY: all clean
