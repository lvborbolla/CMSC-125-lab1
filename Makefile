CC = gcc
CFLAGS = -Wall -Wextra -std=c11

OBJ = mysh.o parser.o executor.o

mysh: $(OBJ)
	$(CC) $(OBJ) -o mysh

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o mysh
