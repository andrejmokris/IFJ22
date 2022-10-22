CC = gcc
PROJECT = ifj22
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra

OBJS = main.o dynamic_string.o scanner.o error.o stack.o expression.o

$(PROJECT): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
		$(CC) $(CFLAGS) -c $^ -o $@

clean:
		rm $(OBJS) $(PROJECT)

zip:
		zip xmokri01 *.c *.h *.pdf Makefile