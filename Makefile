CC = gcc
CFLAGS = -Wall -std=c99 -D_POSIX_C_SOURCE=200809L
SOURCES = mysh.c parser.c executor.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = mysh

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean