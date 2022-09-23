CC = gcc
PROJECT = ifj22
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra

OBJS = main.o

$(PROJECT): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
		$(CC) $(CFLAGS) -c $^ -o $@

clean:
		rm $(OBJS) $(PROJECT)

zip:
		zip xmokri01 *.c *.h *.pdf Makefile